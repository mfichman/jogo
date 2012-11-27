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
    local_slots_(0),
    arg_slots_(0),
    invert_branch_(false),
    invert_guard_(false),
    temp_(0),
    label_(0) {
}

void BasicBlockGenerator::operator()(File* file) {
    if (env_->errors()) {
        return;
    }

    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this); 
    }
}

void BasicBlockGenerator::operator()(Class* feature) {
    // Output intermediate-level code for the class given by 'feature'.  
    // This function also sets up the vtable for the class.
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
    assert(!expr->type()->is_compound() && "Not implemented for values");
    assert(!expr->type()->is_ref());

    Class::Ptr clazz = expr->type()->clazz();
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr one = env_->integer("1");
    String::Ptr size = env_->integer(stringify(bytes));
    FuncMarshal fm(this);
    fm.arg(load(new IntegerLiteral(Location(), size)));
    fm.arg(load(new IntegerLiteral(Location(), one)));
    fm.call(env_->name("Boot_calloc"));
    return_ = pop_ret(expr->type());
    object_temp_.push_back(return_);

    // Initialize the vtable pointer
    Operand vtable = Operand(return_.reg(), Address(0));
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
    Operand refcount = Operand(return_.reg(), Address(1));
    store(refcount, new IntegerLiteral(Location(), one));

    // Slot 2 is the value slot for primitive types
    Operand addr = Operand(return_.reg(), Address(2));
    store(addr, arg); 
    // FixMe: For value types, can't do a simple move
}

void BasicBlockGenerator::operator()(Cast* expr) {
    // Check to make sure that the vtable of 'arg' and 'type' match.  If they
    // don't then return the nil pointer.
    Operand arg = emit(expr->child());
    if (expr->type()->is_any()) {
        return_ = arg;
        return;
    }
    Class::Ptr clazz = expr->type()->clazz();
    Operand vtable1 = Operand(arg.reg(), Address(0));
    Operand vtable2 = load(env_->name(clazz->label()->string()+"__vtable"));
    return_ = RegisterId(temp_++, 0);

    BasicBlock::Ptr mismatch_block = basic_block();
    BasicBlock::Ptr ok_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    // If the vtable pointers are not equal, set the register to zero
    emit(mismatch_block);
    String::Ptr zero = env_->integer("0");
    Location loc;
    block_->instr(MOV, return_, load(new IntegerLiteral(loc, zero)), Operand());
    jump(done_block);

    // Otherwise, keep the same value in the result.
    emit(ok_block);
    if (clazz->is_primitive()) {
        // Slot 2 is the value slot for primitive types
        Operand addr = Operand(arg.reg(), Address(2));
        block_->instr(MOV, return_, addr, Operand());
    } else if (clazz->is_ref()) {
        block_->instr(MOV, return_, arg, Operand());
    } else if (clazz->is_compound()) {
        assert(!"Not implemented");
    } else {
        assert(!"Invalid type");
    }
    emit(done_block);
}

void BasicBlockGenerator::operator()(Is* expr) {
    // Emit code to check whether the expression type is equal to the cast
    // type, and then return true/false depending
    Operand arg = emit(expr->child());

    Class::Ptr clazz = expr->check_type()->clazz();
    Operand vtable1 = Operand(arg.reg(), Address(0));
    Operand vtable2 = load(env_->name(clazz->label()->string()+"__vtable"));
    return_ = RegisterId(temp_++, 0);

    BasicBlock::Ptr mismatch_block = basic_block();
    BasicBlock::Ptr ok_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    Location loc;

    // If the vtable pointers are not equal, set the register to zero
    emit(mismatch_block);
    String::Ptr zero = env_->integer("0");
    mov(return_, load(new BooleanLiteral(loc, zero)));
    jump(done_block);

    // Otherwise, set the register to 1.
    emit(ok_block);
    String::Ptr one = env_->integer("1");
    mov(return_, load(new BooleanLiteral(loc, one)));
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
        free_temps();
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
        free_temps();
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
    Function::Ptr func = expr->function();
    call(func, expr->expression(), expr->expression());
    if (func->type()->is_compound()) {
        expr->file()->dependency(func->type()->clazz()->destructor());
    }
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
    call(expr->function(), expr->arguments(), expr->receiver());
}

void BasicBlockGenerator::operator()(Closure* expr) {
    // Just emit the construct expression for the closure.
    Construct::Ptr construct = expr->construct();
    emit(construct);
}

