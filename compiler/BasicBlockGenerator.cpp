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

#define REFCOUNT_OFFSET 0

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

void BasicBlockGenerator::operator()(Class* feature) {
    // Output intermediate-level code for the class given by 'feature'.  
    // This function also sets up the vtable for the class.
    class_ = feature;
    enter_scope(0);
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    exit_scope();

    if (!feature->is_object()) { return; }

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
            if (func->name()->string() != "@destroy") {
                n++;
            }
        }
    }
    if (!n) { return; }

    // Step 1: Place all keys into buckets using a simple hash.  There will
    // be collisions, but they will be resolved in steps 2-3.
    vector<JumpBucket> bucket(n);
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
            if (func->name()->string() != "@destroy") {
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
            if (func->name()->string() == "@destroy") { continue; }
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
*/
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
            bz(left, false_, left_true);
        }
        emit(left_true); 

        return_ = emit(expr->right(), true_, false_, invert_branch_);
        
    } else if (env_->name("or") == expr->operation()) {
        BasicBlock::Ptr left_false = basic_block();
        
        // Don't use the 'real' false branch; on false we want to emit
        // the test for the second side of the or
        Operand left = emit(expr->left(), true_, left_false, true);
        if (!block_->is_terminated()) {
            bnz(left, true_, left_false);
        }
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
    } else {
        assert(!"Unsupported unary operation");
    }
}

void BasicBlockGenerator::operator()(Call* expr) {
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
    String::Ptr id = expr->identifier();
    String::Ptr scope = expr->scope();
    Function::Ptr func = expr->file()->function(scope, id);

    // Insert a call expression, then pop the return value off the stack.
    call(func->label());
    if (!func->type()->is_void()) {
        return_ = emit_pop_ret();
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
        call(env_->name("_Object__dispatch"));
        fnptr = emit_pop_ret();
    }

    for (int i = args.size()-1; i >= 0; i--) {
        emit_push_arg(i, args[i]);
    }

    if (type->is_interface()) {
        // Dynami dispatch: call the function pointer.
        call(fnptr);
    } else {
        // Static dispatch: insert a call expression, then pop the return value
        // off the stack.
        call(func->label());
    }

    if (!func->type()->is_void()) {
        return_ = emit_pop_ret();
    } else {
        return_ = 0;
    }
}

void BasicBlockGenerator::operator()(Construct* expr) {
}

void BasicBlockGenerator::operator()(Identifier* expr) {
    // Simply look up the value of the variable as stored previously.
    Variable* var = variable(expr->identifier());
    if (var) {
        return_ = var->operand();
    } else {
        // Variable can't be found in a temporary; it must be an argument 
        // passed on the stack.

        int offset = stack(expr->identifier());
        assert(offset);
        return_ = load(Operand::addr(offset)); 
        // A load operand of zero means the variable must be loaded relative
        // to the base pointer.

        variable(expr->identifier(), new Variable(return_, 0));
    }
}

void BasicBlockGenerator::operator()(Empty* expr) {
    // Do nothing
    return_ = env_->integer("0");
}

void BasicBlockGenerator::operator()(Block* statement) {
    enter_scope(basic_block());
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Simple* statement) {
    Expression::Ptr expr = statement->expression();
    expr(this);
}

void BasicBlockGenerator::operator()(Let* statement) {
    // Enter a new scope, then emit code for initializing all the let
    // variables, and initialize code for the body.
    enter_scope(basic_block());
    for (Statement::Ptr v = statement->variables(); v; v = v->next()) {
        emit(v);
    } 
    emit(statement->block());
    exit_scope();
}

void BasicBlockGenerator::operator()(While* statement) {
    // Emit the guard expression in a new basic block
    BasicBlock::Ptr guard_block = basic_block();
    BasicBlock::Ptr body_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();

    emit(guard_block);

    // Recursively emit the boolean guard expression.  
    Operand guard = emit(statement->guard(), body_block, done_block, false);
    if (!block_->is_terminated()) {
        bz(guard, done_block, body_block);
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

    // Recursively emit the boolean guard expression.  We need to pass the
    // true and the false block pointers so that the correct code is emitted
    // on each branch.
    Operand guard = emit(statement->guard(), then_block, else_block, false);
    if (!block_->is_terminated()) {
        bz(guard, else_block, then_block);
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

    Operand value;
    if (dynamic_cast<Empty*>(statement->initializer())) {
        value = env_->integer("0");
    } else {
        value = emit(statement->initializer());
    }

    Type::Ptr type = statement->type(); 
    Variable::Ptr var = variable(statement->identifier());
    if (!var) {
        Type::Ptr type = statement->initializer()->type();
        var = new Variable(++temp_, type);
        variable(statement->identifier(), var);
    } else if (!type->is_value()) {
        emit_refcount_dec(var);
    }
    block_->instr(MOV, var->operand(), value, 0);
    if (!type->is_value()) {
        emit_refcount_inc(var);
    }
}

void BasicBlockGenerator::operator()(Return* statement) {
    if (!dynamic_cast<Empty*>(statement->expression())) {
        // Don't actually return right away; store the result in a pseudo-var
        // and return it after cleanup.
        Operand value = emit(statement->expression());
        Type::Ptr type = statement->expression()->type();
        scope_.back()->return_val(new Variable(value, type));
    }
    scope_.back()->has_return(true);
}

void BasicBlockGenerator::operator()(When* statement) {
}

void BasicBlockGenerator::operator()(Case* statement) {
}

void BasicBlockGenerator::operator()(Fork* statement) {
}

void BasicBlockGenerator::operator()(Yield* statament) {
}

void BasicBlockGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) {
        return;
    }

    // Reset the temporaries for the function.
    temp_ = Operand();
    function_ = feature;
    block_ = 0;
    emit(basic_block());
    stack_.clear();
    enter_scope(basic_block());

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
            variable(f->name(), new Variable(mov(reg), 0));
        }
        index++;
    } 
    
    // Generate code for the body of the function.
    emit(feature->block());
    exit_scope();

    if(feature->type()->is_void()) {
        emit_return();
    }
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
        args.push_back(emit(a));
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

