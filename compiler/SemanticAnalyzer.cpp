/*
 * Copyright (c) 2011 Matt Fichman
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

#include "SemanticAnalyzer.hpp"
#include "Feature.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Location.hpp"
#include <iostream>
#include <cassert>
#include <set>
#include <algorithm>

using namespace std;

SemanticAnalyzer::SemanticAnalyzer(Environment* environment) :
    env_(environment),
    err_(Stream::sterr()) {

    operator()(env_->float_type());
    operator()(env_->int_type());
    operator()(env_->string_type());
    operator()(env_->bool_type());

    if (env_->errors()) {
        return;
    }
    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
    }    
    err_->flush();
}

void SemanticAnalyzer::operator()(Module* feature) {
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
                err_ << f->location();
                err_ << "Duplicate definition of function '";
                err_ << func->name() << "' in " << module_name << "\n";
                env_->error();
            }
            features.insert(func->name());
        } else if (Class::Ptr clazz = dynamic_cast<Class*>(f.pointer())) {
            if (features.find(clazz->name()) != features.end()) {
                err_ << f->location();
                err_ << "Duplicate definition of class '";
                err_ << clazz->name() << "' in " << module_name << "\n";
                env_->error();
            }
            features.insert(clazz->name());
        }
        f(this);
    }
}

void SemanticAnalyzer::operator()(Class* feature) {
    class_ = feature;
    slot_ = 0;
    enter_scope();

    // Make sure that a module with this name doesn't already exist.
    if (env_->module(feature->type()->qualified_name())) {
        err_ << feature->location();
        err_ << "Type name '" << feature->type()->qualified_name();
        err_ << "' conflicts with an existing module name\n"; 
        env_->error();
    }

    if (!feature->is_interface() && !feature->type()->is_primitive()) {
        gen_destructor();
        gen_constructor(); 
    }

    // Check interface/struct/object baseclass and make sure that 
    // disallowed things aren't included.
    for (Type::Ptr m = feature->mixins(); m; m = m->next()) {
        if (m->clazz()) {
            if (feature->is_interface() && !m->is_interface()) {
                err_ << m->location();
                err_ << "Mix-in for interface '" << feature->name();
                err_ << "' must be an interface\n";
                env_->error();
                break;
            } else if (feature->is_object() && m->is_interface()) {
                err_ << m->location();
                err_ << "Mix-in for object type '" << feature->name();
                err_ << "' cannot be an interface\n"; 
                env_->error();
                break;
            } else if (feature->is_value() && !m->is_value()) {
                err_ << m->location();
                err_ << "Mix-in for value type '" << feature->name();
                err_ << "' must be a value type\n"; 
                env_->error();
                break;
            }
         
        }
    }

    // Check for illegal mixins for the current class.  Certain object types
    // cannot be combined, as below:
    if (feature->is_object() && feature->is_interface()) {
        err_ << feature->location();
        err_ << "Class has both object and interface mixins";
        err_ << "\n";    
        env_->error();
    } else if (feature->is_interface() && feature->is_value()) {
        err_ << feature->location();
        err_ << "Class has both interfaces and value mixins";
        err_ << "\n";
        env_->error();
    } else if (feature->is_value() && feature->is_object()) {
        err_ << feature->location();
        err_ << "Class has both value and object mixins";
        err_ << "\n";
        env_->error();
    }

    // Iterate through all the features and make sure there are no duplicates
    // in the current scope.
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            attr(this);
        }
    }

    std::set<String::Ptr> features;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (features.find(func->name()) != features.end()) {
                err_ << f->location();
                err_ << "Duplicate definition of function '";
                err_ << func->name() << "'\n";
                env_->error();
            }
            features.insert(func->name());
            func(this);
        }
    }

    exit_scope();
    class_ = 0;
}

void SemanticAnalyzer::operator()(Formal* formal) {
    Type::Ptr type = formal->type();
    type(this);
}

void SemanticAnalyzer::operator()(NilLiteral* expression) {
    if (!expression->type()) {
        expression->type(env_->nil_type());
    }
}

void SemanticAnalyzer::operator()(StringLiteral* expression) {
    if (!expression->type()) {
        expression->type(env_->string_type()); 
    }
}

void SemanticAnalyzer::operator()(IntegerLiteral* expression) {
    if (!expression->type()) {
        expression->type(env_->int_type()); 
    }
}

void SemanticAnalyzer::operator()(FloatLiteral* expression) {
    if (!expression->type()) {
        expression->type(env_->float_type()); 
    } 
}

void SemanticAnalyzer::operator()(BooleanLiteral* expression) {
    expression->type(env_->bool_type());
}

void SemanticAnalyzer::operator()(Let* expression) {
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

void SemanticAnalyzer::operator()(Binary* expression) {
    // Checks primitive binary expressions.  These include basic logic
    // operations, which cannot be overloaded as methods.
    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    left(this);
    right(this);

    if (env_->name("or") == expression->operation()
        || env_->name("and") == expression->operation()) {

        if (!left->type()->is_boolifiable()) {
            err_ << left->location();
            err_ << "Value types cannot be converted to 'Bool'";
            err_ << "\n";
            env_->error();
        }
        if (!right->type()->is_boolifiable()) {
            err_ << right->location();
            err_ << "Value types cannot be converted to 'Bool'";
            err_ << "\n";
            env_->error();
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(env_->bool_type());
}

void SemanticAnalyzer::operator()(Unary* expression) {
    // Check primitive unary expressions.  These include basic logic 
    // operations, which cannot be overloaded as methods.
    Expression::Ptr child = expression->child();
    child(this);
    if (env_->name("not") == expression->operation()) {
        if (!child->type()->is_boolifiable()) {
            err_ << expression->location();
            err_ << "Value types cannot be converted to 'Bool'";
            err_ << "\n";
            env_->error();
        }
    } else {
        assert(!"Operator not implemented");
    }
    expression->type(env_->bool_type());
}

void SemanticAnalyzer::operator()(Call* expression) {
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
        err_ << expression->location();
        err_ << "Undeclared function '";
        err_ << expression->identifier() << "'";
        err_ << "\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }
    expression->type(func->type());
    
    // Check argument types versus formal parameter types.  Check the arity
    // of the function as well.
    Expression::Ptr arg = expression->arguments(); 
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        Type::Ptr at = arg->type();
        if (arg->type() == env_->self_type()) {
            at = class_->type(); 
        }
        if (!at->subtype(formal->type())) {
            err_ << arg->location();
            err_ << "Argument does not conform to type '";
            err_ << formal->type() << "'";
            err_ << "\n";
            env_->error();
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        err_ << expression->location();
        err_ << "Too many arguments to function ";
        err_ << expression->identifier();
        err_ << "\n";
        env_->error();
    }
    if (formal) {
        err_ << expression->location();
        err_ << "Not enough arguments to function ";
        err_ << expression->identifier();
        err_ << "\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Dispatch* expression) {

    // Evaluate types of argument expressions and the receiver
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Get the class associated with the receiver (always the first argument)
    Expression::Ptr receiver = expression->arguments();
    if (receiver->type()->is_no_type()) {
        expression->type(env_->no_type());
        return;
    }
    if (receiver->type()->is_self()) {
        receiver->type(class_->type());
    }
    Class::Ptr clazz = receiver->type()->clazz();
    if (!clazz) {
        err_ << expression->location();
        err_ << "Undefined class '" << receiver->type() << "'\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }
    
    // Look up the function using the class object.
    String::Ptr id = expression->identifier();
    Function::Ptr func = clazz->function(id); 
    if (!func) {
        err_ << expression->location();
        err_ << "Undeclared function '" << id << "' in class '";
        err_ << clazz->type() << "'\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    } 

    // Set the type of the expression to the return type of the function.
    // Look up the return type in the receiver if it is a generic.
    Type::Ptr rt = func->type();
    if (rt->is_generic()) {
        expression->type(receiver->type()->generic(rt->name()));
    } else {
        expression->type(rt);
    }

    // Check argument types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = func->formals();
    while (arg && formal) {
        // Get the formal type.  If the type is 'self', then the formal
        // parameter is the type of the receiver.  If the type is a generic,
        // then look up the actual type from the class' definition.
        Type::Ptr ft = formal->type();
        if (ft == env_->self_type()) {
            ft = receiver->type();
        }
        if (ft->is_generic()) {
            ft = receiver->type()->generic(ft->name());
        }

        // Get the actual type.  If the actual type is equal to 'self', then
        // get the type from the current class context.
        Type::Ptr at = arg->type();
        if (arg->type() == env_->self_type()) {
            at = class_->type(); 
        }
        if (!at->subtype(ft)) {
            err_ << arg->location();
            err_ << "Argument does not conform to type '" << ft << "'\n";
            env_->error();
        }
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        err_ << expression->location();
        err_ << "Too many arguments to function '" << id << "'\n";
        env_->error();
    }
    if (formal) {
        err_ << expression->location();
        err_ << "Not enough arguments to function '" << id << "'\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Construct* expression) {
    // Evaluate type of argument expression
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this); 
    }

    Type::Ptr type = expression->type();
    type(this);

    // Look up constructor class
    Class::Ptr clazz = type->clazz(); 
    if (!clazz) {
        expression->type(env_->no_type());
        return;
    }
    if (clazz->is_interface()) {
        err_ << expression->location();
        err_ << "Constructor called for interface type '";
        err_ << clazz->name() << "'\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }

    // Look up the constructor using the class object.
    Function::Ptr constr = clazz->function(env_->name("@init"));

    // Check arguments types versus formal parameter types
    Expression::Ptr arg = expression->arguments();
    Formal::Ptr formal = constr ? constr->formals() : 0;
    while (arg && formal) {
        // Get the formal type.  If the type is a generic, then look up the
        // actual type from the class' definition.
        Type::Ptr ft = formal->type();
        if (ft->is_generic()) {
            ft = type->generic(ft->name());
        }

        if (!arg->type()->subtype(ft)) {
            err_ << arg->location();
            err_ << "Argument does not conform to type '";
            err_ << formal->type() << "'\n";
            env_->error();
        }
        arg = arg->next();
        formal = formal->next();
    } 
    if (arg) {
        err_ << expression->location();
        err_ << "Too many arguments to constructor '";
        err_ << expression->type() << "'\n";
        env_->error();
    }
    if (formal) {
        err_ << expression->location();
        err_ << "Not enough arguments to constructor '";
        err_ << expression->type() << "'\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Identifier* expression) {
    // Determine the type of the identifier from the variable store.  If the
    // variable is undeclared, set the type of the expression to no-type to
    // prevent further error messages.
    Variable::Ptr var = variable(expression->identifier());
    if (!var) {
        err_ << expression->location();
        err_ << "Undeclared identifier '" << expression->identifier() << "'\n";
        env_->error();
        expression->type(env_->no_type());
    } else {
        expression->type(var->type());
    }
}

void SemanticAnalyzer::operator()(Empty* expression) {
}

void SemanticAnalyzer::operator()(Block* statement) {
    // Enter a new scope, then check each of the children.  Perform
    // reachability analysis to make sure that the first 'return' statement is
    // always the last statement in the block.
    enter_scope();
    return_ = 0;
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        if (return_) {
            err_ << s->location();
            err_ << "Statement is unreachable\n";
            env_->error();
            break;
        } else {
            s(this);
        }
    }
    exit_scope();
}

void SemanticAnalyzer::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
}

void SemanticAnalyzer::operator()(While* statement) {
    // Check the while statement guard type, and then check all the branches.
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();

    guard(this);
    Type* t = guard->type();
    assert(t);
    if (!env_->bool_type()->equals(guard->type())) {
        err_ << statement->location();
        err_ << "While statement guard expression must have type '";
        err_ << env_->bool_type() << "'";
        err_ << "\n";
        env_->error();
    }
    block(this);
}

void SemanticAnalyzer::operator()(Conditional* statement) {
    // Ensure that the guard is convertible to type bool.  This is always true
    // unless the guard is a value type.
    Expression::Ptr guard = statement->guard();
    Statement::Ptr true_branch = statement->true_branch();
    Statement::Ptr false_branch = statement->false_branch();
    guard(this);
    if (!guard->type()->is_boolifiable()) {
        err_ << guard->location();
        err_ << "Value types cannot be converted to 'Bool'";
        err_ << "\n";
        env_->error();
    }
    true_branch(this);
    if (false_branch) {
        false_branch(this);
    } else {
        return_ = 0;
    }
}

void SemanticAnalyzer::operator()(Assignment* statement) {
    // Ensure that a variable is not duplicated, or re-initialized with a 
    // different type.  This function handles both assignment and variable
    // initialization.
    String::Ptr id = statement->identifier();
    Expression::Ptr init = statement->initializer();
    init(this);

    if (statement->type()) {
        Type::Ptr type = statement->type();
        type(this);
        if (!type->clazz()) {
            variable(new Variable(id, 0, env_->no_type()));
            return;
        }
    }

    Variable::Ptr var = variable(id);
    Type::Ptr type = var ? var->type() : 0;

    // Attempt to assign to an immutable var, usually a param
    if (var && var->is_immutable()) {
        err_ << statement->location();
        err_ << "Value assigned to formal parameter '" << id << "'\n"; 
        env_->error();
        return;
    }
        
    // Attempt to assign void to a variable during initialization.
    if (init->type() && init->type()->equals(env_->void_type())) {
        err_ << init->location();
        err_ << "Void value assigned to variable '" << id << "'\n";
        env_->error();
        variable(new Variable(id, 0, env_->no_type()));
        return;
    }
    
    if (statement->type()) {
        // The variable was declared with an explicit type, but the variable
        // already exists.
        if (type) {
            err_ << statement->location();
            err_ << "Duplicate definition of variable '" << id << "'\n";
            env_->error();
            return;
        }

        // The variable was declared with an explicit type, and the init
        // does not conform to that type.
        if (init->type() && !init->type()->subtype(statement->type())) {
            err_ << init->location();
            err_ << "Expression does not conform to type '";
            err_ << statement->type() << "'\n";
            env_->error();
            return;
        }

        variable(new Variable(id, 0, statement->type()));
    } else {
        // The variable was already declared, but the init type is not
        // compatible with the variable type.
        if (type && !init->type()->subtype(type)) {
            err_ << init->location();
            err_ << "Expression does not conform to type '";
            err_ << type << "' in assignment of '" << id << "'\n";
            env_->error();
            return;
        }
        variable(new Variable(id, 0, init->type())); 
        statement->type(init->type());
    }
}

void SemanticAnalyzer::operator()(Return* statement) {
    Expression::Ptr expression = statement->expression();
    if (expression) {
        expression(this);
        return_ = expression->type();
    }
    if (!expression->type()) {
        expression->type(env_->void_type());
        return;
    }
    if (expression->type()->is_self()) {
        expression->type(class_->type());
    }
    if (!expression->type()->subtype(function_->type())) {
        err_ << statement->location();
        err_ << "Return must conform to type '";
        err_ << function_->type() << "'";
        err_ << "\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(When* statement) {
    Expression::Ptr guard = statement->guard();
    Statement::Ptr block = statement->block();
    guard(this);
    block(this);
}

void SemanticAnalyzer::operator()(Fork* statement) {
    assert("Not supported");
}

void SemanticAnalyzer::operator()(Yield* statement) {
    assert("Not supported");
}

void SemanticAnalyzer::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);

    for (Statement::Ptr b = statement->branches(); b; b = b->next()) {
        b(this);
        When::Ptr when = static_cast<When*>(b.pointer());
        if (!guard->type()->equals(when->guard()->type())) {
            err_ << statement->location();
            err_ << "Case expression does not conform to type '";
            err_ << guard->type() << "'";
            err_ << "\n";
            env_->error();
        }
    }
}

void SemanticAnalyzer::operator()(Function* feature) {
    // Begin type checking a function.  The formal parameters are type-checked
    // first, followed by the function body.  If the function belongs to an
    // an interface, the function should not have a body.
    Block::Ptr block = feature->block();
    function_ = feature;
    enter_scope();

    // Calculate the label name for this function.
    std::string name = module_->name()->string();
    if (class_) {
        if (!name.empty()) {
            name += "_";
        }
        name += class_->name()->string();
    }
    if (!name.empty()) {
        name += "_";
    }
    name += feature->name()->string();

    std::string label;
    label.reserve(name.length());
    for (int i = 0; i < name.length(); i++) {
        if (name[i] == '@') {
            label += '_';
        } else if (name[i] == '?') {
            label += "__g";
        } else if (name[i] == '!') {
            label += "__w";
        } else if (name[i] == '=') {
            label += "__s";
        } else if (name[i] == ':') {
            label += "_";
            i++;
        } else {
            label += name[i];
        }
    }

    feature->label(env_->name(label));

    // Check all formal parameters, and add them as local variables
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        Type::Ptr type = f->type();
        type(this);
        variable(new Variable(f->name(), 0, f->type(), true));
    }
    if (feature->is_constructor()) {
        variable(new Variable(env_->name("self"), 0, env_->self_type(), true));
    }

    Type::Ptr type = feature->type();
    type(this);

    bool is_interface = class_ && class_->is_interface();
    bool is_native = feature->is_native();

    // Check the different combinations of prototype/body/no-body, method, and
    // function.  Prototypes (as in interface definitions) should not have a
    // body.
    if (block && is_interface) {
        err_ << block->location();
        err_ << "Interface function '" << feature->name();
        err_ << "' has a body\n";
        env_->error();
    } else if (block && block->children() && is_native) {
        err_ << feature->location();
        err_ << "Native function '" << feature->name();
        err_ << "' has a body\n";
        env_->error();
    } else if (block && !is_native) {
        block(this); 
        if (type != env_->void_type() && !return_) {
            err_ << feature->location();
            err_ << "Function '" << feature->name();
            err_ << "' must return a value\n";     
            env_->error();
        }
    } else if (!block && !is_interface && !is_native) {
        err_ << feature->location();
        err_ << "Function '" << feature->name() << "' has no body\n";
        env_->error();
    }
        
    exit_scope();
}

void SemanticAnalyzer::operator()(Attribute* feature) {
    // Select a slot for this attribute.
    feature->slot(slot_++);

    if (variable(feature->name())) {
        err_ << feature->location();
        err_ << "Duplicate definition of attribute '";
        err_ << feature->name() << "'\n";
        env_->error();
    }

    // Make sure that the attribute type conforms to the declared type of
    // the attribute, if there is one.
    Expression::Ptr initializer = feature->initializer();
    initializer(this);


    // No explicitly declared type, but the initializer can be used to infer
    // the type.
    if (!feature->type()) {
        variable(new Variable(feature->name(), 0, initializer->type()));     
        feature->type(initializer->type());
        gen_mutator(feature);
        gen_accessor(feature);
        return;
    }

    // Check that the feature type exists.
    Type::Ptr type = feature->type();
    type(this);

    // Make sure that the initializer and the feature type conform.
    if (initializer->type() && !initializer->type()->subtype(feature->type())) {
        err_ << feature->location();
        err_ << "Expression does not conform to type '";
        err_ << initializer->type() << "'";
        err_ << "\n";
        env_->error();
    }

    variable(new Variable(feature->name(), 0, feature->type()));
    gen_mutator(feature);
    gen_accessor(feature);
}

void SemanticAnalyzer::operator()(Import* feature) {
}

void SemanticAnalyzer::operator()(Type* type) {
    // Check to make sure that the type has a defined class.  Also, check
    // all generics that are part of the type to make sure that they resolve.
    if (type->is_self() || type->is_no_type() || type->is_void()) {
        return;
    }

    if (type->is_generic() && class_) {
        Type::Ptr ct = class_->type();
        for (Generic::Ptr gen = ct->generics(); gen; gen = gen->next()) {
            if (gen->type()->name() == type->name()) {
                return;
            } 
        }
        err_ << type->location();
        err_ << "Undeclared generic type '" << type->name() << "'\n";
        env_->error();
        type->is_no_type(true);
        return;
    }

    if (!type->clazz()) {
        err_ << type->location();
        err_ << "Undefined type '" << type << "'\n";
        env_->error();
        type->is_no_type(true);
        return;
    }

    // Check generics for type resolution
    for (Generic::Ptr g = type->generics(); g; g = g->next()) {
        if (!g->type()->clazz() && !type->equals(env_->self_type())) {
            err_ << type->location();
            err_ << "Undefined type '" << type << "'\n";
            env_->error();
        }
        if (g->type()->is_value()) {
            err_ << type->location();
            err_ << "Primitives and value types cannot be used in generics\n";
            env_->error();
        }
    }
}

Variable* SemanticAnalyzer::variable(String* name) {
    // Look up the variable, starting in the current scope and continuing up
    // the scope stack.
    vector<Scope::Ptr>::reverse_iterator i;
    for (i = scope_.rbegin(); i != scope_.rend(); i++) {
        Variable* var = (*i)->variable(name);
        if (var) {
            return var;
        }
    }
    return 0;
}

void SemanticAnalyzer::variable(Variable* var) {
    assert(scope_.size());
    scope_.back()->variable(var);
}

void SemanticAnalyzer::enter_scope() {
    scope_.push_back(new Scope);
}

void SemanticAnalyzer::exit_scope() {
    scope_.pop_back();
}

void SemanticAnalyzer::gen_constructor() {
    // Check for a constructor, and generate one if it isn't present
    if (!class_->function(env_->name("@init"))) {
        String::Ptr nm = env_->name("@init");
        Type::Ptr vt = env_->void_type();
        Location loc;
        Block::Ptr block(new Block(loc, 0, 0));
        class_->feature(new Function(loc, nm, 0, 0, vt, block));
    }
}

void SemanticAnalyzer::gen_destructor() {
    // Check for a destructor, and generate one if it isn't present
    if (!class_->function(env_->name("@destroy"))) {
        String::Ptr nm = env_->name("@destroy");
        Type::Ptr st = env_->self_type();
        Type::Ptr vt = env_->void_type();
        Location loc;
        Block::Ptr block(new Block(loc, 0, 0));
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, nm, self, 0, vt, block));
    }
}

void SemanticAnalyzer::gen_accessor(Attribute* feature) {
    // Insert a setter for the attribute if it doesn't already exist
    String::Ptr get = env_->name(feature->name()->string()+"?");
    if (!class_->function(get) && !feature->is_private()) {
        Location loc;
        Identifier::Ptr attr(new Identifier(loc, feature->name()));
        Return::Ptr ret(new Return(loc, attr)); 
        Block::Ptr block(new Block(loc, 0, ret));
        Type::Ptr st = env_->self_type();
        Type::Ptr ft = feature->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, get, self, 0, ft, block));
    }
}

void SemanticAnalyzer::gen_mutator(Attribute* feature) {
    // Insert a getter for the attribute if it doesn't already exist
    String::Ptr set = env_->name(feature->name()->string()+"=");
    if (!class_->function(set) && !feature->is_immutable()
            && !feature->is_private()) {
        Location loc;
        Identifier::Ptr val(new Identifier(loc, env_->name("_arg0"))); 
        Assignment::Ptr assign(new Assignment(loc, feature->name(), 0, val)); 
        Block::Ptr block(new Block(loc, 0, assign));
        Type::Ptr st = env_->self_type();
        Type::Ptr vt = env_->void_type();
        Type::Ptr ft = feature->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        Formal::Ptr arg(new Formal(loc, env_->name("_arg0"), ft));
        self->next(arg);
        class_->feature(new Function(loc, set, self, 0, vt, block));
    }
}
