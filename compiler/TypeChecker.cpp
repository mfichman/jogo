/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "TypeChecker.hpp"
#include "Feature.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Location.hpp"
#include <iostream>
#include <cassert>
#include <set>

using namespace std;

TypeChecker::TypeChecker(Environment* environment) :
    environment_(environment) {

    if (environment_->errors()) {
        return;
    }
    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void TypeChecker::operator()(Module* feature) {
    module_ = feature;

    std::string module_name = feature->name()->string();
    if (module_name.empty()) {
        module_name = "root module";
    } else {
        module_name = "'" + module_name + "'";
    }

    // Iterate through all functions and classes in the module, and check 
    // their semantics.  Ensure there are no duplicate functions or classes.
    std::set<String::Ptr> features;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (features.find(func->name()) != features.end()) {
                cerr << f->location();
                cerr << "Duplicate definition of function '";
                cerr << func->name() << "' in " << module_name << endl;
            }
            features.insert(func->name());
        } else if (Class::Ptr clazz = dynamic_cast<Class*>(f.pointer())) {
            if (features.find(clazz->name()) != features.end()) {
                cerr << f->location();
                cerr << "Duplicate definition of class '";
                cerr << clazz->name() << "' in " << module_name << endl;
            }
            features.insert(clazz->name());
        }
        f(this);
    }
}

void TypeChecker::operator()(Class* feature) {
    class_ = feature;
    enter_scope();

    // Make sure that a module with this name doesn't already exist.
    if (environment_->module(feature->type()->qualified_name())) {
        cerr << feature->location();
        cerr << "Type name '" << feature->type()->qualified_name();
        cerr << "' conflicts with an existing module name" << endl; 
    }

    // Check interface/struct/object baseclass and make sure that 
    // disallowed things aren't included.
    for (Type::Ptr m = feature->mixins(); m; m = m->next()) {
        if (m->clazz()) {
            if (feature->is_interface() && !m->clazz()->is_interface()) {
                cerr << m->location();
                cerr << "Mix-in for interface '" << feature->name();
                cerr << "' must be an interface" << endl;
                break;
            } else if (feature->is_object() && m->clazz()->is_interface()) {
                cerr << m->location();
                cerr << "Mix-in for object type '" << feature->name();
                cerr << "' cannot be an interface" << endl; 
                break;
            } else if (feature->is_value() && !m->clazz()->is_value()) {
                cerr << m->location();
                cerr << "Mix-in for value type '" << feature->name();
                cerr << "' must be a value type" << endl; 
                break;
            }
         
        }
    }

    // Check for illegal mixins for the current class.  Certain object types
    // cannot be combined, as below:
    if (feature->is_object() && feature->is_interface()) {
        cerr << feature->location();
        cerr << "Class has both object and interface mixins";
        cerr << endl;    
    } else if (feature->is_interface() && feature->is_value()) {
        cerr << feature->location();
        cerr << "Class has both interfaces and value mixins";
        cerr << endl;
    } else if (feature->is_value() && feature->is_object()) {
        cerr << feature->location();
        cerr << "Class has both value and object mixins";
        cerr << endl;
    }

    // Iterate through all the features and add the functions and variables in
    // the current scope.
    std::set<String::Ptr> features;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            if (variable(attr->name())) {
                cerr << f->location();
                cerr << "Duplicate definition of attribute '";
                cerr << attr->name() << "'" << endl;
            } else {
                variable(attr->name(), attr->type());
            }
            continue;
        }
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (features.find(func->name()) != features.end()) {
                cerr << f->location();
                cerr << "Duplicate definition of function '";
                cerr << func->name() << "'" << endl;
            }
            features.insert(func->name());
            continue;
        }
    }

    // Now type-check all the sub-features.
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this); // lol
    }

    exit_scope();
}

void TypeChecker::operator()(Formal* formal) {
    Type::Ptr type = formal->type();
    type(this);
}

void TypeChecker::operator()(StringLiteral* expression) {
    expression->type(environment_->string_type()); 
}

void TypeChecker::operator()(IntegerLiteral* expression) {
    expression->type(environment_->integer_type()); 
}

