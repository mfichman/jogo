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

    for (Module::Itr m = env_->modules(); m; ++m) {
        m(this);
    }    
    err_->flush();
}

void SemanticAnalyzer::operator()(Module* feature) {
    // Iterate through all functions and classes in the module, and check 
    // their semantics.  Ensure there are no duplicate functions or classes.
    // FixMe: Do not iterate over features by module.  Iterate over files or
    // over features in general instead.
    std::set<String::Ptr> features;
    for (Feature::Itr f = feature->features(); f; ++f) {

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

    if (feature->is_primitive()) {
        destructor();
    } else if (feature->is_object()) {
        destructor();
        constructor();
    } else if (feature->is_ref()) {
        // Pass 
    } else if (feature->is_compound()) {
        constructor();
        copier();
        destructor();
    } else {
        assert(!"Invalid type");
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
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (f->is_component()) {
            Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer());
            Type::Ptr m = attr->declared_type();
            if (m->clazz()) {
               if (feature->is_interface() && !m->is_interface()) {
                   err_ << m->location();
                   err_ << "Type embedded in '" << feature->name();
                   err_ << "' must be an interface\n";
                   env_->error();
               } else if (feature->is_object() && m->is_interface()) {
                   err_ << m->location();
                   err_ << "Type embedded in '" << feature->name();
                   err_ << "' must be an object or value\n"; 
                   env_->error();
               } else if (feature->is_value() && m->is_interface()) {
                   err_ << m->location();
                   err_ << "Type embedded in '" << feature->name();
                   err_ << "' must be an object or value\n"; 
                   env_->error();
               }
            } else {
               err_ << m->location();
               err_ << "Undefined type '" << m << "'\n";
               env_->error();
            }
        }
    }

    assign_enums(feature);

    // Iterate through all the features and generate accessors.  Also, select
    // a numeric value for each enum constant within the class
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            operator()(attr);
        }
    }
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }

    genclass_.clear();
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

void SemanticAnalyzer::operator()(HashLiteral* lit) {
    // For a HashLiteral, the type of the expression is Hash[Pair[A,B]], where
    // Pair[A,B] is the type of the first expression argument.  If subsequent
    // types do not match, then the type is Pair[A,B]
    Construct::Ptr cons(new Construct(lit->location(), env_->bottom_type(), 0));
    Member::Ptr member(new Member(lit->location(), cons, env_->name("@insert")));
    if (!type_ || type_->is_top() || type_->is_interface()) {
        type_ = 0;
        Type::Ptr ktype;
        Type::Ptr vtype;
        for (Expression::Ptr a = lit->arguments(); a; a = a->next()) {
            Construct::Ptr pair = dynamic_cast<Construct*>(a.pointer());
            assert(pair && "Non-pair constructor in hash literal");
            Expression::Ptr first = pair->arguments();
            Expression::Ptr second = pair->arguments()->next();
            first(this);
            second(this); 
            Type* kt = pair->arguments()->type();
            Type* vt = pair->arguments()->next()->type();
            ktype = (ktype && !kt->subtype(ktype)) ? env_->any_type() : kt; 
            vtype = (vtype && !vt->subtype(vtype)) ? env_->any_type() : vt;
        }
        if (!ktype) {
            ktype = env_->any_type();
        }
        if (!vtype) {
            vtype = env_->any_type();
        }
        String::Ptr nm(env_->name("Hash"));
        Generic::Ptr gens(new Generic(ktype));
        gens = append(gens, new Generic(vtype));
        Type::Ptr type = new Type(lit->location(), nm, gens, env_);
        cons->type(type);
        lit->type(type);
    } else {
        cons->type(type_);  
        lit->type(type_);
        type_ = 0;
    }
    cons(this);
    for (Expression::Ptr a = lit->arguments(); a; a = a->next()) {
        Construct::Ptr pair = static_cast<Construct*>(a.pointer());
        Call::Ptr call(new Call(a->location(), member, pair->arguments()));
        call(this);
    }
}

void SemanticAnalyzer::operator()(ArrayLiteral* lit) {
    // For an ArrayLiteral, the type of the expression is Array[First], where
    // First is the type of the first expression argument.  If subsequent types
    // do not match, then the type is Array[Any]
    String::Ptr sizestr(env_->integer("0"));
    IntegerLiteral::Ptr size(new IntegerLiteral(lit->location(), sizestr));
    Construct::Ptr cons(new Construct(lit->location(), env_->bottom_type(), size));
    Member::Ptr member(new Member(lit->location(), cons, env_->name("push"))); 
    if (!type_ || type_->is_top() || type_->is_interface()) {
        type_ = 0;
        Type::Ptr etype;
        for (Expression::Ptr a = lit->arguments(); a; a = a->next()) {
            a(this);
            Type* et = a->type();
            etype = (etype && !et->subtype(etype)) ? env_->any_type() : et;
        }
        if (!etype) {
            etype = env_->any_type();
        }
        String::Ptr nm(env_->name("Array"));
        Generic::Ptr gen(new Generic(etype));
        Type::Ptr type(new Type(lit->location(), nm, gen, env_));
        cons->type(type);
        lit->type(type);
    } else {
        cons->type(type_); 
        lit->type(type_);
        type_ = 0;
    }
    cons(this);
    for (Expression::Ptr a = lit->arguments(); a; a = a->next()) {
        Expression::Ptr next(a->next());
        a->next(0);
        Call::Ptr call(new Call(a->location(), member, a));
        call(this);
        a->next(next);
        // FIXME: Hack to temporarily change the next() pointer
    }
       
}

