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

#include "BasicBlockGenerator.hpp"
#include <cassert>
#include <algorithm>
#include <stdint.h>

using namespace std;

BasicBlockGenerator::BasicBlockGenerator(Environment* env, Machine* mach) :
    env_(env),
    machine_(mach),
    label_(0) {

}

void BasicBlockGenerator::operator()(File* file) {
    if (env_->errors()) {
        return;
    }
    file_ = file;
    for (Feature::Ptr f = env_->modules(); f; f = f->next()) {
        f(this);
    }    
    file_ = 0;
}

void BasicBlockGenerator::operator()(Class* feature) {
    // Output intermediate-level code for the class given by 'feature'.  
    // This function also sets up the vtable for the class.
    if (feature->location().file != file_) {
        return;
    }
    class_ = feature;
    calculate_size(feature);
    enter_scope();
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    exit_scope();
    dispatch_table(feature);
    class_ = 0;
}

void BasicBlockGenerator::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockGenerator::operator()(Formal* formal) {
    
}

void BasicBlockGenerator::operator()(StringLiteral* expr) {
    // Load a pointer to the string from the string table.  Strings must
    // always be loaded first, since they are specified by address.
    return_ = load(expr);
    //refcount_inc(return_);
    //object_temp_.push_back(return_);
}

void BasicBlockGenerator::operator()(NilLiteral* expr) {
    return_ = load(expr);
}

void BasicBlockGenerator::operator()(IntegerLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = load(expr);
}

void BasicBlockGenerator::operator()(FloatLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = load(expr);
}

void BasicBlockGenerator::operator()(BooleanLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = load(expr);
}

void BasicBlockGenerator::operator()(Box* expr) {
    // Boxes a value type into an object, by allocating memory on the stack
    // for the value type.  This function allocates and initializes a refcount
    // and vtable pointer for the value type.
    Operand arg = emit(expr->child()); 
    assert(expr->type()->is_primitive() && "Not implemented for values");

    Class::Ptr clazz = expr->type()->clazz();
    calculate_size(clazz);
    String::Ptr one = env_->integer("1");
    String::Ptr size = env_->integer(stringify(clazz->size()));
    push_arg(1, load(new IntegerLiteral(Location(), one)));
    push_arg(0, load(new IntegerLiteral(Location(), size)));
    call(env_->name("calloc"));        

    return_ = pop_ret();
    object_temp_.push_back(return_);

    // Initialize the vtable pointer
    Operand vtable = Operand::addr(return_.temp(), 0);
    Operand label = load(env_->name(clazz->label()->string()+"__vtable"));
    store(vtable, label);
    // FixMe: Eventually, it should be possible to call virtual functions
    // on boxed primitives.  That, however, would require generating special
    // versions of all the basic operators, or adding thunks.  For now,
    // disallow this.
    //String::Ptr zero = env_->integer("0");
    //store(vtable, load(new IntegerLiteral(Location(), zero)));
    
    // Make sure that the refcount starts out at 1, otherwise the object may
    // be freed before the end of the constructor.
    Operand refcount = Operand::addr(return_.temp(), 1);
    store(refcount, new IntegerLiteral(Location(), one));

    // Slot 2 is the value slot for primitive types
    Operand addr = Operand::addr(return_.temp(), 2);
    store(addr, arg); 
    // FixMe: For value types, can't do a simple move
}

void BasicBlockGenerator::operator()(Cast* expr) {
    // Check to make sure that the vtable of 'arg' and 'type' match.  If they
    // don't then return the nil pointer.
    Operand arg = emit(expr->child());
    if (expr->type()->is_any_type()) {
        return_ = arg;
        return;
    }
    Class::Ptr clazz = expr->type()->clazz();
    Operand vtable1 = Operand::addr(arg.temp(), 0);
    Operand vtable2 = load(env_->name(clazz->label()->string()+"__vtable"));
    return_ = temp_++;

    BasicBlock::Ptr mismatch_block = basic_block();
    BasicBlock::Ptr ok_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    // If the vtable pointers are not equal, set the register to zero
    emit(mismatch_block);
    String::Ptr zero = env_->integer("0");
    Location loc;
    block_->instr(MOV, return_, load(new IntegerLiteral(loc, zero)), 0);
    jump(done_block);

    // Otherwise, keep the same value in the result.
    emit(ok_block);
    if (clazz->is_object()) {
        block_->instr(MOV, return_, arg, 0);
    } else if (clazz->is_primitive()) {
        // Slot 2 is the value slot for primitive types
        Operand addr = Operand::addr(arg.temp(), 2);
        block_->instr(MOV, return_, addr, 0);
    } else if (clazz->is_value()) {
        assert(!"Not implemented");
    }
    emit(done_block);
}

