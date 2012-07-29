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
#include "ClosureAnalyzer.hpp"
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
        if (Class::Ptr clazz = dynamic_cast<Class*>(f.pointer())) {
            if (clazz->is_closure()) { continue; }
        }
        f(this);
    }
}

void SemanticAnalyzer::operator()(Class* feature) {
    // Analyze a class, including its child attributes and functions.
    ContextAnchor anchor(this);
    class_ = feature;

    if (feature->file()) {
        feature->file()->dependency(feature);
    }

    // Make sure that there isn't a duplicate of this class.
    Feature::Ptr parent = feature->parent();
    if (parent->clazz(feature->name()) != feature) {
        err_ << feature->location();
        err_ << "Duplicate definition of class '";
        err_ << feature->name() << "'\n";
        env_->error();
    }

    // Make sure that a module with this name doesn't already exist.
    if (env_->module(feature->qualified_name())) {
        err_ << feature->location();
        err_ << "Type name '" << feature->qualified_name();
        err_ << "' conflicts with an existing module name\n"; 
        env_->error();
    }

    if (!feature->is_interface() && !feature->is_mixin()) {
        destructor();
        if (!feature->is_primitive()) {
            constructor(); 
        }
    }

    // Check alternatives to make sure there are no interface types.
    for (Type::Ptr alt = feature->alternates(); alt; alt = alt->next()) {
        if (alt->clazz() && alt->is_interface()) {
            err_ << alt->location();
            err_ << "Interface cannot be used as an alternate\n";
            env_->error(); 
        }
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
        } else if(!m->is_proto()) {
            err_ << m->location();
            err_ << "Undefined type '" << m << "'\n";
            env_->error();
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

    assign_enums(feature);

    // Iterate through all the features and generate accessors.  Also, select
    // a numeric value for each enum constant within the class
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            attr->slot(class_->next_slot());
        }
        f(this);
    }
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

void SemanticAnalyzer::operator()(HashLiteral* literal) {
}

void SemanticAnalyzer::operator()(ArrayLiteral* literal) {
/*
    Type::Ptr parent_type = literal->parent_type();
    Type::Ptr type;
    for (Expression::Ptr a = literal->arguments(); a; a = a->next()) {
        a(this);
        if (parent_type) {
            if (!a->type()->subtype(parent_type)) {
                err_ << a->location();
                err_ << "List element does not conform to type '";
                err_ << parent_type << "'\n";
                env_->error();
            }
        } else if (type) {
            if (!a->type()->subtype(type)) {
                err_ << a->location();
                err_ << "List element does not conform to type '";
                err_ << type << "'\n";
                env_->error();
            }
        } else {
            type = a->type();
        }
    }
*/
}

void SemanticAnalyzer::operator()(Let* expression) {
    // For a let expression, introduce the new variables in a new scope, and 
    // then check the block.
    enter_scope();
    Statement::Ptr block = expression->block();
    for (Expression::Ptr v = expression->variables(); v; v = v->next()) {
        v(this);
    }
    block(this);
    exit_scope();
}