void SemanticAnalyzer::operator()(Let* stmt) {
    // For a let stmt, introduce the new variables in a new scope, and 
    // then check the block.
    enter_scope();
    Statement::Ptr block = stmt->block();
    for (Expression::Ptr v = stmt->variables(); v; v = v->next()) {
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
    if (expr->check_type()->is_union()) {
        err_ << expr->check_type()->location();
        err_ << "Type in 'is' expression cannot be a union\n";
        env_->error();
    }
    if (expr->check_type()->is_any()) {
        err_ << expr->check_type()->location();
        err_ << "Type in 'is' expression cannot be 'Any'\n";
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
    assert(expression->type()&&"Missing Box expression type");
}

void SemanticAnalyzer::operator()(Cast* expression) {
    // Cast expression.  Converts an item of the type specified by 'expr to
    // an item of the type specified by the cast type.
    Expression::Ptr child = expression->child();
    child(this);

    Type::Ptr type = expression->type();
    assert(type&&"Missing Case expression type");
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
    type_ = 0;
    expr(this);

    Type::Ptr type = expr->type();
    if (type->is_top()) {
        expression->type(env_->top_type());
        return;
    }
    Class::Ptr clazz = type->clazz();
    assert(clazz && "Class not found");
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
        expression->type(env_->top_type());
        return;
    }

    // Try to look up the attribute.  Then take the type from the attribute
    // and use it, if the attribute is public.
    Attribute::Ptr attr = clazz->attribute(id);
    if (attr && attr->type() && attr->type()->is_bottom()) {
        err_ << expression->location(); 
        err_ << "Illegal circular reference\n";
        env_-> error();
        expression->type(env_->top_type());
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
        expression->type(env_->top_type());
        return;
    }
    //func(this);
    
    assert(func->type() && "Attribute has no type");
    expression->type(canonical(func->type(), expr->type()));
    expression->function(func);
    if (function_) {
        function_->called_func(func);
    }

    if (func->is_private() && func->parent() != class_) {
        err_ << expression->location();
        err_ << "Function '" << id << "' is private in class '";
        err_ << clazz->name() << "'\n";
        env_->error();
    }
    assert(expression->type());
}

void SemanticAnalyzer::operator()(Call* call) {
    // Look up the function by name in the current context.  The function may
    // be a member of the current module, or of a module that was imported in
    // the current compilation unit.
    if (call->type()) { return; }

    // Check the expression that is being called.  Function resolution happens
    // here, because it depends on the type of the child node.
    Expression::Ptr expr = call->expression();
    expr(this);

    // A function value should have been assigned by the child.
    Function::Ptr func = call->function();
    call->type(env_->top_type());
    if (!func) { return; }

//    func(this);

    // Check to make sure the resolved function is not private
    if (func->is_private() && func->parent() != class_) {
        err_ << call->location();
        err_ << "Function '" << func->name() << "' is private in class '";
        err_ << call->receiver()->type() << "'\n";
        env_->error();  
    }
    
    // Figure out what the receiver type is.  If the type is 'self' then set
    // the type to the actual containing class.
    Type::Ptr receiver;
    if (call->receiver()) {
        call->receiver()->next(call->arguments());
        call->arguments(call->receiver());
        receiver = call->receiver()->type();
    }

    // Evaluate types of argument expressions
    Formal::Ptr f = func->formals();
    for (Expression::Ptr a = call->arguments(); a; a = a->next()) {
        // Push the type of the formal parameter down to the called argument.
        // If the canonical type is generic, then the called argument will bind
        // to whatever the default type is for that literal.  For example, if
        // an array literal is passed in as an argument to a generic function,
        // then the type of the array literal will be Array[ElemType].
        type_ = f ? canonical(f->type(), receiver) : 0;
        a(this);
        f = f ? f->next() : 0;
    }
    type_ = 0;
    call->arguments(args(call, call->arguments(), func, receiver));
    //call->type(canonical(func->type(), receiver));
    call->type(canonical(func->type(), receiver, func, call->arguments()));
    assert(call->type() && "Missing call type");
    if (function_) {
        function_->called_func(func);
    }
}

void SemanticAnalyzer::operator()(Construct* expr) {
    // Evaluate type of argument expression
    Type::Ptr type = expr->type();
    type(this);

    // Look up constructor class
    Class::Ptr clazz = type->clazz(); 
    if (!clazz) {
        expr->type(env_->top_type());
        return;
    }
    if (clazz->is_interface()) {
        err_ << expr->location();
        err_ << "Constructor called for interface type '";
        err_ << clazz->name() << "'\n";
        env_->error();
        expr->type(env_->top_type());
        return;
    }

    // Look up the constructor using the class object.
    Function::Ptr constr = clazz->function(env_->name("@init"));
    if (constr && constr->is_private()) {
        err_ << expr->location();
        err_ << "Constructor is private in class '" << type << "'\n";
        env_->error();  
    }
    Formal::Ptr f = constr->formals();
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        type_ = f ? canonical(f->type(), type) : 0;
        a(this); 
        f = f ? f->next() : 0;
    }
    type_ = 0;
    if (clazz->is_enum()) {
        err_ << expr->location();
        err_ << "Enums do not have constructors\n";
        env_->error();
    } else if (!constr) {
        err_ << expr->location();
        err_ << "Class '" << type << "' has no constructor\n";
        env_->error();
    } else {
        expr->arguments(args(expr, expr->arguments(), constr, type));  
    }
    if (function_) {
        function_->called_func(constr);
    }
}