void BasicBlockGenerator::operator()(Is* expr) {
    // Emit code to check whether the expression type is equal to the cast
    // type, and then return true/false depending
    Operand arg = emit(expr->child());

    Class::Ptr clazz = expr->check_type()->clazz();
    Operand vtable1 = Operand::addr(arg.temp(), 0);
    Operand vtable2 = load(env_->name(clazz->label()->string()+"__vtable"));
    return_ = temp_++;

    BasicBlock::Ptr mismatch_block = basic_block();
    BasicBlock::Ptr ok_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    Location loc;

    // If the vtable pointers are not equal, set the register to zero
    emit(mismatch_block);
    String::Ptr zero = env_->integer("0");
    block_->instr(MOV, return_, load(new BooleanLiteral(loc, zero)), 0);
    jump(done_block);

    // Otherwise, set the register to 1.
    emit(ok_block);
    String::Ptr one = env_->integer("1");
    block_->instr(MOV, return_, load(new BooleanLiteral(loc, one)), 0);
    emit(done_block);
}

void BasicBlockGenerator::operator()(Binary* expr) {
    // Emit the left and right exprs, then perform the operation on
    // the two operands, and return the result.

    if (env_->name("and") == expr->operation()) {
        BasicBlock::Ptr left_true = basic_block();
        
        // Don't use the 'real' true branch; on true we want to emit the    
        // test for the second side of the and
        Operand left = emit(expr->left(), left_true, false_, false);
        if (!block_->is_terminated()) {
            if (invert_guard_) {
                bnz(left, false_, left_true);
            } else {
                bz(left, false_, left_true);
            }
        }
        invert_guard_ = false;
        emit(left_true); 

        return_ = emit(expr->right(), true_, false_, invert_branch_);
        
    } else if (env_->name("or") == expr->operation()) {
        BasicBlock::Ptr left_false = basic_block();
        
        // Don't use the 'real' false branch; on false we want to emit
        // the test for the second side of the or
        Operand left = emit(expr->left(), true_, left_false, true);
        if (!block_->is_terminated()) {
            if (invert_guard_) {
                bz(left, true_, left_false);
            } else {
                bnz(left, true_, left_false);
            }
        }
        invert_guard_ = false;
        emit(left_false);
         
        return_ = emit(expr->right(), true_, false_, invert_branch_);

    } else {
        assert(!"Unsupported binary operation");
    }
}

void BasicBlockGenerator::operator()(Unary* expr) {
    // Emit the code for the expression, then perform the operation on the
    // operand and return the result.

    if (env_->name("not")) {
        // Swap the false and true branches while calling the child, since 
        // the 'not' inverts the conditional 
        Operand op = emit(expr->child(), false_, true_, !invert_branch_);
        invert_guard_ = !invert_guard_;
    } else {
        assert(!"Unsupported unary operation");
    }
}

void BasicBlockGenerator::operator()(Member* expr) {
    // A stand-alone member operator, which means that we indirectly call the
    // getter.
    call(expr->function(), expr->expression());
}

void BasicBlockGenerator::operator()(Call* expr) {
    // The call expression may actually be an operator, if the type of the
    // expression is primitive.
    if (expr->arguments()) {
        Type::Ptr type = expr->arguments()->type();
        String::Ptr id = expr->function()->name();
        if (id->string()[0] == '@' && type->is_primitive()) {
            native_operator(expr);
            return; 
        }
    }

    call(expr->function(), expr->arguments());
}

void BasicBlockGenerator::operator()(Closure* expr) {
    // Just emit the construct expression for the closure.
    Construct::Ptr construct = expr->construct();
    emit(construct);
}

void BasicBlockGenerator::operator()(Construct* expr) {
    // Look up the function by name in the current context.
    String::Ptr id = env_->name("@init");
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->function(id);

    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    vector<Operand> args;
    Formal::Ptr formal = func->formals();
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        Type::Ptr ft = formal->type();
        if (a->type()->is_bool()) {
            args.push_back(bool_expr(a));
        } else {
            args.push_back(emit(a));
        }
        formal = formal->next();
    }
    for (int i = args.size()-1; i >= 0; i--) {
        push_arg(i, args[i]);
    }

    // Insert a call expression, then pop the return value off the stack.
    call(func->label());
    return_ = pop_ret();
    if (!expr->type()->is_value()) {
        object_temp_.push_back(return_);
    }
}

void BasicBlockGenerator::operator()(Constant* expr) {
    // Nothing here, because constants get output in the scope of the main
    // block.
}