void BasicBlockGenerator::operator()(Construct* expr) {
    // Look up the function by name in the current context.
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->constructor();

    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    FuncMarshal fm(this);
    Operand valret;
    if(clazz->is_compound()) {
        // Push the 'self' parameter for the Vector constructor
        valret = stack_value_temp(expr->type());
        fm.arg(valret);
    } 
    Formal::Ptr formal = func->formals();
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        Type::Ptr ft = formal->type();
        if (a->type()->is_bool()) {
            fm.arg(bool_expr(a));
        } else {
            fm.arg(emit(a));
        }
        formal = formal->next();
    }

    // Insert a call expression, then pop the return value off the stack.
    fm.call(func->label());
    if (expr->type()->is_primitive()) {
        assert(!"Primitive constructor?");
    } else if (expr->type()->is_ref()) {
        return_ = pop_ret(func->type());
        object_temp_.push_back(return_);
    } else if (expr->type()->is_compound()) {
        return_ = valret;
    } else {
        assert(!"Invalid type");    
    } 
}

void BasicBlockGenerator::operator()(Constant* expr) {
    // Nothing here, because constants get output in the scope of the main
    // block.
}

void BasicBlockGenerator::operator()(ConstantIdentifier* expr) {
    return_ = load(Operand(expr->constant()->label(), Address(0)));
}

void BasicBlockGenerator::operator()(Identifier* expr) {
    // Simply look up the value of the variable as stored previously.
    return_ = id_operand(expr->identifier());
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

    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Type::Ptr decl = expr->declared_type();
    Expression::Ptr init = expr->initializer();
    Operand assign_addr;
    bool is_secondary = var && !expr->is_let() && decl->is_top();
    bool is_attr = attr;

    if (expr->type()->is_compound()) {
        if (is_secondary) {
            value_dtor(var->operand(), expr->type()); // Delete old value 
            assign_addr = assign_addr_ = var->operand();
        } else if (is_attr) {
            // Nothing
        } else {
            assign_addr = assign_addr_ = stack_value(expr->type()); 
        }
    }

    if (dynamic_cast<Empty*>(init.pointer())) {
        String::Ptr value;
        if (expr->type()->is_enum()) {
            value = expr->type()->clazz()->default_enum_value();
        } else {
            value = env_->integer("0");
        }
        return_ = mov(new IntegerLiteral(Location(), value));
    } else if (init->type()->is_bool()) {
        return_ = bool_expr(init.pointer());
    } else {
        return_ = emit(init);
    }

    assign_addr_ = assign_addr;
    if (is_secondary) {
        secondary_assignment(expr);
    } else if (is_attr) {
        attr_assignment(expr);
    } else {
        initial_assignment(expr);
    }
    assign_addr_ = Operand();
}

void BasicBlockGenerator::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    Type::Ptr type = expr->type();
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
        if (expr->type()->is_primitive()) {
            // Pass
        } else if (expr->type()->is_ref()) {
            refcount_inc(ret);
        } else if (expr->type()->is_compound()) {
            // Copy the value into the return pointer variable.
            Operand dst = id_operand(env_->name("ret"));
            value_assign_mem(ret, dst, expr->type());
            // XCOPY: If RHS value is a temp, then move and free stack (don't copy)
            // XCOPY: ret val
        } else {
            assert(!"Invalid type");
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
        enter_scope();

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
        exit_scope();
        if (!block_->is_terminated()) {
            jump(done_block); 
        }
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
    FuncMarshal fm(this);
    fm.call(env_->name("Coroutine__yield"));
	exception_catch();
}

void BasicBlockGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) { return; }

    // Reset the temporaries for the function.
    feature->basic_block_del_all();
    temp_ = machine_->regs();
    function_ = feature;
    block_ = 0;
    emit(basic_block());
    enter_scope();

    // Pop the formal parameters off the stack in normal order, and save them
    // to a temporary.
    FuncUnmarshal um(this);
    if (feature->is_constructor() && class_->is_compound()) {
        um.arg(env_->name("self"), class_->type());
    } 
    for (Formal* f = feature->formals(); f; f = f->next()) {
        um.arg(f->name(), f->type());
    } 
    if (feature->type()->is_compound()) {
        um.arg(env_->name("ret"), feature->type());
    }

    // Set up the 'self' variable with the constructor, if necessary; this 
    // allocates the memory for the object using calloc so that all of the
    // fields are initialized to zero.
    if (feature->is_constructor()) {
        ctor_preamble(class_);
    }
    if (feature->is_copier()) {
        copier_preamble(class_);
    }

    // If this is main(), then emit the code to load constants.
    if (feature->qualified_name()->string() == "Boot::main") {
        constants();
    }

    // Generate code for the body of the function.
    emit(feature->block());
    if (feature->type()->is_void() || feature->is_constructor()) {
        func_return();
    }
    exit_scope();
    feature->temp_regs(temp_);
    assert(local_slots_ == 0 && "Invalid stack alloc");
    assert(arg_slots_ == 0 && "Invalid stack alloc");
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

