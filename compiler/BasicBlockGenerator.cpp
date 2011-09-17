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

    if (env_->errors()) {
        return;
    }
    for (Feature::Ptr m = env_->modules(); m; m = m->next()) {
        m(this);
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

    if (!feature->is_object()) { return; }

    emit_vtable(feature);
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
    emit_refcount_inc(return_);
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

void BasicBlockGenerator::operator()(Call* expr) {
    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        if (a->type()->is_bool()) {
            args.push_back(emit_bool_expr(a));
        } else {
            args.push_back(emit(a));
        }
    }
    for (int i = args.size()-1; i >= 0; i--) {
        emit_push_arg(i, args[i]);
    }

    // Look up the function by name in the current context.
    String::Ptr id = expr->identifier();
    String::Ptr scope = expr->scope();
    Function::Ptr func = expr->file()->function(scope, id);

    // Insert a call expression, then pop the return value off the stack.
    call(func->label());
    if (!func->type()->is_void()) {
        return_ = emit_pop_ret();
        if (!func->type()->is_value()) {
            object_temp_.push_back(return_);
        }
    } else {
        return_ = 0;
    }
}

void BasicBlockGenerator::operator()(Dispatch* expr) {
    // The call expression may actually be an operator, if the type of the
    // expression is primitive.
    Type::Ptr type = expr->arguments()->type();
    String::Ptr id = expr->identifier();
    if (id->string()[0] == '@' && type->is_primitive()) {
        emit_operator(expr);
        return; 
    }

    // Push objects in anticipation of the call instruction.
    vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }

    // Look up the function by name in the current context.
    Expression::Ptr receiver = expr->arguments();
    Class::Ptr clazz = receiver->type()->clazz();
    Function::Ptr func = clazz->function(id);

    Operand fnptr;
    if (type->is_interface()) { 
        // Dynamic dispatch: call the object dispatch function with the
        // receiver and function name as arguments.
        
        String::Ptr name = env_->string(func->name()->string());
        emit_push_arg(1, load(new StringLiteral(Location(), name)));
        emit_push_arg(0, args[0]); // Receiver
        call(env_->name("Object__dispatch"));
        fnptr = emit_pop_ret();
    }

    for (int i = args.size()-1; i >= 0; i--) {
        emit_push_arg(i, args[i]);
    }

    if (type->is_interface()) {
        // Dynamic dispatch: call the function pointer.
        call(fnptr);
    } else {
        // Static dispatch: insert a call expression, then pop the return value
        // off the stack.
        call(func->label());
    }

    if (!func->type()->is_void()) {
        return_ = emit_pop_ret();
        if (!func->type()->is_value()) {
            object_temp_.push_back(return_);
        }
    } else {
        return_ = 0;
    }
}

void BasicBlockGenerator::operator()(Construct* expr) {
    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }
    for (int i = args.size()-1; i >= 0; i--) {
        emit_push_arg(i, args[i]);
    }

    // Look up the function by name in the current context.
    String::Ptr id = env_->name("@init");
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->function(id);

    // Insert a call expression, then pop the return value off the stack.
    call(func->label());
    return_ = emit_pop_ret();
    if (!expr->type()->is_value()) {
        object_temp_.push_back(return_);
    }
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
        return_ = load(Operand::addr(self.temp(), attr->slot()+2));
        // +2 is for refcount, vtable slots
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
    emit_free_temps();
}