void BasicBlockGenerator::operator()(ConstantIdentifier* expr) {
    Constant::Ptr cn = expr->constant();
    Operand loc(cn->label());
    loc.indirect(true);
    return_ = load(loc);
}

void BasicBlockGenerator::operator()(Identifier* expr) {
    // Simply look up the value of the variable as stored previously.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    if (var) {
        return_ = var->operand();
    } else if (attr) {
        Operand self = variable(env_->name("self"))->operand();
        return_ = load(Operand::addr(self.temp(), attr->slot()));
    } else {
        // Variable can't be found in a temporary; it must be an argument 
        // passed on the stack.

        int offset = stack(id);
        assert(offset);
        return_ = load(Operand::addr(offset)); 
        // A load operand of zero means the variable must be loaded relative
        // to the base pointer.

        variable(new Variable(id, return_, 0));
    }
}

void BasicBlockGenerator::operator()(Empty* expr) {
    // Do nothing
    return_ = load(env_->integer("0"));
}

void BasicBlockGenerator::operator()(Block* statement) {
    enter_scope();
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Simple* statement) {
    Expression::Ptr expr = statement->expression();
    expr(this);
    free_temps();
}

void BasicBlockGenerator::operator()(Let* statement) {
    // Enter a new scope, then emit code for initializing all the let
    // variables, and initialize code for the body.
    enter_scope();
    for (Expression::Ptr v = statement->variables(); v; v = v->next()) {
        emit(v);
        free_temps();
    } 
    emit(statement->block());
    exit_scope();
}

void BasicBlockGenerator::operator()(While* statement) {
    // Emit the guard expression in a new basic block
    BasicBlock::Ptr body_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    BasicBlock::Ptr guard_block;
    if (block_->instrs()) {
        guard_block = basic_block();
        emit(guard_block);
    } else {
        guard_block = block_;
    }

    // Recursively emit the boolean guard expression.  
    invert_guard_ = false;
    Operand guard = emit(statement->guard(), body_block, done_block, false);
    free_temps();
    if (!block_->is_terminated()) {
        if (invert_guard_) {
            bnz(guard, done_block, body_block);
        } else {
            bz(guard, done_block, body_block);
        }
    }

    // Now emit the body of the loop
    emit(body_block);
    emit(statement->block());

    // Jump back up to re-evaluate the guard expression
    jump(guard_block);
    emit(done_block);    
}

void BasicBlockGenerator::operator()(Conditional* statement) {
    // Emit a conditional if/then expression with boolean guard expr.
    BasicBlock::Ptr then_block = basic_block();
    BasicBlock::Ptr else_block;
    BasicBlock::Ptr done_block = basic_block();
    if (statement->false_branch()) {
        else_block = basic_block();
    } else {
        else_block = done_block;
    }

    // Recursively emit the boolean guard expression.  We need to pass the true
    // and the false block pointers so that the correct code is emitted on each
    // branch.
    invert_guard_ = false;
    Operand guard = emit(statement->guard(), then_block, else_block, false);
    free_temps();
    if (!block_->is_terminated()) {
        if (invert_guard_) {
            bnz(guard, else_block, then_block);
        } else {
            bz(guard, else_block, then_block);
        }
    }

    // Now emit the true branch of the conditional
    emit(then_block);
    emit(statement->true_branch());

    // Emit the false branch if it exists; make sure to insert a jump before
    // emitting the false branch
    if (statement->false_branch()) {
        if (!block_->is_terminated()) {
            jump(done_block);
        }
        emit(else_block);
        emit(statement->false_branch());
        // jump(done_block);
    }
    emit(done_block);
}

void BasicBlockGenerator::operator()(Assignment* expr) {
    // FixMe: This breaks SSA form, because variables are not renamed.  With
    // this code the way it is, the generator only generates SSA for temporary
    // expressions.  This limitation is here as a punt on introducing
    // phi-functions until optimizations are needed, since without
    // optimizations, SSA is not needed anyway.

    Expression::Ptr init = expr->initializer();
    if (dynamic_cast<Empty*>(init.pointer())) {
        return_ = env_->integer("0");
    } else if (init->type()->is_bool()) {
        return_ = bool_expr(init.pointer());
    } else {
        return_ = emit(init);
    }

    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;

    if (var) {
        // Assignment to a local var that has already been initialized once in
        // the current scope.
        Type::Ptr type = var->type();
        if (!type->is_value()) {
            refcount_dec(var->operand());
        }
        block_->instr(MOV, var->operand(), return_, 0);
        if (!type->is_value()) {
            refcount_inc(var->operand());
        }
    } else if (attr) {
        // Assignment to an attribute within a class
        Type::Ptr type = expr->type(); 
        Variable::Ptr self = variable(env_->name("self"));
        Operand addr = Operand::addr(self->operand().temp(), attr->slot());  
        Operand old = load(addr);
        if (!type->is_value() && !attr->is_weak()) {
            refcount_dec(old);
        } 
        store(addr, return_);
        if (!type->is_value() && !attr->is_weak()) {
            refcount_inc(return_);
        }
    } else {
        // Assignment to a local var that has not yet been initialized in the
        // current scope.
        Type::Ptr declared = expr->declared_type();
        if (!declared) {
            declared = expr->type();
        }
        //Variable::Ptr var = new Variable(id, mov(return_), declared);
        Variable::Ptr var = new Variable(id, return_, declared);
        variable(var);
        if (!declared->is_value()) {
            refcount_inc(return_);
        }
    }
}