void TypeChecker::operator()(FloatLiteral* expression) {
    expression->type(environment_->float_type()); 
}

void TypeChecker::operator()(BooleanLiteral* expression) {
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Let* expression) {
    // For a lot expression, introduce the new variables in a new scope, and 
    // then check the block.
    enter_scope();
    Statement::Ptr block = expression->block();
    for (Statement::Ptr v = expression->variables(); v; v = v->next()) {
        v(this);
    }
    block(this);
    exit_scope();
}

void TypeChecker::operator()(Binary* expression) {
    // Checks primitive binary expressions.  These include basic logic
    // operations, which cannot be overloaded as methods.
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    left(this);
    right(this);

    if (environment_->name("or") == expression->operation()
        || environment_->name("and") == expression->operation()) {

        if (left->type()->clazz()->is_value()) {
            cerr << left->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
        if (right->type()->clazz()->is_value()) {
            cerr << right->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Unary* expression) {
    // Check primitive unary expressions.  These include basic logic 
    // operations, which cannot be overloaded as methods.
    Expression::Ptr child = expression->child();
    child(this);
    if (environment_->name("not") == expression->operation()) {
        if (child->type()->clazz()->is_value()) {
            cerr << expression->location();
            cerr << "Value types cannot be converted to 'Bool'";
            cerr << endl;
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(environment_->boolean_type());
}

void TypeChecker::operator()(Call* expression) {
    // Evaluate types of argument expressions, then perform type checking
    // on the body of the function.
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Look up the function by name in the current context.  The function may
    // be a member of the current module, or of a module that was imported
    // in the current compilation unit.
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope();
    Function::Ptr func = expression->file()->function(scope, id);
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    expression->type(func->type());
    
    // Check argument types versus formal parameter types.  Check the arity
    // of the function as well.
    Expression::Ptr arg = expression->arguments(); 
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        if (!arg->type()->subtype(formal->type())) {
            cerr << arg->location();
            cerr << "Argument does not conform to type '";
            cerr << formal->type() << "'";
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function ";
        cerr << expression->identifier();
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function ";
        cerr << expression->identifier();
        cerr << endl;
    }
}

void TypeChecker::operator()(Dispatch* expression) {

    // Evaluate types of argument expressions and the receiver
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Get the class associated with the receiver (always the first argument)
    Expression::Ptr receiver = expression->arguments();
    if (receiver->type() == environment_->no_type()) {
        expression->type(environment_->no_type());
        return;
    }
    Class::Ptr clazz = receiver->type()->clazz();
    if (!clazz) {
        cerr << expression->location();
        cerr << "Undefined class '";
        cerr << receiver->type() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    
    // Look up the function using the class object.
    Function::Ptr func = clazz->function(expression->identifier()); 
    if (!func) {
        cerr << expression->location();
        cerr << "Undeclared function '";
        cerr << expression->identifier() << "' in class '";
        cerr << clazz->type() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    } 
    expression->type(func->type());

    // Check argument types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        Type::Ptr ft = formal->type();
        if (formal->type() == environment_->self_type()) {
            ft = receiver->type();
        }
        if (!arg->type()->subtype(ft)) {
            cerr << arg->location();
            cerr << "Argument does not conform to type '";
            cerr << formal->type() << "'";
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to function '";
        cerr << expression->identifier() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(Construct* expression) {
    // Evaluate type of argument expression
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this); 
    }

    // Look up constructor class
    Class::Ptr clazz = expression->type()->clazz(); 
    if (!clazz) {
        cerr << expression->location();
        cerr << "Undefined class '";
        cerr << expression->type() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }
    if (clazz->is_interface()) {
        cerr << expression->location();
        cerr << "Constructor called for interface type '";
        cerr << clazz->name() << "'";
        cerr << endl;
        expression->type(environment_->no_type());
        return;
    }

    // Look up the constructor using the class object.
    Function::Ptr constr = clazz->function(environment_->name("@init"));

    // Check arguments types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = constr ? constr->formals() : 0;
    while (arg && formal) {
        if (!arg->type()->subtype(formal->type())) {
            cerr << arg->location();
            cerr << "Argument does not conform to type '";
            cerr << formal->type() << "'";
            cerr << endl;
        }
        arg = arg->next();
        formal = formal->next();
    } 
    if (arg) {
        cerr << expression->location();
        cerr << "Too many arguments to constructor '";
        cerr << expression->type() << "'";
        cerr << endl;
    }
    if (formal) {
        cerr << expression->location();
        cerr << "Not enough arguments to constructor '";
        cerr << expression->type() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(Identifier* expression) {
    // Determine the type of the identifier from the variable store.  If the
    // variable is undeclared, set the type of the expression to no-type to
    // prevent further error messages.
    Type::Ptr type = variable(expression->identifier());
    if (!type) {
        cerr << expression->location();
        cerr << "Undeclared identifier \"";
        cerr << expression->identifier();
        cerr << "\"";
        cerr << endl;
        expression->type(environment_->no_type());
    } else {
        expression->type(type);
    }
}

void TypeChecker::operator()(Empty* expression) {
}

void TypeChecker::operator()(Block* statement) {
    // Enter a new scope, then check each of the children.  Perform
    // reachability analysis to make sure that the first 'return' statement is
    // always the last statement in the block.
    enter_scope();
    return_ = 0;
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        if (return_) {
            cerr << s->location();
            cerr << "Statement is unreachable";
            cerr << endl;
            break;
        } else {
            s(this);
        }
    }
    exit_scope();
}

void TypeChecker::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void TypeChecker::operator()(While* statement) {
    // Check the while statement guard type, and then check all the branches.
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();

    guard(this);
    Type* t = guard->type();
    assert(t);
    if (!environment_->boolean_type()->equals(guard->type())) {
        cerr << statement->location();
        cerr << "While statement guard expression must have type '";
        cerr << environment_->boolean_type() << "'";
        cerr << endl;
    }
    block(this);
}

void TypeChecker::operator()(Conditional* statement) {
    // Ensure that the guard is convertible to type bool.  This is always true
    // unless the guard is a value type.
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (guard->type()->clazz()->is_value()) {
        cerr << guard->location();
        cerr << "Value types cannot be converted to 'Bool'";
        cerr << endl;
    }
    true_branch(this);
    false_branch(this);
}

void TypeChecker::operator()(Variable* statement) {
    // Ensure that a variable is not duplicated, or re-initialized with a 
    // different type.  This function handles both assignment and variable
    // initialization.
    Expression::Ptr init = statement->initializer();
    init(this);

    if (statement->type()) {
        Type::Ptr type = statement->type();
        type(this);
        if (!type->clazz()) {
            variable(statement->identifier(), environment_->no_type());
            return;
        }
    }

    Type::Ptr type = variable(statement->identifier());


        
    // Attempt to assign void to a variable during initialization.
    if (init->type() && init->type()->equals(environment_->void_type())) {
        cerr << init->location();
        cerr << "Void value assigned to variable '";
        cerr << statement->identifier() << "'" << endl;
        variable(statement->identifier(), environment_->no_type());
        return;
    }
    
    if (statement->type()) {
        // The variable was declared with an explicit type, but the variable
        // already exists.
        if (type) {
            cerr << statement->location();
            cerr << "Duplicate definition of variable '";
            cerr << statement->identifier() << "'" << endl;
            return;
        }

        // The variable was declared with an explicit type, and the init
        // does not conform to that type.
        if (init->type() && !init->type()->subtype(statement->type())) {
            cerr << init->location();
            cerr << "Expression does not conform to type '";
            cerr << statement->type() << "'" << endl;
            return;
        }

        variable(statement->identifier(), statement->type());
        init->type(statement->type());
    } else {
        // The variable was already declared, but the init type is not
        // compatible with the variable type.
        if (type && !init->type()->subtype(type)) {
            cerr << init->location();
            cerr << "Expression does not conform to type '";
            cerr << type << "' in assignment of '";
            cerr << statement->identifier() << "'" << endl;
            return;
        }
        variable(statement->identifier(), init->type()); 
    }
}

void TypeChecker::operator()(Return* statement) {
    Expression::Ptr expression = statement->expression();
    if (expression) {
        expression(this);
        return_ = expression->type();
    }
    if (!expression->type()) {
        expression->type(environment_->void_type());
        return;
    }
    if (!expression->type()->subtype(scope_->type())) {
        cerr << statement->location();
        cerr << "Return must conform to type '";
        cerr << scope_->type() << "'";
        cerr << endl;
    }
}

void TypeChecker::operator()(When* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    block(this);
}

void TypeChecker::operator()(Fork* statement) {
    assert("Not supported");
}

void TypeChecker::operator()(Yield* statement) {
    assert("Not supported");
}

void TypeChecker::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);

    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        b(this);
        When::Ptr when = static_cast<When*>(b.pointer());
        if (!guard->type()->equals(when->guard()->type())) {
            cerr << statement->location();
            cerr << "Case expression does not conform to type '";
            cerr << guard->type() << "'";
            cerr << endl;
        }
    }
}

void TypeChecker::operator()(Function* feature) {
    // Begin type checking a function.  The formal parameters are type-checked
    // first, followed by the function body.  If the function belongs to an
    // an interface, the function should not have a body.
    Statement::Ptr block = feature->block();
    scope_ = feature;
    enter_scope();

    // Check all formal parameters.
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        Type::Ptr type = f->type();
        type(this);
        variable(f->name(), f->type());
    }
    Type::Ptr type = feature->type();
    type(this);
    
    // Check the different combinations of prototype/body/no-body, method,
    // and function.  Prototypes (as in interface definitions) should not 
    // have a body.
    if (block && class_ && class_->is_interface()) {
        cerr << block->location();
        cerr << "Interface method '" << feature->name();
        cerr << "' has a body" << endl;
    } else if (block) {
        block(this); 
        if (type != environment_->void_type() && !return_) {
            cerr << feature->location();
            cerr << "Function '" << feature->name();
            cerr << "' must return a value" << endl;     
        }
    } else if (!class_) {
        cerr << feature->location();
        cerr << "Function '" << feature->name() << "' has no body" << endl;
    } else if (!class_->is_interface()) {
        cerr << feature->location();
        cerr << "Method '" << feature->name() << "' has no body" << endl;
    }

    exit_scope();
}

void TypeChecker::operator()(Attribute* feature) {
    // Make sure that the attribute type conforms to the declared type of
    // the attribute, if there is one.
    Expression::Ptr initializer = feature->initializer();
    initializer(this);

    // No explicitly declared type, but the initializer can be used to infer
    // the type.
    if (!feature->type()) {
        variable(feature->name(), feature->initializer()->type());     
        return;
    }

    // Check that the feature type exists.
    Type::Ptr type = feature->type();
    type(this);

    // Make sure that the initializer and the feature type conform.
    if (initializer->type() && !initializer->type()->subtype(feature->type())) {
        cerr << feature->location();
        cerr << "Expression does not conform to type '";
        cerr << initializer->type() << "'";
        cerr << endl;
    }

    if (feature->type()) {
        variable(feature->name(), feature->type());
    } else {
        variable(feature->name(), initializer->type());
    }
}

void TypeChecker::operator()(Import* feature) {
}

void TypeChecker::operator()(Type* type) {
    // Check to make sure that the type has a defined class.  Also, check
    // all generics that are part of the type to make sure that they resolve.
    if (type == environment_->self_type() || type == environment_->no_type() 
            || type == environment_->void_type()) {
        return;
    }
    if (!type->clazz()) {
        cerr << type->location();
        cerr << "Undefined type '" << type << "'";
        cerr << endl;
        return;
    }

    // Check generics for type resolution
    for (Generic::Ptr g = type->generics(); g; g = g->next()) {
        if (!g->type()->clazz() && !type->equals(environment_->self_type())) {
            cerr << type->location() << endl;
            cerr << "Undefined type '" << type << "'";
            cerr << endl;
        }
    }
}

Type* TypeChecker::variable(String* name) {
    vector<map<String::Ptr, Type::Ptr> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<String::Ptr, Type::Ptr>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return 0;
}

void TypeChecker::variable(String* name, Type* type) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, type));
}

void TypeChecker::enter_scope() {
    variable_.push_back(map<String::Ptr, Type::Ptr>());
}

void TypeChecker::exit_scope() {
    variable_.pop_back();
}

