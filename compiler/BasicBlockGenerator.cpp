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

#define REFCOUNT_OFFSET 0

using namespace std;

BasicBlockGenerator::BasicBlockGenerator(Environment* env, Machine* mach) :
    environment_(env),
    machine_(mach),
    label_(0) {

    if (environment_->errors()) {
        return;
    }
    for (Feature::Ptr m = environment_->modules(); m; m = m->next()) {
        m(this);
    }    
}

void BasicBlockGenerator::operator()(Class* feature) {
    class_ = feature;
    enter_scope();
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    exit_scope();
}

void BasicBlockGenerator::operator()(Module* feature) {
    module_ = feature;
    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
}

void BasicBlockGenerator::operator()(Formal* formal) {
}

void BasicBlockGenerator::operator()(StringLiteral* expression) {
    // Load a pointer to the string from the string table
    //assert(!"Not implemented");
    return_ = expression;
}

void BasicBlockGenerator::operator()(IntegerLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = expression;
}

void BasicBlockGenerator::operator()(FloatLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = expression;
}

void BasicBlockGenerator::operator()(BooleanLiteral* expression) {
    // Load the literal value with load-immediate
    return_ = expression;
}

void BasicBlockGenerator::operator()(Binary* expression) {
    // Emit the left and right expressions, then perform the operation on
    // the two operands, and return the result.

    // if not
    // temp = true_
    // true_ = false_
    // false_ = true_
    if (environment_->name("and") == expression->operation()) {
        BasicBlock::Ptr left_true = basic_block();
        
        // Don't use the 'real' true branch; on true we want to emit the    
        // test for the second side of the and
        Operand left = emit(expression->left(), left_true, false_);
        if (!block_->is_terminated()) {
            bz(left, false_, left_true);
        }
        emit(left_true); 

        return_ = emit(expression->right(), true_, false_);
        //if (!block_->is_terminated()) {
        //    bz(right, false_, true_);
        //}
        
    } else if (environment_->name("or") == expression->operation()) {
        BasicBlock::Ptr left_false = basic_block();
        
        // Don't use the 'real' false branch; on false we want to emit
        // the test for the second side of the or
        Operand left = emit(expression->left(), true_, left_false);
        if (!block_->is_terminated()) {
            bnz(left, true_, left_false);
        }
        emit(left_false);
    
        return_ = emit(expression->right(), true_, false_);
        //if (!block_->is_terminated()) {
        //    bz(right, false_, true_);
        //}
    } else {
        assert(!"Unsupported binary operation");
    }
}

void BasicBlockGenerator::operator()(Unary* expression) {
    // Emit the code for the expression, then perform the operation on the
    // operand and return the result.
    Operand op = emit(expression->child());

    if (environment_->name("not")) {


    
    } else {
        assert(!"Unsupported unary operation");
    }
}

void BasicBlockGenerator::operator()(Call* expression) {
    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    std::vector<Operand> args;
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }
    for (int i = args.size()-1; i >= 0; i--) {
        if (i >= machine_->arg_regs()) {
            // Pass the parameter on the stack
            push(args[i]);
        } else {
            // Pass the parameter using a precolored register
            block_->instr(MOV, -machine_->arg_reg(i)->id(), args[i], 0); 
        }
    }

    // Look up the function by name in the current context.
    String::Ptr id = expression->identifier();
    String::Ptr scope = expression->scope();
    Function::Ptr func = expression->file()->function(scope, id);
    String::Ptr name = func->label();

    // Insert a call expression, then pop the return value off the stack.
    call(name);
    if (!func->type()->is_void()) {
        int val = 0;
        return_ = mov(-machine_->return_reg(val)->id());
    } else {
        return_ = 0;
    }
}

void BasicBlockGenerator::operator()(Dispatch* expression) {
    // The call expression may actually be an operator, if the type of the
    // expression is primitive.
    Type::Ptr type = expression->arguments()->type();
    String::Ptr id = expression->identifier();
    if (id->string()[0] == '@' && type->is_primitive()) {
        emit_operator(expression);
        return; 
    }

    if (type->is_interface()) {
        assert(!"Not implemented");
    }

    // Push objects in anticipation of the call instruction.
    std::vector<Operand> args;
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }
    for (int i = args.size()-1; i >= 0; i--) {
        if (i >= machine_->arg_regs()) {
            // Pass the parameter on the stack
            push(args[i]);
        } else {
            // Pass the parameter using a precolored register
            block_->instr(MOV, -machine_->arg_reg(i)->id(), args[i], 0); 
        }
    }

    // Look up the function by name in the current context.
    Expression::Ptr receiver = expression->arguments();
    Class::Ptr clazz = receiver->type()->clazz();
    Function::Ptr func = clazz->function(id);
    String::Ptr name = func->label();

    // Insert a call expression, then pop the return value off the stack.
    call(name);
    if (!func->type()->is_void()) {
        return_ = pop();
    } else {
        return_ = 0;
    }
}