void BasicBlockGenerator::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    if (!dynamic_cast<Empty*>(expr.pointer())) {
        // Don't actually return right away; store the result in a pseudo-var
        // and return it after cleanup.
        Operand ret;
        if (expr->type()->is_bool()) {
            ret = bool_expr(expr);
        } else {
            ret = emit(expr);
        }
        scope_.back()->return_val(ret);

        // Increment the refcount of the returned value if it is an object.
        // The refcount must be incremented so that the object won't be freed
        // before the function returns.  It is the caller's responsibility to
        // correctly free the returned object.  Note: for a constructor, the
        // refcount is simply already initialized to 1.  This code isn't called
        // because a constructor doesn't actually have a 'return' for 'self'.
        if (!expr->type()->is_value()) {
            refcount_inc(ret);
        }
    }
    scope_.back()->has_return(true);
    free_temps();
}

void BasicBlockGenerator::operator()(Match* statement) {
    // Emit one conditional for each branch in the match statement.
    Operand guard = emit(statement->guard());
    BasicBlock::Ptr done_block = basic_block();  
    BasicBlock::Ptr next_block = basic_block();
    free_temps();

    for (Statement::Ptr t = statement->cases(); t; t = t->next()) {
        emit(next_block);

        Case::Ptr branch = static_cast<Case*>(t.pointer());
        BasicBlock::Ptr true_block = basic_block();
        if (t->next()) {
            next_block = basic_block();
        } else {
            next_block = done_block;
        }
        
        // Emit the guard expression for this branch.
        Operand result = emit(branch->guard());  
        free_temps();
        bne(guard, result, next_block, true_block);
        emit(true_block);

        // Now emit the statements to be run if this branch is true, then jump
        // to the end of the case statement.
        for (Statement::Ptr s = branch->children(); s; s = s->next()) {
            s(this);
        } 
        jump(done_block); 
    }

    emit(done_block);
    free_temps();
}

void BasicBlockGenerator::operator()(Case* statement) {
    // Pass, handled by match
}

void BasicBlockGenerator::operator()(Fork* statement) {
    assert(!"Not implemented");
    free_temps();
}

void BasicBlockGenerator::operator()(Yield* statament) {
//    free_temps();
    call(env_->name("Coroutine__yield")); 
	exception_catch();
}

void BasicBlockGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) {
        return;
    }
    if (feature->location().file != file_) {
        return;
    }

    // Reset the temporaries for the function.
    temp_ = 0;
    function_ = feature;
    block_ = 0;
    emit(basic_block());
    stack_.clear();
    enter_scope();

    // Pop the formal parameters off the stack in normal order, and save them
    // to a temporary.
    int index = 0;
    for (Formal* f = feature->formals(); f; f = f->next()) {
        if (index >= machine_->arg_regs()) {
            // Variable is passed on the stack
            stack(f->name(), stack_.size()+1); 
        } else {
            // Variable is passed by register; precolor the temporary for this
            // formal parameter by using a negative number.  Passing NULL for
			// the type of the variable prevents it from being garbage collected
			// at the end of the scope (which is what we want for function 
			// parameters).
            int reg = -machine_->arg_reg(index)->id();
            variable(new Variable(f->name(), mov(reg), 0));

            // On Windows, the value is also passed with a backing location on
            // the stack.
#ifdef WINDOWS
            stack(f->name(), stack_.size()+1);
#endif
        }
        index++;
    } 

    // Set up the 'self' variable with the constructor, if necessary; this 
    // allocates the memory for the object using calloc so that all of the
    // fields are initialized to zero.
    if (feature->is_constructor()) {
        ctor_preamble(class_);
    }

    // If this is main(), then emit the code to load constants.
    String::Ptr name = feature->name();
    Feature::Ptr parent = feature->parent();
    if (name->string() == "main" && parent->name()->string() == "") {
        constants();
    }

    // Generate code for the body of the function.
    emit(feature->block());
    if (feature->type()->is_void() || function_->is_constructor()) {
        func_return();
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Attribute* feature) {
    // Pass, handled by constructor
}