void SemanticAnalyzer::operator()(ConstantRef* expression) {
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
        expression->type(env_->top_type());
        return;
    }
    if (constant->type() && constant->type()->is_bottom()) {
        err_ << expression->location();
        err_ << "Illegal circular reference\n";
        env_->error();
        expression->type(env_->top_type());
        return;
    }
    constant(this);
    expression->constant(constant);
    expression->type(constant->type()); 
    assert(expression->type());
}

void SemanticAnalyzer::operator()(IdentifierRef* expression) {
    // Determine the type of the identifier from the variable store.  If the
    // variable is undeclared, set the type of the expression to no-type to
    // prevent further error messages.
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope(); 
    Call::Ptr call = dynamic_cast<Call*>(expression->parent());
    Type::Ptr type; // Class of the object stored in 'expression'

    if (expression->type()) {
        return;
    } else if (Variable::Ptr var = variable(id)) {
        type = var->type();
    } else if (class_ && scope->is_empty()) {
        Attribute::Ptr attr = class_->attribute(id);
        if (attr) {
            if (attr->type() && attr->type()->is_bottom()) {
                err_ << expression->location();
                err_ << "Illegal circular reference\n";
                env_->error();      
                expression->type(env_->top_type());
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
            expression->type(env_->top_type());
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
/*
    if (class_ && scope->is_empty()) {
        call->function(class_->function(id));
        if (call->function()) {
            Location loc = expression->location();
            String::Ptr scope = env_->name("");
            String::Ptr name = env_->name("self");
            IdentifierRef::Ptr self = new IdentifierRef(loc, scope, name);
            self->type(class_->type());
            call->receiver(self);
            return;
        }
    } 
*/

    // Parent is a function call; we need to try to evaluate this identifier as
    // a function.  In the first case, the identifier actually resolves
    // directly to a function, so use it.
    call->function(expression->file()->function(scope, id));

    // Special case: resolve using the type of the first argument.  This is a
    // rule unique to Jogo (as far as I know), and allows both of the following
    // forms: 7.cos() and cos(7).  If there is a function named 'cos' in the
    // same scope, that function is used instead of the member function.
    if (scope->is_empty() && !call->function() && call->arguments()) {
        Expression::Ptr arg = call->arguments();
        arg(this);

        Class::Ptr clazz = arg->type()->clazz();
        if (clazz) {
            call->function(clazz->function(id));
        } 
    }

    // If all attempts to resolve the function fail, then it is missing.
    if (!call->function()) {
        err_ << call->location();
        err_ << "Undefined function '";
        if (!scope->is_empty()) {
            err_ << scope << "::";
        }
        err_ << id << "'\n";
        env_->error();
    }
}

void SemanticAnalyzer::operator()(Empty* expression) {
    expression->type(env_->top_type());
}

void SemanticAnalyzer::operator()(ParseError* expression) {
    expression->type(env_->top_type());
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
    enter_scope();
    guard(this);
    if (!guard->type()->is_boolifiable()) {
        err_ << statement->location();
        err_ << "Guard expression must have type '";
        err_ << env_->bool_type() << "'";
        err_ << "\n";
        env_->error();
    }
    block(this);
    exit_scope();
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
    Type::Ptr declared = expr->declared_type();
    Expression::Ptr init = expr->initializer();

    // Push the suggested type of the expression down from the assignment or
    // attribute declaration.
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    if (declared && !declared->is_top()) {
        type_ = declared;
    } else if (var) {
        type_ = var->type();
    } else if (attr) {
        type_ = attr->declared_type();
    }
    init(this);
    type_ = 0;
    
    expr->type(!init->type()->is_top() ? init->type() : declared.pointer());
    if (!declared->is_top()) {
        // If declared_type == top_type, then the variable declaration has no
        // true declared type, but it defines a new variable regardless of
        // whether or not the variable is already shadowed by another variable.
        declared(this);
        // Check to make sure the declared type is valid; if it isn't, then
        // set the variable to top_type and return.
        if (!declared->clazz()) {
            variable(new Variable(id, Operand(), env_->top_type(), !expr->is_mutable()));
            return;
        }
    }

    // Attempt to assign void to a variable during initialization.
    if (init->type()->is_void()) {
        err_ << init->location();
        err_ << "Void value assigned to variable '" << id << "'\n";
        env_->error();
        variable(new Variable(id, Operand(), env_->top_type(), !expr->is_mutable()));
        return;
    }
    
    if (!declared->is_top() || expr->is_let()) {
        // If declared_type == top_type, then the variable declaration has no
        // true declared type, but it defines a new variable regardless of
        // whether or not the variable is already shadowed by another variable.
        initial_assignment(expr);
    } else {
        secondary_assignment(expr);
    }

    var = variable(id);
    if (var && init->type()->is_alt() && !var->type()->equals(init->type())) {
        // Initializer is of the 'Any' or 'Union' type but the storage type is
        // not of the 'Any' or 'Union' type.  Insert a cast expression, which
        // will extract nil or the requested type.
        expr->initializer(new Cast(init->location(), var->type(), init));
    } else if (var && init->type()->is_value() && var->type()->is_alt()) {
        // Initializer is of the 'Value' type, but the storage type is
        // 'Any' or 'Union'.  Insert an expression to box the value.
        expr->initializer(new Box(init->location(), init->type(), init));
    }
}

void SemanticAnalyzer::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    if (dynamic_cast<Empty*>(expr.pointer())) {
        return;
    }
    expr(this);
    return_ = expr->type();
    if (!expr->type()->subtype(function_->type())) {
        err_ << statement->location();
        err_ << "Return must conform to type '";
        err_ << function_->type() << "'";
        err_ << "\n";
        env_->error();
    }

    Type::Ptr ft = function_->type();
    Type::Ptr type = expr->type();
    if (type->is_value() && ft->is_alt()) {
        // Return value is an 'Any' or a 'Union', but the type of the
        // expression is a 'Value.'  Box up the value.
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
    if (feature->is_checked()) { return; }
    feature->is_checked(true);

    // Save the current class an scope info
    ContextAnchor context(this);
    class_ = dynamic_cast<Class*>(feature->parent());
    function_ = feature;
    if (class_) { assign_generic_classes(class_->type()); }

    bool is_functor_func = feature->name()->string().find("@case") == 0;

    // Check for duplicate functions with the same name
    if (parent->feature(feature->name()) != feature) {
        err_ << feature->location();
        if (is_functor_func) {
            err_ << "Duplicate definition of function '@case(";
            err_ << feature->formals()->next()->type() << ")'\n";
        } else {
            err_ << "Duplicate definition of function '";
            err_ << feature->name() << "'\n";
        }
        env_->error();
    }

    // Check all formal parameters, and add them as local variables
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        assign_generic_classes(f->type());
    }
    if (feature->file()->is_interface_file()) { return; }

    enter_scope();
    for (Formal::Ptr f = feature->formals(); f; f = f->next()) {
        Type::Ptr type = f->type();
        type(this);
        variable(new Variable(f->name(), Operand(), type, true));
    }

    if (is_functor_func) {
        Formal::Ptr arg = feature->formals()->next();
        if (!arg) {
            err_ << feature->location();
            err_ << "Missing argument in functor case definition\n";
            env_->error();
        } else if (arg->next()) {
            err_ << arg->next()->location();
            err_ << "Extra arguments in functor case definition\n";
            env_->error();
        } else if (arg->type()->is_value()) {
            err_ << arg->type()->location();
            err_ << "Value type in functor case\n";
            env_->error();
        } else if (arg->type()->generics() || arg->type()->is_generic()) {
            err_ << arg->type()->location();
            err_ << "Generic type in functor case\n";
            env_->error();
        } else if (arg->type()->is_interface()) {
            err_ << arg->type()->location();
            err_ << "Interface type in functor case\n";
            env_->error();
        }
        if (!class_->is_functor()) {
            err_ << feature->location();
            err_ << "Functor case in non-functor type\n";
            env_->error(); 
        }
    }
    if (feature->is_constructor()) {
        String::Ptr nm = env_->name("self");
        variable(new Variable(nm, Operand(), class_->type(), true));
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
    if (block && block->children() && is_interface) {
        err_ << block->location();
        err_ << "Interface function '" << feature->name();
        err_ << "' has a body\n";
        env_->error();
    } else if (block && block->children() && is_native) {
        err_ << feature->location();
        err_ << "Native function '" << feature->name();
        err_ << "' has a body\n";
        env_->error();
    } else if (!block && !is_interface && !is_native && !is_lib_def) {
        err_ << feature->location();
        err_ << "Function '" << feature->name() << "' has no body\n";
        env_->error();
    }
    
    if (block && (block->children() || (!is_native && !is_interface))) { 
        block(this); 
        if (type != env_->void_type() && !return_) {
            err_ << feature->location();
            err_ << "Function '" << feature->name();
            err_ << "' must return a value\n";     
            env_->error();
        }
    }
        
    exit_scope();
    function_ = 0;
    genclass_.clear();
}

void SemanticAnalyzer::operator()(Constant* feature) {
    // Analyze a constant value.
    Expression::Ptr init = feature->initializer();
    Feature::Ptr parent = feature->parent();
    if (feature->type() && init->type()) { return; }

    if (parent->feature(feature->name()) != feature) {
        err_ << feature->location();
        err_ << "Duplicate definition of constant '";
        err_ << feature->name() << "'\n";
        env_->error();
    }

    Class::Ptr clazz = dynamic_cast<Class*>(parent.pointer());
    feature->type(env_->bottom_type());
    init(this);

    if (dynamic_cast<StringLiteral*>(init.pointer())) {
        feature->type(init->type());
    } else if (dynamic_cast<IntegerLiteral*>(init.pointer())) {
        feature->type(init->type());
    } else if (dynamic_cast<FloatLiteral*>(init.pointer())) {
        feature->type(init->type());
    } else if (Construct* constr = dynamic_cast<Construct*>(init.pointer())) {
        Expression* arg = constr->arguments();
        if (init->type()->is_primitive()&&arg&&!arg->next()
            &&dynamic_cast<IntegerLiteral*>(arg)) {
            // OK: Usable as a constant
        } else {
            err_ << feature->location();
            err_ << "Non-literal constant\n";
            env_->error();
        }
        feature->type(init->type());
    } else if (!init || dynamic_cast<Empty*>(init.pointer())) {
        if (clazz && clazz->is_enum()) {
            feature->type(clazz->type());
        } else if (!feature->declared_type()->is_top()) {
            feature->type(feature->declared_type());
        } else {
            err_ << feature->location();
            err_ << "Missing constant type or initializer\n";
            env_->error();
        }
    } else {
        err_ << feature->location();
        err_ << "Non-literal constant\n";
        env_->error();
    }

    env_->constant(feature);
}

void SemanticAnalyzer::operator()(Attribute* feature) {
    // Process an attribute of a class, check for type correctness, and eval
    // the initializer expression.
    Expression::Ptr init = feature->initializer();
    Feature::Ptr parent = feature->parent();
    Type::Ptr declared = feature->declared_type();
    if (feature->type()) { return; }
    if (feature->file()->is_interface_file()) {
        feature->type(declared);
        return;
    }

    // Save the current class and variable scope.
    ContextAnchor context(this);
    class_ = dynamic_cast<Class*>(feature->parent());
    assert(class_ && "Attribute with no class");
    assign_generic_classes(class_->type());
    if (class_->is_enum()) {
        if (feature->is_embedded()) {
            err_ << feature->location();
            err_ << "Cannot embed type '" << feature->name() << "' in ";
            err_ << "enum '" << class_->qualified_name() << "'\n";
            env_->error();
        } else {
            err_ << feature->location();
            err_ << "Cannot embed attribute '" << feature->name() << "' in ";
            err_ << "enum '" << class_->qualified_name() << "'\n";
            env_->error();
        }
    }

    // Check for self-embedded types
    if(feature->is_component() && feature->declared_type()->clazz() == class_) {
        err_ << feature->location();
        err_ << "Type '" << feature->name() << "' is self-embedded\n";
        env_->error();
    }

    // Check for duplicate attributes
    if (parent->feature(feature->name()) != feature) {
        if(feature->is_component()) {
            err_ << feature->location();
            err_ << "Type '" << feature->name() << "' is embedded multiple ";
            err_ << "times in '" << parent->name() << "'\n";
            env_->error();
        } else {
            err_ << feature->location();
            err_ << "Duplicate definition of attribute '";
            err_ << feature->name() << "'\n";
            env_->error();
        }
    } 

    // Make sure that the attribute type conforms to the declared type of the
    // attribute, if there is one.  Set the type to 'bottom' while checking the
    // init, in case there are circular references.
    feature->type(env_->bottom_type());
    type_ = declared;
    init(this);
    type_ = 0;

    if (declared->is_top()) {
        // No explicitly declared type, but the init can be used to infer the
        // type.
        feature->type(init->type());
    } else {
        // Check that the feature type exists.
        declared(this);    
        feature->type(declared);
    }

    Class::Ptr clazz = feature->type()->clazz();
    Function::Ptr ctor = clazz ? clazz->constructor() : 0;
    if (feature->is_component()) {
        if (ctor && ctor->formals()) {
            // Mixin w/ constructor args
            err_ << init->location();
            err_ << "Embedded type '" << feature->type() << "' has ";
            err_ << "constructor arguments\n"; 
            env_->error();
        } else {
            feature->initializer(new Construct(Location(), feature->type(), 0));
            assert(feature->type() && " no type for embedded attr"); 
        }
    } else if (declared->is_compound() && init->type()->is_top()) {
        // No constructor for a value type attribute 
        err_ << init->location();
        err_ << "Uninitialized value type attribute '";
        err_ << feature->name() << "'\n";
        env_->error();
        return;
    }

    // Make sure that the init and the feature type conform.
    if (!init->type()->subtype(declared)) {
        err_ << feature->location();
        err_ << "Expression does not conform to type '";
        err_ << declared << "'";
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
        Expression::Ptr arg = new IdentifierRef(loc, env_->name(""), id); 
        String::Ptr fid = env_->name("_"+id->string());
        Formal::Ptr formal = new Formal(loc, fid, var->type()); 
        IdentifierRef::Ptr rhs = new IdentifierRef(loc, env_->name(""), fid);
        Assignment::Ptr as = new Assignment(loc, id, env_->top_type(), rhs); 
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
        attr->type(type);
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
    if (type->is_top() || type->is_void()) {
        return;
    }
    if (type->is_generic() && !type->clazz()) {
        std::map<String::Ptr,Class::Ptr>::iterator it = genclass_.find(type->name());
        if (it != genclass_.end()) {
            type->clazz(it->second);
        } else {
            err_ << type->location();
            err_ << "Unbound generic type '" << type->name() << "'\n";
            env_->error();
            type->is_top(true);
        }
        return;
    }

    if (!type->clazz()) {
        err_ << type->location();
        err_ << "Undefined type '" << type << "'\n";
        env_->error();
        type->is_top(true);
        return;
    }

    // Check generics for type resolution
    for (Generic::Ptr g = type->generics(); g; g = g->next()) {
        Type::Ptr gen = g->type();
        gen(this);
        if (!type->is_interface() && g->type()->is_value()) {
            err_ << type->location();
            err_ << "Primitives and value types cannot be used in generic classes\n";
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
        type->is_top(true);
    }
    if (formal) {
        err_ << type->location();
        err_ << "Not enough arguments for type '" << type->name() << "'\n";
        env_->error();
        type->is_top(true);
    }
}

Expression::Ptr SemanticAnalyzer::args(Expression* call, Expression* args, Function* fn, Type* rec) {
    // Check argument types versus formal parameter types
    String::Ptr id = fn->name();
    Formal::Ptr formal = fn ? fn->formals() : 0;
    Expression::Ptr arg = args;
    if (arg) {
        arg->last(0);
    }

    std::vector<Expression::Ptr> outv;

    while (arg && formal) {
        // Get the formal type.  If the type is 'self', then the formal
        // parameter is the type of the receiver.  If the type is a generic,
        // then look up the actual type from the class' definition.
        Type::Ptr ft = canonical(formal->type(), rec, fn, args);

        // Get the actual argument type.
        Type::Ptr at = arg->type();
    
        // Build the modified argument list (which may include cast 
        // expressions that were auto-inserted by the compiler).
        if (!at->subtype(ft) && !formal->is_self()) {
            err_ << arg->location();
            err_ << "Argument does not conform to type '" << ft << "'";
            if (ft != formal->type()) {
                err_ << " (" << formal->type() << ")";
            }
            err_ << "\n";
            env_->error();
        }

        // If the formal type is not 'Any' but the argument is of type 'Any',
        // the automatically insert a cast.  Cast also unboxes primitives and
        // value types if necessary.
        Expression::Ptr actual;
        if (at->is_alt() && !ft->equals(at)) {
            actual = new Cast(arg->location(), ft, arg);
        } else if (at->is_value() && ft->is_alt()) {
            actual = new Box(arg->location(), at, arg); 
        } else {
            actual = arg;
        }
        outv.push_back(actual);
        formal = formal->next();
        arg = arg->next();
    }

    // Unlink all the original arguments
    for (Expression::Ptr arg = args; arg;) {
        Expression::Ptr prev = arg;
        arg = arg->next();
        prev->next(0);
        prev->last(0);
    }

    // Re-link all the processed arguments
    Expression::Ptr out;
    for (size_t i = 0; i < outv.size(); ++i) {
        out = append(out.pointer(), outv[i].pointer()); 
    }
    if (arg) {
        err_ << args->location();
        err_ << "Too many arguments to function '" << id << "'\n";
        env_->error();
    }
    if (formal) {
        if (args) {
            err_ << args->location();
        } else if (rec) {
            err_ << rec->location();
        } else {
            err_ << call->location();
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
    assert(env_->errors() || !var->type()->is_top());
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
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Expression::Ptr init = expr->initializer();
    attr(this);

    bool immut = attr ? attr->is_immutable() : !expr->is_mutable(); 
    if (declared->is_top()) {
        variable(new Variable(id, Operand(), expr->type(), immut));
    } else {
        variable(new Variable(id, Operand(), declared, immut));
    }

    // The variable was declared with an explicit type, but the variable
    // already exists.
    if (var && scope() == var->scope()) {
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

    // The variable was declared with an explicit type, and the init
    // does not conform to that type.
    if (!init->type()->subtype(declared)) {
        err_ << init->location();
        err_ << "Expression does not conform to type '";
        err_ << declared << "'\n";
        env_->error();
        return;
    }

    // No constructor for a value type variable
    if (declared->is_compound() && init->type()->is_top()) {
        err_ << init->location();
        err_ << "Uninitialized value type variable '" << id << "'\n";
        env_->error();
        return;
    }
}

void SemanticAnalyzer::secondary_assignment(Assignment* expr) {
    // Checks an assignment to a variable that has no explicit type def.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Type::Ptr declared = expr->declared_type();
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Expression::Ptr init = expr->initializer();
    attr(this);

    // Attempt to assign to an immutable var, usually a param
    if (var && var->is_immutable()) {
        err_ << expr->location();
        err_ << "Value assigned to immutable variable '" << id << "'\n"; 
        env_->error();
        return;
    }

    // The variable was already declared, but the init type is not
    // compatible with the variable type.
    if (var && !init->type()->subtype(var->type())) {
        err_ << init->location();
        err_ << "Expression does not conform to type '";
        err_ << var->type() << "' in assignment of '" << id << "'\n";
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
    bool immut = attr ? attr->is_immutable() : !expr->is_mutable();
    if (!var) {
        variable(new Variable(id, Operand(), expr->type(), immut));
    }
}

void SemanticAnalyzer::copier() {
    // Create a copier function for value types.  A copier takes the value to
    // be copied as the first argument.
    String::Ptr nm = env_->name("@copy");
    Function::Ptr copy = class_->function(nm);
    // Custom copy constructor!
    if (!copy) {
        Type::Ptr vt = env_->void_type();
        Location loc = class_->location();
        Block::Ptr block(new Block(loc, 0, 0));
        Formal::Ptr self(new Formal(loc, env_->name("self"), class_->type()));
        Formal::Ptr other(new Formal(loc, env_->name("val"), class_->type())); 
        self->next(other);
        class_->feature(new Function(loc, env_, nm, self, 0, vt, block));
    } else {
        //assert(copy->parent() == class_ && "Not supported");
    }
}

void SemanticAnalyzer::constructor() {
    // Check for a constructor, and generate one if it isn't present
    String::Ptr nm = env_->name("@init");
    Function::Ptr ctor = class_->function(nm);
    if (!ctor || (ctor->parent() != class_)) {
        Type::Ptr vt = env_->void_type();
        Location loc = class_->location();
        Block::Ptr block(new Block(loc, 0, 0));
        class_->feature(new Function(loc, env_, nm, 0, 0, vt, block));
    }
}

void SemanticAnalyzer::destructor() {
    // Check for a destructor, and generate one if it isn't present
    String::Ptr nm = env_->name("@destroy");
    Function::Ptr dtor = class_->function(nm);
    if (!dtor || (dtor->parent() != class_)) {
        Type::Ptr st = class_->type();
        Type::Ptr vt = env_->void_type();
        Location loc = class_->location();
        Block::Ptr block(new Block(loc, 0, 0));
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, env_, nm, self, 0, vt, block));
    }
}

void SemanticAnalyzer::accessor(Attribute* feat) {
    // Insert a setter for the attribute if it doesn't already exist
    String::Ptr get = env_->name(feat->name()->string()+"?");
    if (!class_->function(get) && !feat->is_private() 
        && !feat->is_component()) {

        Location loc = class_->location();
        String::Ptr id = feat->name();
        IdentifierRef::Ptr attr(new IdentifierRef(loc, env_->name(""), id));
        Return::Ptr ret(new Return(loc, attr)); 
        Block::Ptr block(new Block(loc, 0, ret));
        Type::Ptr st = class_->type();
        Type::Ptr ft = feat->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        class_->feature(new Function(loc, env_, get, self, 0, ft, block));
    }
}

void SemanticAnalyzer::mutator(Attribute* feat) {
    // Insert a getter for the attribute if it doesn't already exist
    String::Ptr set = env_->name(feat->name()->string()+"=");
    if (!class_->function(set) && !feat->is_immutable() && !feat->is_private()
        && !feat->is_component()) {

        Location loc = class_->location();
        String::Ptr id = env_->name("_arg0");
        String::Ptr fn = feat->name();
        IdentifierRef::Ptr val(new IdentifierRef(loc, env_->name(""), id)); 
        Assignment::Ptr assign(new Assignment(loc, fn, env_->top_type(), val)); 
        Block::Ptr block(new Block(loc, 0, new Simple(loc, assign)));
        Type::Ptr st = class_->type();
        Type::Ptr vt = env_->void_type();
        Type::Ptr ft = feat->type();
        Formal::Ptr self(new Formal(loc, env_->name("self"), st));
        Formal::Ptr arg(new Formal(loc, env_->name("_arg0"), ft));
        self->next(arg);
        class_->feature(new Function(loc, env_, set, self, 0, vt, block));
    }
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

void SemanticAnalyzer::assign_generic_classes(Type* type) {
    // Recursively generate anonymous generic types for each unbound generic
    // type variable.
    if (type->is_generic()) {
        std::map<String::Ptr,Class::Ptr>::iterator it = genclass_.find(type->name());
		if (type->clazz() && it != genclass_.end()) {
			assert(type->clazz() == it->second && "Type reassigned");
		} else if (type->clazz()) {
			genclass_.insert(std::make_pair(type->name(), type->clazz()));
		} else if (it == genclass_.end()) {
            Class::Ptr clazz = new Class(type->location(), env_, type);
            env_->genclass(clazz);
            genclass_.insert(std::make_pair(type->name(), clazz));
            type->clazz(clazz);
		} else {
			type->clazz(it->second);
		}
    } else {
        for (Generic::Ptr g = type->generics(); g; g = g->next()) {
            assign_generic_classes(g->type()); 
        }
    }
}

Type* SemanticAnalyzer::binding_for(Type* in, Type* map, Type* bound) {
    // Looks up a type variable.  The 'map' type is used to locate the type
    // variable within the structure given by 'bound'.  Both 'map' and
    // 'bound' must have the same structure.  Example:
    // in => :a
    // map => Array[List[:a]]
    // bound => Array[List[String]]
    // out => String 
    // If the function fails to find a match, then it returns 'in'
    assert(in->is_generic() && "Not a generic type variable");
    if (map->is_generic()) {
        if (in->name() == map->name()) {
            return bound;
        } else {
            return in;
        }
    }
    Generic* genmap = map->generics();
    Generic* genbound = bound->generics();
    while (genmap && genbound) {
        assert(genmap->type()->is_generic() && "Bad map type");
        Type* out = binding_for(in, genmap->type(), genbound->type());
        if (out != in) {
            return out;
        }
        genmap = genmap->next();
        genbound = genbound->next();
    }
    return in;
}

Type* SemanticAnalyzer::binding_for(Type* in, Function* func, Expression* args) {
    // Looks up a type variable, given the function 'func' which defines the
    // type var names, and the arguments 'args', which bind real types to the
    // type var names.  Example:
    // in => :a
    // func => foo(var :a) :a 
    // args => "hello"
    // out => String
    // If the function fails to find a match, then it returns 'in'.
    assert(in->is_generic() && "Not a generic type variable");
    Formal* fm = func->formals();
    Expression* arg = args;
    while (fm && arg) {
        Type* out = binding_for(in, fm->type(), arg->type());
        if (in != out) {
            return out;
        }
        fm = fm->next();
        arg = arg->next();
    }
    return in;
}

Type* SemanticAnalyzer::binding_for(Type* in, Type* recv, Function* func, Expression* args) {
    // Looks up a type variable, first using 'receiver', and then using 'func'
    // and 'args' if the type could not be identified by the receiver alone.
    assert(in->is_generic() && "Not a generic type variable");
    if (recv) {
        Type::Ptr out = binding_for(in, recv->clazz()->type(), recv);  
        if (in != out) {
            return out;
        }
    } 
    return binding_for(in, func, args);
}

Type* SemanticAnalyzer::canonical(Type* in, Type* map, Type* bound) {
    // Replaces all type variables with concrete types, using "generic" to look
    // up the matching concrete type in "bound"
    if (in->is_generic()) { 
        return binding_for(in, map, bound); 
    } else if (in->generics()) {
        Generic::Ptr genout;
        for (Generic* g = in->generics(); g; g = g->next()) {
            Type* t = canonical(g->type(), map, bound);
            genout = append(genout, new Generic(t));
        }
        return new Type(in->location(), in->qualified_name(), genout, in->env());
    } else {
        return in;
    } 
}

Type* SemanticAnalyzer::canonical(Type* in, Function* func, Expression* args) {
    // Given a function and its arguments, find the type that 'type' maps to.
    // If 'type' is not a generic type var, then it maps to itself.  

    // FIXME: This function may need to be 'memoized' to have good performance
    // with large numbers of args, because it does a dumb search through all
    // the arguments looking for the mapped type.  This is linear in the number
    // of args.
    if (in->is_generic()) {
        return binding_for(in, func, args);
    } else if (in->generics()) { 
        Generic::Ptr genout;
        for (Generic* g = in->generics(); g; g = g->next()) {
            Type* t = canonical(g->type(), func, args);
            genout = append(genout, new Generic(t));
        }
        return new Type(in->location(), in->qualified_name(), genout, in->env());
    } else {
        return in;
    }
}

Type* SemanticAnalyzer::canonical(Type* in, Type* recv, Function* func, Expression* args) {
    // Look up the canonical form of the type by using  the receiver type and
    // function args to get the correct binding for each variable.
    if (in->is_generic()) {
        return binding_for(in, recv, func, args);
    } else if (in->generics()) { 
        Generic::Ptr genout;
        for (Generic* g = in->generics(); g; g = g->next()) {
            Type* t = canonical(g->type(), recv, func, args);
            genout = append(genout, new Generic(t));
        }
        return new Type(in->location(), in->qualified_name(), genout, in->env());
    } else {
        return in;
    }
}

Type* SemanticAnalyzer::canonical(Type* in, Type* receiver) {
    // Uses the 'receiver' type to get the canonical type of 'in'.  That is, if
    // 'in' is :a and receiver is Array[String] for class def Array[:a], then
    // this function returns :a
    if (receiver) {
        return canonical(in, receiver->clazz()->type(), receiver);
    } else {
        return in;
    }
}