void BasicBlockGenerator::call(Function* func, Expression* args, Expression* recv) {
    // Push objects in anticipation of the call instruction.  Arguments must be
    // pushed in reverse order.
    if (recv) {
        // Look up the function again, in case it needs to re-resolve
        Class::Ptr clazz = recv->type()->clazz();
        func = clazz->function(func->name());
    }

    Formal::Ptr formal = func->formals();
    FuncMarshal fm(this);
    Operand receiver;
    Operand valret;
    if (func->type()->is_compound()) {
        valret = stack_value_temp(func->type());
    }
    for (Expression::Ptr a = args; a; a = a->next()) {
        Type::Ptr ft = formal->type();
        Operand val = a->type()->is_bool() ? bool_expr(a) : emit(a);
        fm.arg(val);
        if(a == args) {
            receiver = val;
        }
        formal = formal->next();
    }
    if (func->type()->is_compound()) {
        // Push a pointer for the return value at the end of the argument list.
        // The returned value type data will be stored at that location on the
        // stack.
        fm.arg(valret);
    }

    Operand fnptr;
    if (func->is_virtual()) {
        // Dynamic dispatch: call the object dispatch function with the
        // receiver and function name as arguments.
        String::Ptr name = env_->string(func->name()->string());
        FuncMarshal fm(this);
        fm.arg(receiver); // Receiver
        fm.arg(load(new StringLiteral(Location(), name)));
        fm.call(env_->name("Object__dispatch"));
        fnptr = pop_ret(func->type());
    } 

    if (func->is_virtual()) {
        // Dynamic dispatch: call the function pointer.
        fm.call(fnptr);
    } else {
        // Static dispatch: insert a call expression, then pop the return value
        // off the stack.
        fm.call(func->label());
    }

    if (func->type()->is_void()) {
        return_ = Operand();
    } else if (func->type()->is_primitive()) {
        return_ = pop_ret(func->type()); 
    } else if (func->type()->is_compound()) {
        // Return value is allocated on the stack already, and no value is
        // returned by register.
        return_ = valret;
    } else if (func->type()->is_ref()) {
        return_ = pop_ret(func->type());
        object_temp_.push_back(return_);
    } else {
        assert(!"Invalid type");
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
	Constant::Ptr static current(new Constant(Location(), env_, name, 0, 0));

	function_->file()->dependency(current);
	Operand val = load(Operand(name, Address(0)));

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
        free_temps();
        if (invert_branch_) {
            be(args[0], args[1], true_, false_);
        } else {
            bne(args[0], args[1], false_, true_);
        }
    } else if (id == "@less") {
        free_temps();
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

void BasicBlockGenerator::variable(Variable* var) {
    assert(scope_.size());
    scope_.back()->variable(var);
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

Operand BasicBlockGenerator::stack_value(Type* type) {
    // Allocate space for a local value-type variable or temporary on the
    // stack, and return the address of the allocated space.
    calculate_size(type->clazz());
    local_slots_inc(type->clazz()->slots());
    return Address(-local_slots_,0); 
}

Operand BasicBlockGenerator::stack_value_temp(Type* type) {
    // Allocates storage for a temporary stack variable.
    Operand val;
    if (!!assign_addr_) {
        val = assign_addr_;
    } else {
        val = mov(stack_value(type));  
        Variable::Ptr var = new Variable(0, val, type);
        value_temp_.insert(std::make_pair(var->operand().reg(), var));
    }
    assign_addr_ = Operand();
    return val;
}

Operand BasicBlockGenerator::id_operand(String* id) {
    // Returns an operand for the given id that may be used in the current
    // scope.  Assert if the lookup fails.
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    if (var && !var->operand().reg()) {
        // Variable can't be found in a temporary; it must be an argument
        // passed on the stack.
        assert(!!var->operand().addr() && "No address or reg for operand");
        Operand val = load(Operand(var->operand().addr())); 
        if (var->operand().is_float()) {
            val = Operand(RegisterId(val.reg().id(), RegisterId::FLOAT));
        }
        variable(new Variable(id, val, 0));
        return val;
    } else if (var && !!var->operand().reg()) {
        return var->operand();
    } else if (attr) {
        Operand self = variable(env_->name("self"))->operand();
        if (attr->type()->is_compound()) {
            return Operand(self.reg(), Address(attr->slot(), 0));
        } else if (attr->type()->is_float()) {
            Operand val = load(Operand(self.reg(), Address(attr->slot())));
            return Operand(RegisterId(val.reg().id(), RegisterId::FLOAT));
        } else {
            return load(Operand(self.reg(), Address(attr->slot())));
        }
    } else {
        assert(!"Identifier not found");
    }
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
    return_ = RegisterId(++temp_, 0);

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
    mov(return_, load(new IntegerLiteral(loc, one)));
    jump(done_block);
    
    // Now emit the code for the 'false' branch.
    emit(else_block);
    String::Ptr zero = env_->integer("0");
    mov(return_, load(new IntegerLiteral(loc, zero)));
    
    emit(done_block);
    return return_;
}

void BasicBlockGenerator::scope_cleanup(Variable* var) {
    // Emits the code to clean up the stack when exiting block.  This includes 
    // decrementing reference counts, and calling destructors for value types.
    Type::Ptr type = var->type();
    assert(!!var->operand() && "Nil operand");
    if (!type) {
        // Nil variable type indicates that no cleanup need be done.
    } else if (type->is_primitive()) {
        // Do nothing, b/c the primitive has no destructor.
    } else if (type->is_ref()) {
        // Emit a branch to check the variable's reference count and
        // free it if necessary.
        refcount_dec(var->operand());               
    } else if (type->is_compound()) {
        // Call the value type's destructor, if it exists.
        value_dtor(var->operand(), type);
        calculate_size(type->clazz());
        local_slots_dec(type->clazz()->slots());
    } else {
        assert(!"Invalid type");
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
    if (!scope_.empty() && !!retval) {
        // Return the value by register, if the architecture supports return by
        // register
        if (retval.is_float() && machine_->float_return_regs()) {
            mov(machine_->float_return_reg(0)->id(), retval); 
        } else if (!retval.is_float() && machine_->int_return_regs()) {
            mov(machine_->int_return_reg(0)->id(), retval); 
        } else {
            assert(!"No return register");
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
    // 'var'.  Insert a call expression to call the refcount_dec function.  If
    // the variable is already in the temp list, then the current function has
    // already incremented the refcount for that object -- so just remove it
    // from the temp list instead of calling refcount_inc.
    for (int i = 0; i < object_temp_.size(); i++) {
        if (object_temp_[i].reg() == var.reg()) { 
            std::swap(object_temp_[i], object_temp_.back());
            object_temp_.pop_back();
            return;
        }
    }
    FuncMarshal fm(this);
    fm.arg(var);
    fm.call(env_->name("Object__refcount_inc"));
}

void BasicBlockGenerator::refcount_dec(Operand var) {
    // Emit code to decrement the reference count for the object specified by
    // 'temp'.  Insert a call expression to call the refcount_dec function 
    FuncMarshal fm(this);
    fm.arg(var);
    fm.call(env_->name("Object__refcount_dec"));
}

Operand BasicBlockGenerator::pop_ret(Type* type) {
    Register* reg = 0;
    if (type->is_float()) {
        reg = machine_->float_return_reg(0); 
    } else {
        reg = machine_->int_return_reg(0);
    }
    assert(reg && "No return regs for this architecture");
    if (type->is_float()) {
        return mov(RegisterId(reg->id().id(), RegisterId::FLOAT));
    } else {
        return mov(reg->id());
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
/*  Dump hash values
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
    // Calculate the memory footprint of the given class, and assign an address
    // to each attribute in the class.
    if (feature->slots()) { return; }
    if (feature->is_primitive()) {
        // All primitives have size equal to one word when stored on the stack
        // or as an attribute of a type.  FixMe: Eventually, implement alignment
        // so that this doesn't have to be the case.
        feature->slots_inc(machine_->word_size());
    } else if (feature->is_ref()) {
        // Size of refcount + vtable pointer
        feature->slots_inc(2);
    } else if (feature->is_compound()) { 
        // Do nothing
    }
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Type::Ptr type = attr->type();
            attr->slot(feature->slots());
            if (type->is_compound()) {
                calculate_size(type->clazz());
                feature->slots_inc(type->clazz()->slots());
            } else {
                feature->slots_inc(1);
            } 
        }
    }
}

void BasicBlockGenerator::ctor_preamble(Class* clazz) {
    // Emits the memory alloc/vtable setup for the class.

    // Allocate the memory for the new object by calling calloc with the size
    // of the object.
    Operand self;
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr size = env_->integer(stringify(bytes));
    String::Ptr one = env_->integer("1");
    if (clazz->is_object()) {
        FuncMarshal fm(this);
        fm.arg(load(new IntegerLiteral(Location(), size)));
        fm.arg(load(new IntegerLiteral(Location(), one)));
        fm.call(env_->name("Boot_calloc"));
 
        // Obtain a pointer to the 'self' object, and store it in the 'self'
        // variable.
        self = pop_ret(class_->type()); 
        variable(new Variable(env_->name("self"), self, 0)); 
       
        // Initialize the vtable pointer
        Operand vtable = Operand(self.reg(), Address(0));
        Operand label = load(env_->name(clazz->label()->string()+"__vtable"));
        store(vtable, label);
        
        // Make sure that the refcount starts out at 1, otherwise the object may
        // be freed before the end of the constructor.
        Operand refcount = Operand(self.reg(), Address(1));
        store(refcount, new IntegerLiteral(Location(), one));
    
    } else if (clazz->is_compound()) {
        self = id_operand(env_->name("self"));
        FuncMarshal fm(this);
        fm.arg(self);
        fm.arg(load(new IntegerLiteral(Location(), size)));
        fm.call(env_->name("Boot_mzero"));
    } else {
        assert(!"Invalid type");
    }

    // Emit initializer code for initialized attributes
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Expression::Ptr init = attr->initializer();
            if (!init || dynamic_cast<Empty*>(init.pointer())) {
                continue;
            }
            Operand value = emit(init);
            if (init->type()->is_primitive()) {
                // Don't need to do anything special for primitives
                Operand addr = Operand(self.reg(), Address(attr->slot()));
                store(addr, value);
            } else if (init->type()->is_ref()) {
                Operand addr = Operand(self.reg(), Address(attr->slot()));
                store(addr, value);
                refcount_inc(value);
            } else if (init->type()->is_compound()) {
                Operand addr = Operand(self.reg(), Address(attr->slot(), 0)); 
                value_assign_mem(value, addr, init->type());
                // XCOPY: If RHS is a temp, then move and free stack (don't copy)
                // XCOPY: attr = val
            } else {
                assert(!"Invalid type");
            }
            free_temps();
        }
    }
}

void BasicBlockGenerator::copier_preamble(Class* clazz) {
    // Emits the copy constructor preamble, which adjusts refcounts for
    // reference-type attributes of the value type.
    Operand self = id_operand(env_->name("self"));
    Operand other = id_operand(env_->name("val"));
    
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr size = env_->integer(stringify(bytes));
    FuncMarshal fm(this);
    fm.arg(self);
    fm.arg(other);
    fm.arg(load(new IntegerLiteral(Location(), size)));
    fm.call(env_->name("Boot_memcpy"));

    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer());
        if (attr && attr->type()->is_ref()) {
            Operand addr = Operand(self.reg(), Address(attr->slot())); 
            Operand ptr = load(addr); 
            refcount_inc(ptr); 
        }
    }
}

void BasicBlockGenerator::dtor_epilog(Function* feature) {
    // Free all of the attributes, and call destructors.
    std::vector<Attribute::Ptr> attrs;
    for (Feature::Ptr f = class_->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            if (attr->type()->is_primitive()) {
                // Pass
            } else if (attr->type()->is_ref()) {
                if (!attr->is_weak()) {
                    attrs.push_back(attr);
                }
            } else if (attr->type()->is_compound()) {
                attrs.push_back(attr);
            } else {
                assert(!"Invalid type");
            }
        }
    } 

    // The attributes need to be released in the reverse order
    for (int i = attrs.size()-1; i >= 0; i--) {
        Attribute::Ptr attr = attrs[i];
        Operand self = variable(env_->name("self"))->operand();
        if (attr->type()->is_ref()) {
            Operand addr = Operand(self.reg(), Address(attr->slot()));
            refcount_dec(load(addr));
        } else if (attr->type()->is_compound()) {
            Operand addr = Operand(self.reg(), Address(attr->slot(), 0));
            value_dtor(addr, attr->type()); 
        } else {
            assert(!"Invalid type");
        }
    }

    if (class_->is_primitive()) {
        // Pass
    } else if (class_->is_ref()) {
        FuncMarshal fm(this);
        fm.arg(variable(env_->name("self"))->operand());
        fm.call(env_->name("Boot_free"));
    } else if (class_->is_compound()) {
        // Pass
    } else {
        assert(!"Invalid type");
    }
}

void BasicBlockGenerator::free_temps() {
    // Free all object temporaries that were used to evaluate the expression by
    // decrementing their refcount.
    for (int i = 0; i < object_temp_.size(); i++) {
        refcount_dec(object_temp_[i]);
    }
    for (std::map<RegisterId, Variable::Ptr>::iterator i = value_temp_.begin();
        i != value_temp_.end(); ++i) {
        
        scope_cleanup(i->second); 
    } 
    object_temp_.clear();
    value_temp_.clear();
}

void BasicBlockGenerator::constants() {
    for (int i = 0; i < env_->constants(); i++) {
        Constant::Ptr cn = env_->constant(i);
        store(Operand(cn->label(), Address(0)), emit(cn->initializer()));
        free_temps();
        function_->file()->dependency(cn);
    }
}

void BasicBlockGenerator::local_slots_inc(int count) {
    local_slots_ += count;
    function_->local_slots(std::max(function_->local_slots(), local_slots_));
}

void BasicBlockGenerator::local_slots_dec(int count) {
    local_slots_ -= count;
    assert(local_slots_ >= 0);
}

void BasicBlockGenerator::arg_slots_inc(int count) {
    arg_slots_ += count;
    function_->arg_slots(std::max(function_->arg_slots(), arg_slots_));
}

void BasicBlockGenerator::arg_slots_dec(int count) {
    arg_slots_ -= count;
}

void BasicBlockGenerator::attr_assignment(Assignment* expr) {
    // Assignment to an attribute within a class
    String::Ptr id = expr->identifier();
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Type::Ptr type = expr->type(); 
    Variable::Ptr self = variable(env_->name("self"));

    if (type->is_primitive()) {
        Operand addr = Operand(self->operand().reg(), Address(attr->slot()));  
        store(addr, return_);
    } else if (type->is_ref()) {
        Operand addr = Operand(self->operand().reg(), Address(attr->slot()));  
        if (!attr->is_weak()) {
            refcount_dec(load(addr));
        } 
        store(addr, return_);
        if (!attr->is_weak()) {
            refcount_inc(return_);
        }
    } else if (type->is_compound()) {
        Operand addr = Operand(self->operand().reg(), Address(attr->slot(), 0));  
        value_dtor(addr, type);
        value_assign_mem(return_, addr, type);
        // XCOPY: If RHS is a temp, then move and free stack (don't copy)
        // XCOPY: attr = val
    } else {
        assert(!"Invalid type");
    }
}

void BasicBlockGenerator::secondary_assignment(Assignment* expr) {
    // Assignment to a local var that has already been initialized once in the
    // current scope.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Type::Ptr type = var->type();

    if (type->is_primitive()) {
        mov(var->operand(), return_);
    } else if (type->is_ref()) {
        refcount_dec(var->operand());
        mov(var->operand(), return_);
        refcount_inc(var->operand());
    } else if (type->is_compound()) {
        value_assign_reg(return_, var->operand(), type);
        // XCOPY: If RHS is a temp, then move TO REG (don't copy or free!)
        // XCOPY: local = val
    } else {
        assert(!"Invalid type");
    }
    return_ = var->operand();
}

void BasicBlockGenerator::initial_assignment(Assignment* expr) {
    // Assignment to a local var that has not yet been initialized in the
    // current scope.
    String::Ptr id = expr->identifier();
    Type::Ptr declared = expr->declared_type();
    Type::Ptr type = declared->is_top() ? expr->type() : declared.pointer();

    // Check that the value is in-register; if it isn't then there's an error
    // somewhere in the code generator.  Literal expressions should load the
    // value into a temporary SSA register.
    if (type->is_primitive()) {
        // For a primitive, do nothing b/c the value is already assigned a 
        // temporary, and no refcount needs to be incremented.
        assert(!!return_.reg());
    } else if (type->is_ref()) {
        assert(!!return_.reg());
        refcount_inc(return_);
    } else if (type->is_compound()) {
        value_assign_reg(return_, assign_addr_, type);
        // XCOPY: If value is a temp, then move to REG (don't copy or free!)
        // XCOPY: local = val 
    } else {
        assert(!"Invalid type");
    }
    variable(new Variable(id, return_, type));
}

void BasicBlockGenerator::value_assign_mem(Operand src, Operand dst, Type* type) {
    // Assign the value pointed to by 'src' to the mem location at 'dst'. 
    assert(!!src.reg());
    if (value_temp_.find(src.reg()) == value_temp_.end()) {
        // Value is not a temporary; invoke copy constructor
        value_copy(src, dst, type); 
    } else {
        // Value is a temporary; move data from 'src' to 'dst'
        value_move(src, dst, type);
        value_temp_.erase(src.reg());
        calculate_size(type->clazz());
        local_slots_dec(type->clazz()->slots()); 
    }  
    return_ = mov(dst);
}

void BasicBlockGenerator::value_assign_reg(Operand src, Operand dst, Type* type) {
    // Assign the value pointed to by 'src' to the temporary at 'dst.' Leave
    // the original value on the stack, but remove it from the temporary list.
    // That way, the storage allocated for the temporary is reassigned to the
    // named variable at 'dst.'  If 'dist' is the nil operand, then either a)
    // allocate new space on the stack or b) do nothing and use 'src' as
    // return_ if 'src' is a temporary that's already on the stack.
    if (src == dst) {
        assert(src == dst && "Temporary not assigned to var location");
    } else {
        // Value is not a temporary; invoke copy constructor
        value_copy(src, dst, type); 
    }
    return_ = mov(dst);
}

void BasicBlockGenerator::value_copy(Operand src, Operand dst, Type* type) {
    // Copies a value from 'src' to 'dst', and then increments the refcount
    // for any non-weak object attributes in 'type.'
    assert(!!src);
    assert(!!dst);
    Function::Ptr copy = type->clazz()->function(env_->name("@copy"));
    assert(copy && "Missing copy constructor");
    FuncMarshal fm(this);
    fm.arg(dst);
    fm.arg(src);
    fm.call(copy->label());           
}

void BasicBlockGenerator::value_move(Operand src, Operand dst, Type* type) {
    // Move the value without invoking the copy constructor
    assert(!!src.reg());
    calculate_size(type->clazz());
    int bytes = type->clazz()->slots() * machine_->word_size();
    String::Ptr size = env_->integer(stringify(bytes));
    FuncMarshal fm(this);
    fm.arg(dst);
    fm.arg(src);
    fm.arg(load(new IntegerLiteral(Location(), size)));
    fm.call(env_->name("Boot_memcpy")); 
}

void BasicBlockGenerator::value_dtor(Operand op, Type* type) {
    // Calls the destructor for the value type at location "op", where "op"
    // is a register containing the pointer to the value type on the stack.
    Function::Ptr dtor = type->clazz()->destructor();
    assert(dtor && "Missing value type destructor");
    FuncMarshal fm(this);
    fm.arg(op);
    fm.call(dtor->label());
}

Operand BasicBlockGenerator::emit(TreeNode* node, BasicBlock* yes, 
    BasicBlock* no, bool inv) {

    BasicBlock* true_save = true_;
    BasicBlock* false_save = false_;
    bool invert_branch_save_ = invert_branch_;
    true_ = yes;
    false_ = no;
    invert_branch_ = inv;
    node->operator()(this);
    true_ = true_save;
    false_ = false_save;
    invert_branch_ = invert_branch_save_;
    return return_;
}

Operand BasicBlockGenerator::emit(TreeNode* node) {
    node->operator()(this);
    return return_;
}

Operand BasicBlockGenerator::emit(Opcode op, Operand t2, Operand t3) {
    Instruction in = block_->instr(op, temp_inc(), t2, t3);
    return in.result();
}

Operand BasicBlockGenerator::emit(Opcode op, Operand t2) {
    Instruction in = block_->instr(op, temp_inc(), t2, Operand());
    return in.result();
}

Operand BasicBlockGenerator::mov(Operand res, Operand t2) { 
    // FixMe: This could be a problem for SSA analysis.
    Instruction in = block_->instr(MOV, res, t2, Operand());
    return in.result();
}

void BasicBlockGenerator::emit(BasicBlock* block) {
    if (block_) {
        block_->next(block);
    }
    block_ = block;
    function_->basic_block(block);
}

void BasicBlockGenerator::store(Operand addr, Operand value) {
    block_->instr(STORE, Operand(), addr, value);    
}

void BasicBlockGenerator::call(Operand func) {
    block_->instr(CALL, Operand(), func, Operand());
}

void BasicBlockGenerator::ret() {
    block_->instr(RET, Operand(), Operand(), Operand());
}

void BasicBlockGenerator::branch(Opcode op, Operand t2, Operand t3, 
    BasicBlock* branch, BasicBlock* next) {

    block_->instr(op, Operand(), t2, t3);
    block_->branch(branch);
    block_->next(next);
}

void BasicBlockGenerator::bne(Operand t2, Operand t3, BasicBlock* target, 
    BasicBlock* next) {
    branch(BNE, t2, t3, target, next);
}

void BasicBlockGenerator::be(Operand t2, Operand t3, BasicBlock* target, 
    BasicBlock* next) {
    branch(BE, t2, t3, target, next);
}

void BasicBlockGenerator::bnz(Operand t2, BasicBlock* target, 
    BasicBlock* next) {
    branch(BNZ, t2, Operand(), target, next);
}

void BasicBlockGenerator::bz(Operand t2, BasicBlock* target, 
    BasicBlock* next) {
    branch(BZ, t2, Operand(), target, next);
}

void BasicBlockGenerator::bg(Operand t2, Operand t3, BasicBlock* target, 
    BasicBlock* next) {
    branch(BG, t2, t3, target, next);
}

void BasicBlockGenerator::bl(Operand t2, Operand t3, BasicBlock* target,
    BasicBlock* next) {
    branch(BL, t2, t3, target, next);
}

void BasicBlockGenerator::bge(Operand t2, Operand t3, BasicBlock* target,
    BasicBlock* next) {
    branch(BGE, t2, t3, target, next);
}

void BasicBlockGenerator::ble(Operand t2, Operand t3, BasicBlock* target, 
    BasicBlock* next) {
    branch(BLE, t2, t3, target, next);
}

void BasicBlockGenerator::jump(BasicBlock* target) {
    block_->instr(JUMP, Operand(), Operand(), Operand());
    block_->branch(target);
}

void FuncMarshal::arg(Operand op) {
    // Add an argument to the list, and keep track of the number of float/int
    // args so that they are assigned to argument registers/stack properly.
    if (op.is_float()) {
        float_args_++;
    } else {
        int_args_++; 
    }
    arg_.push_back(op);
}

void FuncMarshal::call(Operand func) {
    // Emits instructions for pushing the function arguments and invoking the
    // function specified by 'func', which may be a label or a function
    // pointer.
    assert(gen_->machine_->sp_reg());
    RegisterId sp = gen_->machine_->sp_reg()->id();
    int int_arg = 0;
    int float_arg = 0;
    int stack_arg = 0;
    for (int i = 0; i < arg_.size(); ++i) {
        Register* reg = 0;
        if (arg_[i].is_float()) {
            reg = gen_->machine_->float_arg_reg(float_arg);
            float_arg++;
        } else {
            reg = gen_->machine_->int_arg_reg(int_arg);
            int_arg++;
        }
        if (reg) {
            gen_->mov(reg->id(), arg_[i]); // Pass argument by register
#ifdef WINDOWS
            gen_->arg_slots_inc(1);
            gen_->store(Operand(sp, Address(stack_arg)), arg_[i]);
            stack_arg++;
            // Windows: also pass the value on the stack
#endif
        } else {
            gen_->arg_slots_inc(1);
            gen_->store(Operand(sp, Address(stack_arg)), arg_[i]);
            stack_arg++;
        }
    }
    gen_->call(func);
    gen_->arg_slots_dec(stack_arg);
}

void FuncUnmarshal::arg(String* name, Type* type) {
    // Unmarshal an argument by emitting code to save named parameter's
    // location on the stack, or else to move the argument from a precolored
    // arg reg into a temporary.
    Register* reg = 0;
    if (type->is_float()) {
        reg = gen_->machine_->float_arg_reg(float_args_++);
    } else {
        reg = gen_->machine_->int_arg_reg(int_args_++);
    } 
    if (reg) {
        // Variable is passed by register; precolor the temporary for this
        // formal parameter by using a negative number.  Passing NULL for
		// the type of the variable prevents it from being garbage collected
		// at the end of the scope (which is what we want for function 
		// parameters).
        gen_->variable(new Variable(name, gen_->mov(reg->id()), 0));

        // On Windows, the value is also passed with a backing location on
        // the stack.
#ifdef WINDOWS
        RegisterId id(0, type->is_float() ? RegisterId::FLOAT : 0);
        Operand op(id, Address(++stack_args_));
        gen_->variable(new Variable(name, op, 0));
#endif
    } else {
        RegisterId id(0, type->is_float() ? RegisterId::FLOAT : 0);
        Operand op(id, Address(++stack_args_));
        gen_->variable(new Variable(name, op, 0));
    }
}