void BasicBlockGenerator::operator()(Import* feature) {
    // Pass
}

void BasicBlockGenerator::operator()(Type* feature) {
    // Pass
}

void BasicBlockGenerator::call(Function* func, Expression* args) {
    // Push objects in anticipation of the call instruction.  Arguments must be
    // pushed in reverse order.
    vector<Operand> val;
    Formal::Ptr formal = func->formals();
    for (Expression::Ptr a = args; a; a = a->next()) {
        Type::Ptr ft = formal->type();
        if (a->type()->is_bool()) {
            val.push_back(bool_expr(a));
        } else {
            val.push_back(emit(a));
        }
        formal = formal->next();
    }

    Operand fnptr;
    if (func->is_virtual()) {
        // Dynamic dispatch: call the object dispatch function with the
        // receiver and function name as arguments.
        
        String::Ptr name = env_->string(func->name()->string());
        push_arg(1, load(new StringLiteral(Location(), name)));
        push_arg(0, val[0]); // Receiver
        call(env_->name("Object__dispatch"));
        fnptr = pop_ret();
    } 

    for (int i = val.size()-1; i >= 0; i--) {
        push_arg(i, val[i]);
    }

    if (func->is_virtual()) {
        // Dynamic dispatch: call the function pointer.
        call(fnptr);
    } else {
        // Static dispatch: insert a call expression, then pop the return value
        // off the stack.
        call(func->label());
    }

    if (!func->type()->is_void()) {
        return_ = pop_ret();
        if (!func->type()->is_value()) {
            object_temp_.push_back(return_);
        }
    } else {
        return_ = 0;
    }
	if (func->throw_spec() == Function::THROW) {
		exception_catch();
	}
}

void BasicBlockGenerator::exception_catch() {
	// If the function can throw an exception, then generate the landing pad and
	// return.  Currently, only an exception throw is allowed, to support cleaning
	// up coroutines.
	String::Ptr name = env_->name("Exception__current");
	static Constant::Ptr current(new Constant(Location(), env_, name, 0, 0));

	function_->file()->dependency(current);
	Operand loc(name);
	loc.indirect(true);
	Operand val = load(loc);

	// Branch to the continuation if there was no exception.
	BasicBlock::Ptr except_block = basic_block();
	BasicBlock::Ptr done_block = basic_block(); 
	bz(val, done_block, except_block);

	// Handle the exception by returning from the function.
	emit(except_block);
	func_return();

	// Start a new code block that continues after the exception check.
	emit(done_block);
}

void BasicBlockGenerator::native_operator(Call* expr) {
    // FixMe: Replace this with a mini-parser that can read three-address-code
    // and output it as an inline function.
    string id = expr->function()->name()->string();

    vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        if (a->type()->is_bool()) {
            args.push_back(bool_expr(a));
        } else {
            args.push_back(emit(a));
        }
    }
   
    if (id == "@add") {
        assert(args.size() == 2);
        return_ = add(args[0], args[1]); 
    } else if (id == "@sub") {
        assert(args.size() == 2);
        return_ = sub(args[0], args[1]);
    } else if (id == "@mul") {
        assert(args.size() == 2);
        return_ = mul(args[0], args[1]);
    } else if (id == "@div") {
        assert(args.size() == 2);
        return_ = div(args[0], args[1]);
    } else if (id == "@neg") {
        assert(args.size() == 1);
        return_ = neg(args[0]);
    } else if (id == "@mod") {
        assert(!"Not implemented");
    } else if (id == "@compl") {
        assert(!"Not implemented");
    } else if (id == "@equal") {
        if (invert_branch_) {
            be(args[0], args[1], true_, false_);
        } else {
            bne(args[0], args[1], false_, true_);
        }
    } else if (id == "@less") {
        if (invert_branch_) {
            bl(args[0], args[1], true_, false_);
        } else {
            bge(args[0], args[1], false_, true_);
        } 
    }
}

BasicBlock* BasicBlockGenerator::basic_block() {
    BasicBlock* block = new BasicBlock();
    block->label(env_->name(".l" + stringify(++label_)));
    return block;
}