void BasicBlockGenerator::operator()(Let* statement) {
    // Enter a new scope, then emit code for initializing all the let
    // variables, and initialize code for the body.
    enter_scope();
    for (Statement::Ptr v = statement->variables(); v; v = v->next()) {
        emit(v);
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
    emit_free_temps();
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
    emit_free_temps();
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

void BasicBlockGenerator::operator()(Assignment* statement) {
    // FixMe: This breaks SSA form, because variables are not renamed.  With
    // this code the way it is, the generator only generates SSA for temporary
    // expressions.  This limitation is here as a punt on introducing
    // phi-functions until optimizations are needed, since without
    // optimizations, SSA is not needed anyway.

    Expression::Ptr init = statement->initializer();
    Operand value;
    if (dynamic_cast<Empty*>(init.pointer())) {
        value = env_->integer("0");
    } else if (init->type()->is_bool()) {
        value = emit_bool_expr(init.pointer());
    } else {
        value = emit(init);
    }

    String::Ptr id = statement->identifier();
    Type::Ptr type = statement->type(); 
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;

    if (var) {
        // Assignment to a local var that has already been initialized once in
        // the current scope.
        if (!type->is_value()) {
            emit_refcount_dec(var->operand());
        }
        block_->instr(MOV, var->operand(), value, 0);
        if (!type->is_value()) {
            emit_refcount_inc(var->operand());
        }
    } else if (attr) {
        // Assignment to an attribute within a class
        Variable::Ptr self = variable(env_->name("self"));
        Operand addr = Operand::addr(self->operand().temp(), attr->slot()+2);  
        Operand old = load(addr);
        if (!type->is_value() && !attr->is_weak()) {
            emit_refcount_dec(old);
        } 
        // +2 is for vtable and refcount
        store(addr, value);
        if (!type->is_value() && !attr->is_weak()) {
            emit_refcount_inc(value);
        }
    } else {
        // Assignment to a local var that has not yet been initialized in the
        // current scope.
        Variable::Ptr var = new Variable(id, mov(value), init->type());
        variable(var);
        if (!type->is_value()) {
            emit_refcount_inc(var->operand());
        }
    }

    emit_free_temps();
}

void BasicBlockGenerator::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    if (!dynamic_cast<Empty*>(expr.pointer())) {
        // Don't actually return right away; store the result in a pseudo-var
        // and return it after cleanup.
        Operand ret;
        if (expr->type()->is_bool()) {
            ret = emit_bool_expr(expr);
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
            emit_refcount_inc(ret);
        }
    }
    scope_.back()->has_return(true);
    emit_free_temps();
}

void BasicBlockGenerator::operator()(When* statement) {
    assert(!"Not implemented");
    emit_free_temps();
}

void BasicBlockGenerator::operator()(Case* statement) {
    assert(!"Not implemented");
    emit_free_temps();
}

void BasicBlockGenerator::operator()(Fork* statement) {
    assert(!"Not implemented");
    emit_free_temps();
}

void BasicBlockGenerator::operator()(Yield* statament) {
    assert(!"Not implemented");
    emit_free_temps();
}

void BasicBlockGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) {
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
            // formal parameter by using a negative number.
            int reg = -machine_->arg_reg(index)->id();
            variable(new Variable(f->name(), mov(reg), 0));
        }
        index++;
    } 

    // Set up the 'self' variable with the constructor, if necessary; this 
    // allocates the memory for the object using calloc so that all of the
    // fields are initialized to zero.
    if (feature->is_constructor()) {
        emit_ctor_preamble(feature);
    }

    // Generate code for the body of the function.
    emit(feature->block());
    if (feature->type()->is_void() || function_->is_constructor()) {
        emit_return();
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

void BasicBlockGenerator::emit_operator(Dispatch* expr) {
    // FixMe: Replace this with a mini-parser that can read three-address-code
    // and output it as an inline function.
    string id = expr->identifier()->string();

    vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        if (a->type()->is_bool()) {
            args.push_back(emit_bool_expr(a));
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

    // Remove variables in reverse order!
    for (int i = scope->variables()-1; i >= 0; i--) { 
        emit_cleanup(scope->variable(i));
    }

    if (!scope->has_return()) {
        scope_.pop_back();
        return;
    }

    // Search backwards through the vector and clean up variables in the 
    // containing scope.
    for (int j = scope_.size()-2; j > 1; j--) {
        Scope::Ptr s = scope_[j];
        for (int i = s->variables()-1; i >= 0; i--) {
            emit_cleanup(scope->variable(i));
        }
    }

    emit_return();
    scope_.pop_back();
}

Operand BasicBlockGenerator::emit_bool_expr(Expression* expression) {
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
    emit_free_temps();
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

void BasicBlockGenerator::emit_cleanup(Variable* var) {
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
            emit_refcount_dec(var->operand());               
        }
    }
}

void BasicBlockGenerator::emit_return() {
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
        emit_dtor_epilog(function_);
    }
    
    ret();
}

void BasicBlockGenerator::emit_refcount_inc(Operand var) {
    // Emit code to increment the reference count for the object specified
    // by 'temp'
    int temp = var.temp();

    // Insert a call expression to call the refcount_dec function 
    if (machine_->arg_regs()) {
        int val = 0;
        block_->instr(MOV, -machine_->arg_reg(val)->id(), temp, 0);
    } else {
        push(temp);
    }
    call(env_->name("Object__refcount_inc"));
}