void BasicBlockGenerator::operator()(Construct* expression) {
}

void BasicBlockGenerator::operator()(Identifier* expression) {
    // Simply look up the value of the variable as stored previously.
    return_ = variable(expression->identifier());
    if (!return_.temporary()) {
        int offset = stack(expression->identifier());
        return_ = load(0, offset); 
        // A load operand of zero means the variable must be loaded relative
        // to the base pointer.

        variable(expression->identifier(), return_);
    }
}

void BasicBlockGenerator::operator()(Empty* expression) {
    // Do nothing
    return_ = environment_->integer("0");
}

void BasicBlockGenerator::operator()(Block* statement) {
    for (Statement::Ptr s = statement->children(); s; s = s->next()) {
        s(this);
    }
}

void BasicBlockGenerator::operator()(Simple* statement) {
    Expression::Ptr expression = statement->expression();
    expression(this);
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

    // Recursively emit the boolean guard expression.  We need to save the
    // true and the false block pointers so that the correct code is emitted
    // on each branch.
    Operand guard = emit(statement->guard(), then_block, else_block);
    if (!block_->is_terminated()) {
        bz(guard, else_block, then_block);
    }

    // Now emit the true branch; only jump if there is a false branch
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

void BasicBlockGenerator::operator()(Variable* statement) {
    // FixMe: This breaks SSA form, because variables are not renamed.  With
    // this code the way it is, the generator only generates SSA for temporary
    // expressions.  This limitation is here as a punt on introducing
    // phi-functions until optimizations are needed, since without
    // optimizations, SSA is not needed anyway.


    Operand value;
    if (dynamic_cast<Empty*>(statement->initializer())) {
        value = environment_->integer("0");
    } else {
        value = emit(statement->initializer());
    }

    Operand var = variable(statement->identifier());
    if (!var.temporary()) {
        var = ++temp_;
        variable(statement->identifier(), var);
    }

    block_->instr(MOV, var, value, 0);
}

void BasicBlockGenerator::operator()(Return* statement) {
    if (!dynamic_cast<Empty*>(statement->expression())) {
        Operand value = emit(statement->expression());
        if (machine_->return_regs()) {
            // Return the value by register, if the architecture supports
            // return by register
            int reg = 0;
            block_->instr(MOV, -machine_->return_reg(reg)->id(), value, 0); 
        } else {
            // Otherwise, return on the stack.
            push(value);
        }
    }
    ret();
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
    emit(basic_block());
    stack_.clear();
    enter_scope();

    // Pop the formal parameters off the stack in normal order, and save them
    // to a temporary.
    int index = 0;
    for (Formal* f = feature->formals(); f; f = f->next()) {
        if (index >= machine_->arg_regs()) {
            // Variable is passed on the stack
            stack(f->name(), stack_.size()); 
        } else {
            // Variable is passed by register; precolor the temporary for this
            // formal parameter by using a negative number.
            variable(f->name(), mov(-machine_->arg_reg(index)->id()));
        }
        index++;
    } 
    
    // Generate code for the body of the function.
    emit(feature->block());


    if (feature->type()->equals(environment_->void_type())) {
        ret(); 
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

void BasicBlockGenerator::emit_operator(Dispatch* expression) {
    // FixMe: Replace this with a mini-parser that can read three-address-code
    // and output it as an inline function.
    std::string id = expression->identifier()->string();

    std::vector<Operand> args;
    for (Expression::Ptr a = expression->arguments(); a; a = a->next()) {
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
        assert(!"Not implemented");
    }
}

BasicBlock* BasicBlockGenerator::basic_block() {
    BasicBlock* block = new BasicBlock();
    block->label(environment_->name("l" + stringify(++label_)));
    return block;
}


Operand BasicBlockGenerator::variable(String* name) {
    vector<map<String::Ptr, Operand> >::reverse_iterator i;
    for (i = variable_.rbegin(); i != variable_.rend(); i++) {
        map<String::Ptr, Operand>::iterator j = i->find(name);        
        if (j != i->end()) {
            return j->second;
        }
    }
    return Operand();
}

int BasicBlockGenerator::stack(String* name) {
    map<String::Ptr, int>::iterator i = stack_.find(name);
    if (i != stack_.end()) {
        return i->second;
    } else {
        return 0;
    }
    
}

void BasicBlockGenerator::variable(String* name, Operand temporary) {
    assert(variable_.size());
    variable_.back().insert(make_pair(name, temporary));
}

void BasicBlockGenerator::stack(String* name, int offset) {
    stack_.insert(make_pair(name, offset));
}

void BasicBlockGenerator::enter_scope() {
    variable_.push_back(map<String::Ptr, Operand>());
}

void BasicBlockGenerator::exit_scope() {
    variable_.pop_back();
}