Variable* BasicBlockGenerator::variable(String* name) {
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

int BasicBlockGenerator::stack(String* name) {
    map<String::Ptr, int>::iterator i = stack_.find(name);
    if (i != stack_.end()) {
        return i->second;
    } else {
        return 0;
    }
}

void BasicBlockGenerator::variable(Variable* var) {
    assert(scope_.size());
    scope_.back()->variable(var);
}

void BasicBlockGenerator::stack(String* name, int offset) {
    stack_.insert(make_pair(name, offset));
}

void BasicBlockGenerator::enter_scope() {
    scope_.push_back(new Scope);
}

void BasicBlockGenerator::exit_scope() {
    // Pops the symbol table for this scope off the stack, and inserts code
    // to perform cleanup at the end of the scope.
    Scope::Ptr scope = scope_.back();

    if (!scope->has_return()) {    
		// Remove variables in reverse order!
		for (int i = scope->variables()-1; i >= 0; i--) { 
			scope_cleanup(scope->variable(i));
		}
        scope_.pop_back();
        return;
    }

    func_return();
    scope_.pop_back();
}

Operand BasicBlockGenerator::bool_expr(Expression* expression) {
    // Emits a boolean expression with short-circuiting, and stores the result
    // in a fixed operand.  Note:  This breaks SSA form, because a value gets
    // assigned different values on different branches.
    if (dynamic_cast<BooleanLiteral*>(expression)) {
        return_ = emit(expression);
        return return_;
    }

    BasicBlock::Ptr then_block = basic_block();
    BasicBlock::Ptr else_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();
    return_ = ++temp_;

    // Recursively emit the boolean guard expression.
    invert_guard_ = false;
    Operand guard = emit(expression, then_block, else_block, false);
    free_temps();
    if (!block_->is_terminated()) {
        if (invert_guard_) {
            bnz(guard, else_block, then_block);
        } else {
            bz(guard, else_block, then_block);
        }
    }

    Location loc;

    // Now emit the code for the 'true' branch, i.e., assign 'true' to the
    // return value.
    emit(then_block);
    String::Ptr one = env_->integer("1");
    block_->instr(MOV, return_, load(new IntegerLiteral(loc, one)), 0);
    jump(done_block);
    
    // Now emit the code for the 'false' branch.
    emit(else_block);
    String::Ptr zero = env_->integer("0");
    block_->instr(MOV, return_, load(new IntegerLiteral(loc, zero)), 0);
    
    emit(done_block);

    return return_;
}

void BasicBlockGenerator::scope_cleanup(Variable* var) {
    // Emits the code to clean up the stack when exiting block.  This includes 
    // decrementing reference counts, and calling destructors for value types.
    Type::Ptr type = var->type();
    if (type && !type->is_primitive()) {
        if (type->is_value()) {
            cerr << type->name()->string() << endl;
            assert(!"Need to figure out how to do value types");
            // Call destructor!
        } else {
            // Emit a branch to check the variable's reference count and
            // free it if necessary.
            refcount_dec(var->operand());               
        }
    }
}

void BasicBlockGenerator::func_return() {
    // Search backwards through the vector and clean up variables in the 
    // containing scope.  Do NOT clean up variables in the top scope; those
	// variables are parameters and they are anchored (hence j > 1).
    for (int j = scope_.size()-1; j > 0; j--) {
        Scope::Ptr s = scope_[j];
        for (int i = s->variables()-1; i >= 0; i--) {
            scope_cleanup(s->variable(i));
        }
    }

    // Emit an actual return.  Emit code to return the value saved in the var
    // '_ret' if the variable has been set.
    Operand retval = scope_.back()->return_val();
    if (function_->is_constructor()) {
        retval = variable(env_->name("self"))->operand();
    }
    if (!scope_.empty() && retval.temp()) {
        if (machine_->return_regs()) {
            // Return the value by register, if the architecture supports return
            // by register
            int index = 0;
            int reg = -machine_->return_reg(index)->id();
            block_->instr(MOV, reg, retval, 0); 
        } else {
            // Otherwise, return on the stack.
            push(retval);
        }
    }
    
    // If this is the destructor, then release all the pointers held by the
    // object, and then call free() to release memory if the object was
    // dynamically allocated.
    if (function_->is_destructor()) {
        dtor_epilog(function_);
    }
    
    ret();
}

void BasicBlockGenerator::refcount_inc(Operand var) {
    // Emit code to increment the reference count for the object specified by
    // 'temp'.  Insert a call expression to call the refcount_dec function.  If
    // the variable is already in the temp list, then the current function has
    // already incremented the refcount for that object -- so just remove it
    // from the temp list instead of calling refcount_inc.
    for (int i = 0; i < object_temp_.size(); i++) {
        if (object_temp_[i].temp() == var.temp()) { 
            std::swap(object_temp_[i], object_temp_.back());
            object_temp_.pop_back();
            return;
        }
    }

    push_arg(0, var);
    call(env_->name("Object__refcount_inc"));
}

void BasicBlockGenerator::refcount_dec(Operand var) {
    // Emit code to decrement the reference count for the object specified by
    // 'temp'.  Insert a call expression to call the refcount_dec function 
    push_arg(0, var);
    call(env_->name("Object__refcount_dec"));
}

void BasicBlockGenerator::push_arg(int i, Operand op) {
    if (i >= machine_->arg_regs()) {
        // Argument is pushed on the stack
        push(op);
    } else {
        // Argument is passed by register
        block_->instr(MOV, -machine_->arg_reg(i)->id(), op, 0);

        // On Windows, the argument is also passed by the stack as a backing
        // store
#ifdef WINDOWS
        push(op);
#endif
    }        
}

Operand BasicBlockGenerator::pop_ret() {
    if (0 >= machine_->return_regs()) {
        return pop();
    } else {
        int val = 0;
        return mov(-machine_->return_reg(val)->id());
    }
}

uint64_t fnv_hash(uint64_t hash, String* str) {
    const string& name = str->string();
    for (int i = 0; i < name.length(); ++i) {
        hash = ((hash * 0x01000193) ^ name[i]);
    }
    return hash;
}

typedef vector<Function::Ptr> JumpBucket;
struct SortJumpBuckets {
    bool operator()(const JumpBucket& a, const JumpBucket& b) {
        return a.size() > b.size();
    }
};

void BasicBlockGenerator::dispatch_table(Class* feature) {
    // Generate the Pearson perfect hash that will be used for vtable lookups.
    if (!feature->is_object()) {
        return;
    }
    
    // Initially set the size of the hash table to the number of functions.
    // The size of the table will grow to prevent collisions.
    int n = 0;
    for (Feature* f = feature->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f)) {
            if (!func->is_constructor() && !func->is_destructor()) {
                n++;
            }
        }
    }
    for (Type* m = feature->mixins(); m; m = m->next()) {
        Class* clazz = m->clazz();   
        for (Feature* f = clazz->features(); f; f = f->next()) {
            if (Function* func = dynamic_cast<Function*>(f)) {
                // Ignore functions that have been overidden
                if (feature->function(func->name()) == func) {
                    n++;
                }
            }
        }
    }
    if (!n) { return; }
    n = max(10, n * 2);

    File* file = feature->file();

    // Step 1: Place all keys into buckets using a simple hash.  There will
    // be collisions, but they will be resolved in steps 2-3.
    vector<JumpBucket> bucket(n);
    for (Feature* f = feature->features(); f; f = f->next()) {
        if (Function* func = dynamic_cast<Function*>(f)) {
            if (!func->is_constructor() && !func->is_destructor()) {
                uint64_t hash = fnv_hash(0, func->name()) % n;
                bucket[hash].push_back(func);
            }
        }
    }
    for (Type* m = feature->mixins(); m; m = m->next()) {
        Class* clazz = m->clazz();   
        for (Feature* f = clazz->features(); f; f = f->next()) {
            if (Function* func = dynamic_cast<Function*>(f)) {
                // Ignore functions that have been overidden
                if (feature->function(func->name()) == func) {
                    uint64_t hash = fnv_hash(0, func->name()) % n;
                    bucket[hash].push_back(func);
                    if (func->file() != file) {
                        file->dependency(func);
                    }
                }
            }
        }
    }

    // Step 2: Sort buckets and process the ones with the most items first.
    sort(bucket.begin(), bucket.end(), SortJumpBuckets()); 
    
    // Step 3: Attempt to place items in the buckets into empty slots in the
    // second jump table, starting with the largest buckets first.
    vector<Function*> value(n);
    vector<Function*> slots;
    feature->jump1(n-1, 0);
    for (int i = 0; i < bucket.size(); i++) {
        if (bucket[i].size() <= 0) { break; }
        int d = 1; 
    retry:
        // Try to place all the values into empty value slots in the second
        // jump table.  If that doesn't work, then increment the hash mixing
        // value "d"
        slots = value;
        for (int j = 0; j < bucket[i].size(); j++) {
            uint64_t hash = fnv_hash(d, bucket[i][j]->name()) % n;
            if (slots[hash]) {
                d++;
                goto retry;
            } else {
                slots[hash] = bucket[i][j];
            }
        }

        // Success! Record the d-value (i.e., hash mixing value) for the 
        // current bucket, and continue.
        uint64_t hash = fnv_hash(0, bucket[i][0]->name()) % n;
        feature->jump1(hash, d);
        value = slots;
    }
    for (int i = 0; i < value.size(); i++) {
        feature->jump2(i, value[i]);
    }