void SemanticAnalyzer::operator()(Is* expr) {
    // Check that the 'is' type is always a value or object type without type
    // parameters.
    Expression::Ptr child = expr->child();
    child(this);
    expr->type(env_->bool_type());

    if (expr->check_type()->is_interface()) {
        err_ << expr->check_type()->location();
        err_ << "Type in 'is' expression cannot be an interface\n";
        env_->error();
    }
    if (expr->check_type()->is_alt_type()) {
        err_ << expr->check_type()->location();
        err_ << "Type in 'is' expression cannot be a union\n";
        env_->error();
    }
    if (expr->check_type()->generics()) {
        err_ << expr->check_type()->location();
        err_ << "Type in 'is' expression cannot be parameterized\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Box* expression) {
    // Box expression
    Expression::Ptr child = expression->child();
    child(this);

    if (child->type() != expression->type()) {
        err_ << expression->location();
        err_ << "Box type does not match expression type\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Cast* expression) {
    // Cast expression.  Converts an item of the type specified by 'expr to
    // an item of the type specified by the cast type.
    Expression::Ptr child = expression->child();
    child(this);

    Type::Ptr type = expression->type();
    type(this);
    
    if (type->is_value()) {
        err_ << type->location();
        err_ << "Illegal cast to value type\n";
        env_->error();
    } else if (type->is_interface()) {
        err_ << type->location();
        err_ << "Illegal cast interface type\n";
    }
}

void SemanticAnalyzer::operator()(Binary* expression) {
    // Checks primitive binary expressions.  These include basic logic
    // operations, which cannot be overloaded as methods.
    expression->type(env_->bool_type());

    Expression::Ptr left = expression->left();
    Expression::Ptr right = expression->right();
    left(this);
    right(this);

    if (env_->name("or") == expression->operation()
        || env_->name("and") == expression->operation()) {

        if (!left->type()->is_boolifiable()) {
            err_ << left->location();
            err_ << "Value types cannot be converted to 'Bool'\n";
            env_->error();
        }
        if (!right->type()->is_boolifiable()) {
            err_ << right->location();
            err_ << "Value types cannot be converted to 'Bool'\n";
            env_->error();
        }
    } else {
        assert(!"Operator not implemented");
    }
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

void SemanticAnalyzer::operator()(Member* expression) {
    // Determine whether this is just a member access, which will be translated
    // into a function call, or if it is a below a call node.  If the
    // expression is below a call node, then the call node is a member function
    // dispatch.
    String::Ptr id = expression->identifier();
    Expression::Ptr expr = expression->expression(); 
    Call::Ptr call = dynamic_cast<Call*>(expression->parent());

    // Recursively check the LHS of the '.' operator
    expr(this);

    Type::Ptr type = expr->type();
    if (type->is_no_type()) {
        expression->type(env_->no_type());
        return;
    }

    Class::Ptr clazz = type->is_self() ? class_.pointer() : type->clazz();
    if (!clazz) {
        expression->type(env_->no_type());
        return;
    }
    
    if (call) {
        // First lookup: check to see if the member with name 'id' is present
        // in the class corresponding to the type of the LHS of the '.'
        // operator. 
        call->function(clazz->function(id)); 
        if (!call->function()) {
            err_ << call->location();
            err_ << "Function '" << id << "' not found in class '";
            err_ << clazz->name() << "'\n";
            env_->error();
        } else {
            call->receiver(expr); 
        } 

        // Second lookup: check to see if there is an accessor that returns
        // an object that is callable. FIXME 
        expression->type(env_->no_type());
        return;
    }

    // Try to look up the attribute.  Then take the type from the attribute
    // and use it, if the attribute is public.
    Attribute::Ptr attr = clazz->attribute(id);
    if (attr && attr->type() == env_->bottom_type()) {
        err_ << expression->location(); 
        err_ << "Illegal circular reference\n";
        env_-> error();
        expression->type(env_->no_type());
        return;
    }
    if (attr) {
        attr(this);
    }
    Function::Ptr func = clazz->function(env_->name(id->string()+"?")); 

    if (!func) {
        err_ << expression->location();
        err_ << "Attribute '" << id << "' not found in class '";
        err_ << clazz->name() << "'\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }
    
    assert(func->type() && "Attribute has no type");
    expression->type(fix_generics(expr->type(), func->type()));
    expression->function(func);
	expression->file()->dependency(func);
	if (function_) {
		function_->called_func(func);
	}

    if (func->is_private()) {
        err_ << expression->location();
        err_ << "Function '" << id << "' is private in class '";
        err_ << clazz->name() << "'\n";
        env_->error();
    }
    assert(expression->type());
}

void SemanticAnalyzer::operator()(Call* expression) {
    // Look up the function by name in the current context.  The function may
    // be a member of the current module, or of a module that was imported in
    // the current compilation unit.
    if (expression->type()) { return; }

    // Evaluate types of argument expressions, then perform type checking
    // on the body of the function.
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        a(this);
    }

    // Check the expression that is being called.  Function resolution happens
    // here, because it depends on the type of the child node.
    Expression::Ptr expr = expression->expression();
    expr(this);

    // A function value should have been assigned by the child.
    Function::Ptr func = expression->function();
    if (!func) {
        expression->type(env_->no_type());
        return;
    }
        
    expression->type(func->type());

    // Check to make sure the resolved function is not private
	if (func->is_private() && !expression->receiver()->type()->is_self()) {
        err_ << expression->location();
        err_ << "Function '" << func->name() << "' is private in class '";
        err_ << expression->receiver()->type() << "'\n";
        env_->error();  
    }
    
    // Figure out what the receiver type is.  If the type is 'self' then set
    // the type to the actual containing class.
    Type::Ptr receiver;
    if (expression->receiver()) {
        expression->receiver()->next(expression->arguments());
        expression->arguments(expression->receiver());
        if (expression->receiver()->type()->is_self()) {
            receiver = class_->type();
        } else { 
            receiver = expression->receiver()->type();
        }
        expression->type(fix_generics(receiver, func->type()));
    } else {
        expression->type(func->type());
    }

    // FIXME: Look up generics for function
    expression->file()->dependency(func);
    expression->arguments(args(expression->arguments(), func, receiver));
	if (function_) {
		function_->called_func(func);
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
    if (constr && constr->is_private()) {
        err_ << expression->location();
        err_ << "Constructor is private in class '" << type << "'\n";
        env_->error();  
    }
    expression->file()->dependency(constr);
    
    if (clazz->is_enum()) {
        err_ << expression->location();
        err_ << "Enums do not have constructors\n";
        env_->error();
    } else if (!constr) {
        err_ << expression->location();
        err_ << "Class '" << type << "' has no constructor\n";
        env_->error();
    } else {
        expression->arguments(args(expression->arguments(), constr, type));  
    }
	if (function_) {
		function_->called_func(constr);
	}
}

void SemanticAnalyzer::operator()(ConstantIdentifier* expression) {
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope();
    
    File::Ptr file = expression->file();
    Constant::Ptr constant;
    if (class_ && scope->is_empty()) {
        constant = class_->constant(id);
    } else {
        constant = file->constant(scope, id);
    }
    if (!constant) {
        err_ << expression->location();
        err_ << "Undefined constant '";
        if (scope && !scope->string().empty()) {
             err_ << scope << "::";
        }
        err_ << id << "'\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }
    if (constant->type() == env_->bottom_type()) {
        err_ << expression->location();
        err_ << "Illegal circular reference\n";
        env_->error();
        expression->type(env_->no_type());
        return;
    }
    constant(this);
    file->dependency(constant);
    
    expression->constant(constant);
    expression->type(constant->type()); 
    assert(expression->type());
}

void SemanticAnalyzer::operator()(Identifier* expression) {
    // Determine the type of the identifier from the variable store.  If the
    // variable is undeclared, set the type of the expression to no-type to
    // prevent further error messages.
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope(); 
    Call::Ptr call = dynamic_cast<Call*>(expression->parent());
    Type::Ptr type; // Class of the object stored in 'expression'

    if (Variable::Ptr var = variable(id)) {
        type = var->type();
    } else if (class_ && scope->is_empty()) {
        Attribute::Ptr attr = class_->attribute(id);
        if (attr) {
            if (attr->type() == env_->bottom_type()) {
                err_ << expression->location();
                err_ << "Illegal circular reference\n";
                env_->error();      
                expression->type(env_->no_type());
                return;
            }
            attr(this);
            type = attr->type();
        }
    }

    if (!call) {
        // First handle the simple case, which is that this is just a plain
        // local variable or attribute access.
        if (!type) {
            err_ << expression->location();
            err_ << "Undefined variable '" << id << "'\n";
            env_->error();
            expression->type(env_->no_type());
        } else {
            expression->type(type);
        }
        return;
    }

    // Handle the case where a variable is called using the @call operator. 
    if (type) {
        Class* clazz = type->clazz();
        if(clazz) {
            call->function(clazz->function(env_->name("@call")));
        }
        if (!call->function()) {
            err_ << expression->location();
            err_ << "Object is not callable\n";
            env_->error();
        } else {
            call->receiver(expression);
            expression->type(type);
        }
        return;
    }

    // This case handles a function that is called without the 'self'
    // receiver, but is a function of the enclosing class.  In this case,
    // the 'self' receiver is inserted automatically.
    if (class_ && scope->is_empty()) {
        call->function(class_->function(id));
        if (call->function()) {
            Location loc = expression->location();
            String::Ptr scope = env_->name("");
            String::Ptr name = env_->name("self");
            Identifier::Ptr self = new Identifier(loc, scope, name);
            self->type(env_->self_type());
            call->receiver(self);
            return;
        }
    }

    // Parent is a function call; we need to try to evaluate this identifier as
    // a function.  In the first case, the identifier actually resolves
    // directly to a function, so use it.
    call->function(expression->file()->function(scope, id));

    // If all attempts to resolve the function fail, then it is missing.
    if (!call->function()) {
        err_ << call->location();
        err_ << "Undeclared function '" << id << "'\n";
        env_->error();
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

    bool return_checked = false;
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        if (return_ && ! return_checked) {
            err_ << s->location();
            err_ << "Statement is unreachable\n";
            env_->error();
            return_checked = true;
        }
        s(this);
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
    return_ = 0;
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
        err_ << "Value types cannot be converted to 'Bool'\n";
        env_->error();
    }
    true_branch(this);
    if (false_branch) {
        false_branch(this);
    } else {
        return_ = 0;
    }
}

void SemanticAnalyzer::operator()(Assignment* expr) {
    // Ensure that a variable is not duplicated, or re-initialized with a 
    // different type.  This function handles both assignment and variable
    // initialization.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Type::Ptr declared = expr->declared_type();
    Type::Ptr type = (var && declared != env_->no_type()) ? var->type() : 0;
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Expression::Ptr init = expr->initializer();

    init(this);
    if (init->type()) {
        expr->type(init->type());
    } else {
        expr->type(expr->declared_type());
    }

    if (declared && declared != env_->no_type()) {
        // If declared_type == no_type, then the variable declaration has no
        // true declared type, but it defines a new variable regardless of
        // whether or not the variable is already shadowed by another variable.
        declared(this);
        // Check to make sure the declared type is valid; if it isn't, then
        // set the variable to no_type and return.
        if (!declared->clazz()) {
            variable(new Variable(id, Operand(), env_->no_type()));
            return;
        }
    }

    // Attempt to assign to an immutable var, usually a param
    if (var && var->is_immutable()) {
        err_ << expr->location();
        err_ << "Value assigned to formal parameter '" << id << "'\n"; 
        env_->error();
        return;
    }
        
    // Attempt to assign void to a variable during initialization.
    if (init->type() && init->type()->equals(env_->void_type())) {
        err_ << init->location();
        err_ << "Void value assigned to variable '" << id << "'\n";
        env_->error();
        variable(new Variable(id, Operand(), env_->no_type()));
        return;
    }
    
    if (declared && declared != env_->no_type()) {
        // If declared_type == no_type, then the variable declaration has no
        // true declared type, but it defines a new variable regardless of
        // whether or not the variable is already shadowed by another variable.
        initial_assignment(expr);
    } else {
        secondary_assignment(expr);
    }
}

void SemanticAnalyzer::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    if (expr) {
        expr(this);
        return_ = expr->type();
    }
    if (!expr->type()) {
        expr->type(env_->void_type());
        return;
    }
    if (expr->type()->is_self()) {
        expr->type(class_->type());
    }
    if (!expr->type()->subtype(function_->type())) {
        err_ << statement->location();
        err_ << "Return must conform to type '";
        err_ << function_->type() << "'";
        err_ << "\n";
        env_->error();
    }

    Type::Ptr ft = function_->type();
    Type::Ptr type = expr->type();
    if (type->is_value() && ft->is_alt_type()) {
        statement->expression(new Box(expr->location(), type, expr)); 
    }
}

void SemanticAnalyzer::operator()(Fork* statement) {
    assert(!"Not supported");
}

void SemanticAnalyzer::operator()(Yield* statement) {
    // Nothing to check
    assert("Not supported" && !statement->expression());

	// A yield statement can throw an exception; therefore, the enclosing 
	// function can also throw an exception.
	function_->throw_spec(Function::THROW);
}

void SemanticAnalyzer::operator()(Case* statement) {
    Expression::Ptr guard = statement->guard();
    guard(this);
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
}

void SemanticAnalyzer::operator()(Match* statement) {
	// Check a match statement.  First by check the guard, and then by ensure 
	// that each case expression has the same type as the guard.
    Expression::Ptr guard = statement->guard();
    guard(this);

    for (Statement::Ptr b = statement->cases(); b; b = b->next()) {
        b(this);
        Case::Ptr with = static_cast<Case*>(b.pointer());
        if (!guard->type()->equals(with->guard()->type())) {
            err_ << statement->location();
            err_ << "Match expression does not conform to type '";
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
    Feature::Ptr parent = feature->parent();
    function_ = feature;
    enter_scope();

    // Check for duplicate functions with the same name
    if (parent->feature(feature->name()) != feature) {
        err_ << feature->location();
        err_ << "Duplicate definition of function '";
        err_ << feature->name() << "'\n";
        env_->error();
    }

    // Check all formal parameters, and add them as local variables
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        Type::Ptr type = f->type();
        type(this);
        variable(new Variable(f->name(), Operand(), f->type(), true));
    }
    if (feature->is_constructor()) {
        String::Ptr nm = env_->name("self");
        variable(new Variable(nm, Operand(), env_->self_type(), true));
    }

    Type::Ptr type = feature->type();
    type(this);

    File::Ptr file = feature->location().file;
    bool is_lib_def = file && file->is_interface_file();
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
    } else if (!block && !is_interface && !is_native && !is_lib_def) {
        err_ << feature->location();
        err_ << "Function '" << feature->name() << "' has no body\n";
        env_->error();
    }
        
    exit_scope();
    function_ = 0;
}

void SemanticAnalyzer::operator()(Constant* feature) {
    // Analyze a constant value.
    Expression::Ptr initializer = feature->initializer();
    Feature::Ptr parent = feature->parent();
    if (initializer && feature->type() && initializer->type()) { return; }

    if (parent->feature(feature->name()) != feature) {
        err_ << feature->location();
        err_ << "Duplicate definition of constant '";
        err_ << feature->name() << "'\n";
        env_->error();
    }

    Class::Ptr clazz = dynamic_cast<Class*>(parent.pointer());
    feature->type(env_->bottom_type());
    if(initializer) {
        initializer(this);
        feature->type(initializer->type());
    } else if(clazz->is_enum()) {
        feature->type(clazz->type());
    }
    env_->constant(feature);
}

void SemanticAnalyzer::operator()(Attribute* feature) {
    Expression::Ptr initializer = feature->initializer();
    Feature::Ptr parent = feature->parent();
    if (feature->type() && (!initializer || initializer->type())) { return; }

    // Save the current class and variable scope.
    ContextAnchor context(this);
    class_ = dynamic_cast<Class*>(feature->parent());

    // Check for duplicate attributes
    if (parent->feature(feature->name()) != feature) {
        err_ << feature->location();
        err_ << "Duplicate definition of attribute '";
        err_ << feature->name() << "'\n";
        env_->error();
    } 

    // Make sure that the attribute type conforms to the declared type of
    // the attribute, if there is one.  Set the type to 'bottom' while 
    // checking the initializer, in case there are circular references.
    Type::Ptr save = feature->type();
    feature->type(env_->bottom_type());
    initializer(this);
    feature->type(save);

    // No explicitly declared type, but the initializer can be used to infer
    // the type.
    if (!feature->type()) {
        feature->type(initializer->type());
        mutator(feature);
        accessor(feature);
        if (feature->file()) {
            feature->file()->dependency(feature->type()->clazz());
        }
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

    mutator(feature);
    accessor(feature);
}

void SemanticAnalyzer::operator()(Closure* expression) {
    Type::Ptr type = expression->clazz()->type();
    expression->type(type);

    // Analyze the closure to find unbound variables
    ClosureAnalyzer::Ptr analyzer(new ClosureAnalyzer(env_)); 
    Function::Ptr func = expression->function();
    func(analyzer.pointer());

    // Now build the arg/formal list for the call to the constructor for the
    // closure.  Basically, this code is turns a closure into a call to a 
    // closure object.
    Location loc = expression->location();
    Expression::Ptr args;
    Formal::Ptr formals;
    Statement::Ptr stmts;
    Feature::Flags flags = Feature::PRIVATE; 
    Expression::Ptr empty = new Empty(loc);

    for (int i = 0; i < analyzer->unbound_vars(); i++) {
        String::Ptr id = analyzer->unbound_var(i);
        Variable::Ptr var = variable(id);
        if (!var) { continue; }

        // Add a new formal to the constructor; also add a statement to assign
        // the formal to the attribute stored in the closure object.
        Expression::Ptr arg = new Identifier(loc, env_->name(""), id); 
        String::Ptr fid = env_->name("_"+id->string());
        Formal::Ptr formal = new Formal(loc, fid, var->type()); 
        Identifier::Ptr rhs = new Identifier(loc, env_->name(""), fid);
        Assignment::Ptr as = new Assignment(loc, id, 0, rhs); 
        Statement::Ptr stmt = new Simple(loc, as);
        if (args) {
            formal->next(formals);
            arg->next(args);
            stmt->next(stmts);
        }
        formals = formal;
        args = arg; 
        stmts = stmt;
        
        // Create a new attribute inside the closure object to store the 
        // variable.
        Type::Ptr type = var->type();
        Attribute::Ptr attr = new Attribute(loc, env_, id, flags, type, empty);
        expression->clazz()->feature(attr); 
    }

    Construct::Ptr constr = new Construct(loc, type, args);
    expression->construct(constr);
    
    Type::Ptr ret = env_->void_type();
    Block::Ptr block = new Block(loc, 0, stmts);
    String::Ptr name = env_->name("@init");
    Function::Ptr ctor = new Function(loc, env_, name, formals, 0, ret, block);
    expression->clazz()->feature(ctor);

    Class::Ptr clazz = expression->clazz();
    clazz(this);
    constr(this);
}

void SemanticAnalyzer::operator()(Import* feature) {
}

void SemanticAnalyzer::operator()(Type* type) {
    // Check to make sure that the type has a defined class.  Also, check
    // all generics that are part of the type to make sure that they resolve.
    if (type->is_self() || type->is_no_type() || type->is_void()) {
        return;
    }
    if (type->file()) {
        type->file()->dependency(type->clazz());
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
        Type::Ptr gen = g->type();
        gen(this);
        if (g->type()->is_value()) {
            err_ << type->location();
            err_ << "Primitives and value types cannot be used in generics\n";
            env_->error();
        }
    }
    Generic::Ptr arg = type->generics();
    Generic::Ptr formal = type->clazz()->type()->generics();
    while (arg && formal) {
        arg = arg->next();
        formal = formal->next();
    }
    if (arg) {
        err_ << type->location();
        err_ << "Too many arguments for type '" << type->name() << "'\n";
        env_->error();
        type->is_no_type(true);
    }
    if (formal) {
        err_ << type->location();
        err_ << "Not enough arguments for type '" << type->name() << "'\n";
        env_->error();
        type->is_no_type(true);
    }
}

Expression::Ptr SemanticAnalyzer::args(Expression* args, Function* fn, Type* rec) {
    // Check argument types versus formal parameter types
    String::Ptr id = fn->name();
    Formal::Ptr formal = fn ? fn->formals() : 0;
    Expression::Ptr arg = args;
    Expression::Ptr out;
    if (arg) {
        arg->last(0);
    }

    while (arg && formal) {
        // Get the formal type.  If the type is 'self', then the formal
        // parameter is the type of the receiver.  If the type is a generic,
        // then look up the actual type from the class' definition.
        Type::Ptr ft = formal->type();
        if (ft->is_self()) {
            ft = rec->is_self() ? class_->type() : rec;
        }
        if (ft->is_generic()) {
            ft = rec->generic(ft->name());
        }

        // Get the actual type.  If the actual type is equal to 'self', then
        // get the type from the current class context.
        Type::Ptr at = arg->type();
        if (at->is_self()) {
            at = class_->type(); 
        }
    
        // Build the modified argument list (which may include cast 
        // expressions that were auto-inserted by the compiler).
        if (!at->subtype(ft)) {
            err_ << arg->location();
            err_ << "Argument does not conform to type '" << ft << "'\n";
            env_->error();
        }

        // If the formal type is not 'Any' but the argument is of type 'Any',
        // the automatically insert a cast.  Cast also unboxes primitives and
        // value types if necessary.
        Expression::Ptr actual;
        if (at->is_alt_type() && !ft->equals(at)) {
            actual = new Cast(arg->location(), ft, arg);
        } else if (at->is_value() && ft->is_alt_type()) {
            actual = new Box(arg->location(), at, arg); 
        } else {
            actual = arg;
        }
        out = append(out, actual.pointer());
        Expression::Ptr prev = arg;
        arg = arg->next();
        prev->next(0);
        formal = formal->next();
    }
    if (arg) {
        err_ << args->location();
        err_ << "Too many arguments to function '" << id << "'\n";
        env_->error();
    }
    if (formal) {
        if (args) {
            err_ << args->location();
        } else {
            err_ << rec->location();
        }
        err_ << "Not enough arguments to function '" << id << "'\n";
        env_->error();
    }

    return out;    
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
    assert(env_->errors()||var->type()!=env_->no_type());
    assert(scope_.size());
    scope_.back()->variable(var);
    assert(var->scope()==scope());
}

void SemanticAnalyzer::enter_scope() {
    scope_.push_back(new Scope);
}

void SemanticAnalyzer::exit_scope() {
    scope_.pop_back();
}

void SemanticAnalyzer::initial_assignment(Assignment* expr) {
    // Checks the initial assignment of a variable for type-correctness.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Type::Ptr declared = expr->declared_type();
    Type::Ptr type = (var && declared != env_->no_type()) ? var->type() : 0;
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Expression::Ptr init = expr->initializer();

    variable(new Variable(id, Operand(), declared));

    // The variable was declared with an explicit type, but the variable
    // already exists.
    if (type && scope() == var->scope()) {
        err_ << expr->location();
        err_ << "Duplicate definition of variable '" << id << "'\n";
        env_->error();
        return;
    }
    // The variable is already declared as an attribute.
    if (attr) {
        err_ << expr->location();
        err_ << "Local variable declaration shadows attribute\n";
        env_->error();      
        return;
    }
    if (!init->type()) {
        return; 
    }

    // The variable was declared with an explicit type, and the init
    // does not conform to that type.
    if (!init->type()->subtype(declared)) {
        err_ << init->location();
        err_ << "Expression does not conform to type '";
        err_ << declared << "'\n";
        env_->error();
        return;
    }

    // Initializer is of the 'Any' type or an alternate type, but the
    // declared type is not equal.  Insert a cast expression.
    if (init->type()->is_alt_type() && !declared->equals(init->type())) {
        expr->initializer(new Cast(init->location(), declared, init));
    } else if (init->type()->is_value() && declared->is_alt_type()) {
        expr->initializer(new Box(init->location(), init->type(), init));
    }
}

void SemanticAnalyzer::secondary_assignment(Assignment* expr) {
    // Checks an assignment to a variable that has already been defined.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Type::Ptr declared = expr->declared_type();
    Type::Ptr type = (var && declared != env_->no_type()) ? var->type() : 0;
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Expression::Ptr init = expr->initializer();

    // The variable was already declared, but the init type is not
    // compatible with the variable type.
    if (type && !init->type()->subtype(type)) {
        err_ << init->location();
        err_ << "Expression does not conform to type '";
        err_ << type << "' in assignment of '" << id << "'\n";
        env_->error();
        return;
    }

    // The variable is an attribute, but the type does not match the type
    // of the attribute that is being assigned
    if (attr && !init->type()->subtype(attr->type())) {
        err_ << init->location();
        err_ << "Expression does not conform to type '";
        err_ << attr->type() << "' in assignment of '" << id << "'\n";
        env_->error();
        return;
    }

    // Initializer is of the 'Any' type, but the variable type is not.
    // Insert a cast expression.
    if (type) {
        Type::Ptr it = init->type();
        if (it->is_alt_type() && !type->equals(init->type())) {
            expr->initializer(new Cast(init->location(), type, init));
        } else if (it->is_value() && type->is_alt_type()) {
            expr->initializer(new Box(init->location(), it, init));
        }
    } else {
        variable(new Variable(id, Operand(), init->type())); 
    }
}

void SemanticAnalyzer::constructor() {
    // Check for a constructor, and generate one if it isn't present
    if (!class_->function(env_->name("@init"))) {
        String::Ptr nm = env_->name("@init");
        Type::Ptr vt = env_->void_type();
        Location loc = class_->location();
        Block::Ptr block(new Block(loc, 0, 0));
        class_->feature(new Function(loc, env_, nm, 0, 0, vt, block));
    }
}

void SemanticAnalyzer::destructor() {
    // Check for a destructor, and generate one if it isn't present
   String::Ptr nm = env_->name("@destroy");
    if (!class_->function(nm)) {
        Type::Ptr st = env_->self_type();
        Type::Ptr vt = env_->void_type();
        Location loc = class_->location();
        Block::Ptr block(new Block(loc, 0, 0));
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, env_, nm, self, 0, vt, block));
    }
}

void SemanticAnalyzer::accessor(Attribute* feature) {
    // Insert a setter for the attribute if it doesn't already exist
    String::Ptr get = env_->name(feature->name()->string()+"?");
    if (!class_->function(get) && !feature->is_private()) {
        Location loc = class_->location();
        String::Ptr id = feature->name();
        Identifier::Ptr attr(new Identifier(loc, env_->name(""), id));
        Return::Ptr ret(new Return(loc, attr)); 
        Block::Ptr block(new Block(loc, 0, ret));
        Type::Ptr st = env_->self_type();
        Type::Ptr ft = feature->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, env_, get, self, 0, ft, block));
    }
}

void SemanticAnalyzer::mutator(Attribute* feature) {
    // Insert a getter for the attribute if it doesn't already exist
    String::Ptr set = env_->name(feature->name()->string()+"=");
    if (!class_->function(set) && !feature->is_immutable()
            && !feature->is_private()) {
        Location loc = class_->location();
        String::Ptr id = env_->name("_arg0");
        Identifier::Ptr val(new Identifier(loc, env_->name(""), id)); 
        Assignment::Ptr assign(new Assignment(loc, feature->name(), 0, val)); 
        Block::Ptr block(new Block(loc, 0, new Simple(loc, assign)));
        Type::Ptr st = env_->self_type();
        Type::Ptr vt = env_->void_type();
        Type::Ptr ft = feature->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        Formal::Ptr arg(new Formal(loc, env_->name("_arg0"), ft));
        self->next(arg);
        class_->feature(new Function(loc, env_, set, self, 0, vt, block));
    }
}

Type* SemanticAnalyzer::fix_generics(Type* parent, Type* type) {
    // Resolves a generic type (e.g., :a) by looking up the actual type in the
    // container type 'parent'.  For example, if 'parent' is Array[String], and
    // :a is the first type parameter of array, this function would return
    // 'String'.  This function recursively substitutes generics, so List[:a]
    // would return List[String] if the 'parent' type is List.
    if (type->is_generic()) {
        return parent->generic(type->name());
    }
    if (!type->generics()) {
        return type;
    }

    // Iterate through all the generics in this type, and replace them with     
    // the resolved generic type.
    Generic::Ptr first;
    Generic::Ptr last;

    for (Generic::Ptr g = type->generics(); g; g = g->next()) {
        Type::Ptr t = fix_generics(parent, g->type());
        Generic::Ptr gen(new Generic(t));
        if (!first) {
            first = gen;
            last = gen;
        } else {
            last->next(gen);
            last = gen;
        }
    } 
    String* qn = type->qualified_name();
    return new Type(type->location(), qn, first, env_);
}

void SemanticAnalyzer::assign_enums(Class* feature) {
    // Assigns values to enums and checks to ensure that there are no duplicates.
    bool has_auto_enum = false;
    int next_enum_value = 0;

    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Constant::Ptr cons = dynamic_cast<Constant*>(f.pointer())) {
            if (!cons->initializer()) {
                // Case 1: Auto-assigned enum constant value (no initializer
                // expression)
                String::Ptr str(env_->integer(stringify(next_enum_value++)));
                IntegerLiteral::Ptr lit(new IntegerLiteral(Location(), str));
                lit->type(feature->type()); 
                cons->initializer(lit);
                has_auto_enum = true;
            } else if (feature->is_enum()) {
                // Case 2: Manually-assigned enum constant value (initializer 
                // expression present).
                Expression* init = cons->initializer();
                if (IntegerLiteral::Ptr lit = dynamic_cast<IntegerLiteral*>(init)) {
                    std::stringstream ss(lit->value()->string());
                    int value;
                    ss >> value;
                    next_enum_value = std::max(next_enum_value, value+1);
                    if (has_auto_enum) {
                        err_ << lit->location();
                        err_ << "Explicit enum constants defined ";
                        err_ << "after auto enum constant(s)\n";
                        env_->error();
                    }
                    lit->type(feature->type());
                }
            }
        }
    }
}