void debug4() { }

BasicBlock* BasicBlockGenerator::basic_block() {
    BasicBlock* block = new BasicBlock();
    block->label(env_->name("l" + stringify(++label_)));
    if (block->label()->string() == "l7") {
        debug4();
    }
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

void BasicBlockGenerator::variable(String* name, Variable* var) {
    assert(scope_.size());
    scope_.back()->variable(name, var);
}

void BasicBlockGenerator::stack(String* name, int offset) {
    stack_.insert(make_pair(name, offset));
}

void BasicBlockGenerator::enter_scope(BasicBlock* cleanup) {
    scope_.push_back(new Scope(cleanup));
}

void BasicBlockGenerator::exit_scope() {
    // Pops the symbol table for this scope off the stack, and inserts code
    // to perform cleanup at the end of the scope.
    Scope::Ptr scope = scope_.back();

    map<String::Ptr, Variable::Ptr>::iterator i;
    for (i = scope->variable_.begin(); i != scope->variable_.end(); i++) {
        emit_var_cleanup(i->second);
    }

    if (!scope->has_return()) {
        scope_.pop_back();
        return;
    }

    // Search backwards through the vector and clean up variables in the 
    // containing scope.
    for (int j = scope_.size()-2; j > 1; j--) {
        Scope::Ptr s = scope_[j];
        for (i = s->variable_.begin(); i != s->variable_.end(); i++) {
            emit_var_cleanup(i->second);
        }
    }

    emit_return();
    scope_.pop_back();
}

void BasicBlockGenerator::emit_var_cleanup(Variable* var) {
    Type::Ptr type = var->type();
    if (type && !type->is_primitive()) {
        if (type->is_value()) {
            cerr << type->name()->string() << endl;
            assert(!"Need to figure out how to do value types");
            // Call destructor!
        } else {
            // Emit a branch to check the variable's reference count and
            // free it if necessary.
            emit_refcount_dec(var);               
        }
    }
}

void BasicBlockGenerator::emit_return() {
    // Emit an actual return.  Emit code to return the value saved in the var
    // '_ret' if the variable has been set.
    if (!scope_.empty() && scope_.back()->return_val()) {
        Variable::Ptr return_val = scope_.back()->return_val();
        if (machine_->return_regs()) {
            // Return the value by register, if the architecture supports return
            // by register
            int index = 0;
            int reg = -machine_->return_reg(index)->id();
            block_->instr(MOV, reg, return_val->operand(), 0); 
        } else {
            // Otherwise, return on the stack.
            push(return_val->operand());
        }
    }
    ret();
}

void BasicBlockGenerator::emit_refcount_inc(Variable* var) {
    // Emit code to increment the reference count for the object specified
    // by 'temp'
    int temp = var->operand().temp();

    // Insert a call expression to call the refcount_dec function 
    if (machine_->arg_regs()) {
        int val = 0;
        block_->instr(MOV, -machine_->arg_reg(val)->id(), temp, 0);
    } else {
        push(temp);
    }
    call(env_->name("_Object__refcount_inc"));
}

void BasicBlockGenerator::emit_refcount_dec(Variable* var) {
    // Emit code to decrement the reference count for the object specified
    // by 'temp'
    int temp = var->operand().temp();

    // Insert a call expression to call the refcount_dec function 
    if (machine_->arg_regs()) {
        int val = 0;
        block_->instr(MOV, -machine_->arg_reg(val)->id(), temp, 0);
    } else {
        push(temp);
    }
    call(env_->name("_Object__refcount_dec"));
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

/*
void BasicBlockGenerator::emit_refcount_check(const VarInfo& info) {
    // Emit code to decrement the reference count for the object specified
    // by 'temp'
    int temp = info.first.temp();
    Operand count;

    // count = object->count + 1
    count = load(Operand::addr(temp, 1));
    IntegerLiteral::Ptr one(new IntegerLiteral(Location(), env_->integer("1")));
    count = sub(count, load(one.pointer()));
    
    // if (count == 0) { free(); }
    BasicBlock::Ptr then_block = basic_block();
    BasicBlock::Ptr done_block = basic_block();
    bnz(count, done_block, then_block);
    
    // TODO: Need to disallow assignments to function parameters...
    emit(then_block);    
    
    Class::Ptr clazz = info.second->clazz(); 
    Function::Ptr func = clazz->function(env_->name("@destroy"));
    if (!func) {
        assert(!"No destructor found!");
    }
    
    // Insert a call expression to call the destructor.  
    if (machine_->arg_regs()) {
        int val = 0;
        block_->instr(MOV, -machine_->arg_reg(val)->id(), temp, 0);
    } else {
        push(temp);
    }
    call(func->label());
    emit(done_block);
}
*/