/*
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (func->is_destructor() || func->is_constructor()) { 
                continue; 
            }
            std::cout << func->name()->string() << std::endl;
            uint64_t hash1 = fnv_hash(0, func->name());
            uint64_t d = feature->jump1(hash1 % n);
            uint64_t hash2 = fnv_hash(d, func->name());
            std::cout << "   n: " << n << std::endl;
            std::cout << "   hash1 %n: " << hash1 % n << std::endl;
            std::cout << "   hash1: " << hash1 << std::endl;
            std::cout << "   d: " << d << std::endl;
            std::cout << "   hash2: " << hash2 << std::endl;
        }
    }
    std::cout << feature->jump1s() << std::endl;
    std::cout << feature->jump2s() << std::endl;
*/
}

void BasicBlockGenerator::calculate_size(Class* feature) {
    // Calculate the memory footprint of the given class.
    if (feature->size()) { return; }

    int size = 2 * machine_->word_size(); // Size of refcount + vtable pointer
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Type::Ptr type = attr->type();
            if (type->is_value() && !type->is_primitive()) {
                // FIXME: Handle complex value types larger than an Int
                assert(!"Value types not supported");    
            } 
            size += machine_->word_size();
        }
    }
    if (feature->is_primitive()) {
        size += machine_->word_size();
    }

    feature->size(size);
}