void BasicBlockGenerator::emit_refcount_dec(Operand var) {
    // Emit code to decrement the reference count for the object specified
    // by 'temp'
    int temp = var.temp();

    // Insert a call expression to call the refcount_dec function 
    if (machine_->arg_regs()) {
        int val = 0;
        block_->instr(MOV, -machine_->arg_reg(val)->id(), temp, 0);
    } else {
        push(temp);
    }
    call(env_->name("Object__refcount_dec"));
}

void BasicBlockGenerator::emit_push_arg(int i, Operand op) {
    if (i >= machine_->arg_regs()) {
        push(op);
    } else {
        block_->instr(MOV, -machine_->arg_reg(i)->id(), op, 0);
    }        
}

Operand BasicBlockGenerator::emit_pop_ret() {
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

void BasicBlockGenerator::emit_vtable(Class* feature) {
    // Generate the Pearson perfect hash that will be used for vtable lookups.
    // FixMe: This is extremely haggard, because I couldn't find an algorithm
    // for generating the Pearson hash permutation algorithm.  Supposedly,
    // there is such an algorithm, but I couldn't find any literature on it.  
    // This brute-force method of ensuring no collisions may fail in some 
    // cases, which is a serious problem.
    
    // Initially set the size of the hash table to the number of functions.
    // The size of the table will grow to prevent collisions.
    int n = 0;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (!func->is_constructor() && !func->is_destructor()) {
                n++;
            }
        }
    }
    if (!n) { return; }
    n = max(10, n * 2);

    // Step 1: Place all keys into buckets using a simple hash.  There will
    // be collisions, but they will be resolved in steps 2-3.
    vector<JumpBucket> bucket(n);
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (!func->is_constructor() && !func->is_destructor()) {
                uint64_t hash = fnv_hash(0, func->name()) % n;
                bucket[hash].push_back(func);
            }
        }
    }

    // Step 2: Sort buckets and process the ones with the most items first.
    sort(bucket.begin(), bucket.end(), SortJumpBuckets()); 
    
    // Step 3: Attempt to place items in the buckets into empty slots in the
    // second jump table, starting with the largest buckets first.
    vector<Function::Ptr> value(n);
    vector<Function::Ptr> slots;
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
    feature->size(size);
}

void BasicBlockGenerator::emit_ctor_preamble(Function* feature) {
    // Emits the memory alloc/vtable setup for the class.  FIXME: Eventually,
    // move to a prototype & copy type approach for initializing variables
    // FIXME: Run variable initializers here

    // Allocate the memory for the new object by calling calloc with the size
    // of the object.
    Operand self;
    if (class_->is_object()) {
        String::Ptr one = env_->integer("1");
        String::Ptr size = env_->integer(stringify(class_->size()));
        emit_push_arg(1, load(new IntegerLiteral(Location(), one)));
        emit_push_arg(0, load(new IntegerLiteral(Location(), size)));
        call(env_->name("calloc"));        
 
        // Obtain a pointer to the 'self' object, and store it in the 'self'
        // variable.
        self = emit_pop_ret(); 
        variable(new Variable(env_->name("self"), self, 0)); 
       
        // Initialize the vtable pointer
        Operand vtable = Operand::addr(self.temp(), 0);
        Operand label = load(env_->name(class_->name()->string()+"__vtable"));
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
    for (Feature::Ptr f = class_->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Expression::Ptr init = attr->initializer();
            if (!init) {
                continue;
            }
            if (dynamic_cast<Empty*>(init.pointer())) {
                continue;
            }
            Operand value = emit(init);
            Operand addr = Operand::addr(self.temp(), attr->slot()+2);
            // +2 is for vtable and refcount slots
            store(addr, value);
            if (!init->type()->is_value()) {
                emit_refcount_inc(value);
            }
            emit_free_temps();
        }
    }
}

void BasicBlockGenerator::emit_dtor_epilog(Function* feature) {
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
        Operand val = load(Operand::addr(self.temp(), attrs[i]->slot()+2));
        emit_refcount_dec(val);
        // +2 is for refcount, vtable slots
    }

    if (class_->is_object()) {
        emit_push_arg(0, variable(env_->name("self"))->operand());
        call(env_->name("free"));
    }
}

void BasicBlockGenerator::emit_free_temps() {
    // Free all object temporaries that were used to evaluate the expression by
    // decrementing their refcount.
    for (int i = 0; i < object_temp_.size(); i++) {
        emit_refcount_dec(object_temp_[i]);
    }
    object_temp_.clear();
}