void BasicBlockGenerator::ctor_preamble(Class* clazz) {
    // Emits the memory alloc/vtable setup for the class.

    // Allocate the memory for the new object by calling calloc with the size
    // of the object.
    Operand self;
    if (clazz->is_object()) {
        String::Ptr one = env_->integer("1");
        String::Ptr size = env_->integer(stringify(clazz->size()));
        push_arg(1, load(new IntegerLiteral(Location(), one)));
        push_arg(0, load(new IntegerLiteral(Location(), size)));
        call(env_->name("calloc"));        
 
        // Obtain a pointer to the 'self' object, and store it in the 'self'
        // variable.
        self = pop_ret(); 
        variable(new Variable(env_->name("self"), self, 0)); 
       
        // Initialize the vtable pointer
        Operand vtable = Operand::addr(self.temp(), 0);
        Operand label = load(env_->name(clazz->label()->string()+"__vtable"));
        store(vtable, label);
        
        // Make sure that the refcount starts out at 1, otherwise the object may
        // be freed before the end of the constructor.
        Operand refcount = Operand::addr(self.temp(), 1);
        store(refcount, new IntegerLiteral(Location(), one));
    
    } else {
        // FIXME: Implement ctor for value types; need to pass in the 'self'
        // pointer for value type ctors.
        assert(!"Not implemented");
    }

    // Emit initializer code for initialized attributes
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Expression::Ptr init = attr->initializer();
            if (!init) {
                continue;
            }
            if (dynamic_cast<Empty*>(init.pointer())) {
                continue;
            }
            Operand value = emit(init);
            Operand addr = Operand::addr(self.temp(), attr->slot());
            store(addr, value);
            if (!init->type()->is_value()) {
                refcount_inc(value);
            } else if (!init->type()->is_primitive()) {
                // FixMe: for value types, can't do a simple move
                assert("Not implemented");
            }
            free_temps();
        }
    }
}

void BasicBlockGenerator::dtor_epilog(Function* feature) {
    // Free all of the attributes, and call destructors.
    std::vector<Attribute::Ptr> attrs;
    for (Feature::Ptr f = class_->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            if (!attr->type()->is_value() && !attr->is_weak()) {
                attrs.push_back(attr);
            }
        }
    } 

    // The attributes need to be released in the reverse order
    for (int i = attrs.size()-1; i >= 0; i--) {
        Operand self = variable(env_->name("self"))->operand();
        Operand val = load(Operand::addr(self.temp(), attrs[i]->slot()));
        refcount_dec(val);
    }

    if (class_->is_object()) {
        push_arg(0, variable(env_->name("self"))->operand());
        call(env_->name("free"));
    }
}

void BasicBlockGenerator::free_temps() {
    // Free all object temporaries that were used to evaluate the expression by
    // decrementing their refcount.
    for (int i = 0; i < object_temp_.size(); i++) {
        refcount_dec(object_temp_[i]);
    }
    object_temp_.clear();
}

void BasicBlockGenerator::constants() {
    for (int i = 0; i < env_->constants(); i++) {
        Constant::Ptr cons = env_->constant(i);
        Operand loc(cons->label());
        loc.indirect(true); 
        store(loc, emit(cons->initializer()));
        free_temps();
        function_->file()->dependency(cons);
    }
}
