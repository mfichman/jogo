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

#include "IrGenerator.hpp"
#include "IrBlockPrinter.hpp"
#include "DispatchTableGenerator.hpp"
#include <cassert>
#include <algorithm>
#include <stdint.h>

IrGenerator::IrGenerator(Environment* env, Machine* mach) :
    env_(env),
    machine_(mach),
    local_slots_(0),
    arg_slots_(0),
    invert_branch_(false),
    invert_guard_(false),
    temp_(0),
    label_(0) {
}

void IrGenerator::operator()(File* file) {
    if (env_->errors()) {
        return;
    }

    for (Feature::Itr f = file->features(); f; ++f) {
        f(this); 
    }
}

void IrGenerator::operator()(Class* feature) {
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

void IrGenerator::operator()(Formal* formal) {
    
}

void IrGenerator::operator()(StringLiteral* expr) {
    // Load a pointer to the string from the string table.  Strings must
    // always be loaded first, since they are specified by address.
    return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(NilLiteral* expr) {
    return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(IntegerLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(FloatLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(BooleanLiteral* expr) {
    // Load the literal value with load-immediate
    return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(HashLiteral* expr) {
    // Generate code for a hash literal
    Location loc = expr->location();
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr ctor = clazz->constructor();
    Function::Ptr insert = clazz->function(env_->name("@insert"));
    FuncMarshal fm(this);
    fm.call(Operand(ctor->label()));
    IrValue::Ptr array = pop_ret(expr->type());
    for (Expression* arg = expr->arguments(); arg; arg = arg->next()) {
        Construct::Ptr pair = dynamic_cast<Construct*>(arg);
        assert(pair && "Non-pair constructor in hash literal");
        FuncMarshal fm(this);
        IrValue::Ptr key = emit(pair->arguments());
        IrValue::Ptr val = emit(pair->arguments()->next());
        fm.arg(array->operand());
        fm.arg(key->operand());
        fm.arg(val->operand());
        fm.call(Operand(insert->label())); 
    }
    return_ = array; 
}

void IrGenerator::operator()(ArrayLiteral* expr) {
    // Generate code for an array literal
    Location loc = expr->location();
    int elems = 0;
    for (Expression* arg = expr->arguments(); arg; arg = arg->next()) {
        elems++; 
    }
    Function::Ptr ctor = expr->type()->clazz()->constructor();
    Function::Ptr push = expr->type()->clazz()->function(env_->name("push"));
    String::Ptr size = env_->integer(stringify(elems));
    FuncMarshal fm(this);
    fm.arg(load(Operand(new IntegerLiteral(loc, size))));
    fm.call(Operand(ctor->label()));
    IrValue::Ptr array = pop_ret(expr->type());
    for (Expression* arg = expr->arguments(); arg; arg = arg->next()) {
        FuncMarshal fm(this);
        IrValue::Ptr item = emit(arg);
        fm.arg(array->operand());
        fm.arg(item->operand());
        fm.call(Operand(push->label()));
    }
    return_ = array;
}

void IrGenerator::operator()(Box* expr) {
    // Boxes a value type into an object, by allocating memory on the stack
    // for the value type.  This function allocates and initializes a refcount
    // and vtable pointer for the value type.
    Location loc = expr->location();
    IrValue::Ptr arg = emit(expr->child()); 
    assert(!expr->child()->type()->is_compound() && "Not implemented for values");
    assert(!expr->child()->type()->is_ref());

    Class::Ptr clazz = expr->child()->type()->clazz();
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr one = env_->integer("1");
    String::Ptr size = env_->integer(stringify(bytes));
    FuncMarshal fm(this);
    fm.arg(load(Operand(new IntegerLiteral(loc, size))));
    fm.arg(load(Operand(new IntegerLiteral(loc, one))));
    fm.call(Operand(env_->name("Boot_calloc")));
    return_ = pop_ret(expr->type());

    // Initialize the vtable pointer
    Operand vtable(return_->operand().reg(), Address(0));
    Operand vtablel(env_->name(clazz->label()->string()+"__vtable"));
    Operand label = load(vtablel);
    store(vtable, label);

    // FixMe: Eventually, it should be possible to call virtual functions
    // on boxed primitives.  That, however, would require generating special
    // versions of all the basic operators, or adding thunks.  For now,
    // disallow this.
    //String::Ptr zero = env_->integer("0");
    //store(vtable, load(new IntegerLiteral(Location(), zero)));
    
    // Make sure that the refcount starts out at 1, otherwise the object may
    // be freed before the end of the constructor.
    Operand refcount = Operand(return_->operand().reg(), Address(1));
    store(refcount, Operand(new IntegerLiteral(loc, one)));

    // Slot 2 is the value slot for primitive types
    Operand addr = Operand(return_->operand().reg(), Address(2));
    store(addr, arg->operand());
    // FixMe: For value types, can't do a simple move
}

void IrGenerator::operator()(Cast* expr) {
    // Check to make sure that the vtable of 'arg' and 'type' match.  If they
    // don't then return the nil pointer.
    IrValue::Ptr arg = emit(expr->child());
    if (expr->type()->is_any()) {
        return_ = new IrValue(this, arg->operand(), expr->type(), arg->flags());
        arg->is_dead(true);
        return;
    }
    Class::Ptr clazz = expr->type()->clazz();
    Operand vtable1 = load(Operand(arg->operand().reg(), Address(0)));
    Operand vtablel(env_->name(clazz->label()->string()+"__vtable"));
    Operand vtable2 = load(vtablel);
    Operand out(RegisterId(temp_++, 0));

    IrBlock::Ptr mismatch_block = ir_block();
    IrBlock::Ptr ok_block = ir_block();
    IrBlock::Ptr done_block = ir_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    // If the vtable pointers are not equal, set the register to zero
    label(mismatch_block);
    String::Ptr zero = env_->integer("0");
    Location loc;
    mov(out, load(Operand(new IntegerLiteral(loc, zero))));
    jump(done_block);

    // Otherwise, keep the same value in the result.
    label(ok_block);
    if (clazz->is_primitive()) {
        // Slot 2 is the value slot for primitive types
        load(out, Operand(arg->operand().reg(), Address(2)));
    } else if (clazz->is_ref()) {
        mov(out, arg->operand());
    } else if (clazz->is_compound()) {
        assert(!"Not implemented");
    } else {
        assert(!"Invalid type");
    }
    // Transfer ownership from arg => out.  Kill arg so that the value isn't
    // double-freed.
    label(done_block);
    return_ = new IrValue(this, out, expr->type(), arg->flags());
    arg->is_dead(true);
}

void IrGenerator::operator()(Is* expr) {
    // Emit code to check whether the expression type is equal to the cast
    // type, and then return true/false depending
    IrValue::Ptr arg = emit(expr->child());

    Class::Ptr clazz = expr->check_type()->clazz();
    Operand vtable1 = load(Operand(arg->operand().reg(), Address(0)));
    Operand vtablel(env_->name(clazz->label()->string()+"__vtable"));
    Operand vtable2 = load(vtablel);
    Operand out(RegisterId(temp_++, 0));

    IrBlock::Ptr mismatch_block = ir_block();
    IrBlock::Ptr ok_block = ir_block();
    IrBlock::Ptr done_block = ir_block();

    // Emit vtable pointer comparison
    be(vtable1, vtable2, ok_block, mismatch_block);

    Location loc;

    // If the vtable pointers are not equal, set the register to zero
    label(mismatch_block);
    String::Ptr zero = env_->integer("0");
    mov(out, load(Operand(new BooleanLiteral(loc, zero))));
    jump(done_block);

    // Otherwise, set the register to 1.
    label(ok_block);
    String::Ptr one = env_->integer("1");
    mov(out, load(Operand(new BooleanLiteral(loc, one))));
    label(done_block);

    return_ = new IrValue(this, out, expr->type()); 
}

void IrGenerator::operator()(Binary* expr) {
    // Emit the left and right exprs, then perform the operation on
    // the two operands, and return the result.

    if (env_->name("and") == expr->operation()) {
        IrBlock::Ptr left_true = ir_block();
        if (!true_) {
            false_ = top_false_ = ir_block();
            true_ = top_true_ = ir_block();
        }
        
        // Don't use the 'real' true branch; on true we want to emit the    
        // test for the second side of the and
        IrValue::Ptr left = emit(expr->left(), left_true, false_, false);
        if (!block_->is_terminated()) {
            if (invert_guard_) {
                bnz(left->operand(), false_, left_true);
            } else {
                bz(left->operand(), false_, left_true);
            }
        }
        invert_guard_ = false;
        label(left_true); 
        return_ = emit(expr->right(), true_, false_, invert_branch_);
    } else if (env_->name("or") == expr->operation()) {
        IrBlock::Ptr left_false = ir_block();
        if (!true_) {
            false_ = top_false_ = ir_block();
            true_ = top_true_ = ir_block();
        }
        
        // Don't use the 'real' false branch; on false we want to emit
        // the test for the second side of the or
        IrValue::Ptr left = emit(expr->left(), true_, left_false, true);
        if (!block_->is_terminated()) {
            if (invert_guard_) {
                bz(left->operand(), true_, left_false);
            } else {
                bnz(left->operand(), true_, left_false);
            }
        }
        invert_guard_ = false;
        label(left_false);
        return_ = emit(expr->right(), true_, false_, invert_branch_);
    } else {
        assert(!"Unsupported binary operation");
    }
}

void IrGenerator::operator()(Unary* expr) {
    // Emit the code for the expression, then perform the operation on the
    // operand and return the result.

    if (env_->name("not")) {
        // Swap the false and true branches while calling the child, since 
        // the 'not' inverts the conditional 
        if (!true_) {
            false_ = top_false_ = ir_block();
            true_ = top_true_ = ir_block();
        }
        emit(expr->child(), false_, true_, !invert_branch_);
        invert_guard_ = !invert_guard_;
    } else {
        assert(!"Unsupported unary operation");
    }
}

void IrGenerator::operator()(Member* expr) {
    // A stand-alone member operator, which means that we indirectly call the
    // getter.
    Function::Ptr func = expr->function();
    call(func, expr->expression(), expr->expression(), expr->type());
}

void IrGenerator::operator()(Call* expr) {
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
    call(expr->function(), expr->arguments(), expr->receiver(), expr->type());
}

void IrGenerator::operator()(Closure* expr) {
    // Just emit the construct expression for the closure.
    Construct::Ptr construct = expr->construct();
    return_ = emit(construct);
}

void IrGenerator::operator()(Construct* expr) {
    // Look up the function by name in the current context.
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->constructor();

    // Push objects in anticipation of the call instruction.  Arguments must
    // be pushed in reverse order.
    FuncMarshal fm(this);
    IrValue::Ptr valret;
    if(clazz->is_compound()) {
        // Push the 'self' parameter for the Vector constructor
        valret = stack_value_temp(expr->type());
        fm.arg(valret->operand());
    } else {
        assign_loc_ = Operand();
    }
    Formal::Ptr formal = func->formals();
    std::vector<IrValue::Ptr> vals;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        Type::Ptr ft = formal->type();
        IrValue::Ptr val = emit(a);
        vals.push_back(val);
        fm.arg(val->operand());
        formal = formal->next();
    }

    // Insert a call expression, then pop the return value off the stack.
    fm.call(Operand(func->label()));
    if (expr->type()->is_primitive()) {
        return_ = pop_ret(expr->type());
    } else if (expr->type()->is_ref()) {
        return_ = pop_ret(expr->type());
    } else if (expr->type()->is_compound()) {
        return_ = valret;
    } else {
        assert(!"Invalid type");    
    } 
}

void IrGenerator::operator()(Constant* expr) {
    // Nothing here, because constants get output in the scope of the main
    // block.
}

void IrGenerator::operator()(ConstantRef* expr) {
    Operand op = load(Operand(expr->constant()->label(), Address(0)));
    if (expr->type()->is_float()) {
        op.reg(RegisterId(op.reg().id(), RegisterId::FLOAT));
    }
    return_ = new IrValue(this, op, expr->type());
}

void IrGenerator::operator()(IdentifierRef* expr) {
    // Simply look up the value of the variable as stored previously.
    return_ = id_operand(expr->identifier());
}

void IrGenerator::operator()(Empty* expr) {
    // Do nothing
    Operand op = load(Operand(env_->integer("0")));
    return_ = new IrValue(this, op, expr->type(), IrValue::DEAD);
}

void IrGenerator::operator()(Block* stmt) {
    Expression::Ptr parent = stmt->parent();
    bool const is_void = (!parent || parent->type()->is_void());

    IrValue::Ptr out;
    if (is_void) {
        // Nothing
    } else if (!!assign_loc_) {
        out = new IrValue(this, assign_loc_, stmt->type(), IrValue::DEAD);
    } else {
        out = assign_loc_alloc(stmt->type(), 0);
    }
    assign_loc_ = Operand();

    enter_scope();
    for (Expression::Ptr s = stmt->children(); s; s = s->next()) {
        if (!is_void && !s->next()) {
            assign_loc_ = out->operand();
        }
        return_ = emit(s);
        if (!is_void && !s->next()) {
            assign(return_, out); 
        }
        return_ = 0;
    }
    if (is_void) {
        return_ = new IrValue(this, Operand(), stmt->type(), IrValue::DEAD);
    } else {
        return_ = out;
    }
    exit_scope();
}

void IrGenerator::operator()(Let* stmt) {
    // Enter a new scope, then emit code for initializing all the let
    // variables, and initialize code for the body.
    Operand assign_loc = assign_loc_;
    assign_loc_ = Operand();
    enter_scope();
    for (Expression::Ptr v = stmt->variables(); v; v = v->next()) {
        return_ = emit(v);
    } 
    assign_loc_ = assign_loc;
    emit(stmt->block());
    exit_scope();
}

void IrGenerator::operator()(While* statement) {
    // Emit the guard expression in a new basic block
    IrBlock::Ptr body_block = ir_block();
    IrBlock::Ptr done_block = ir_block();

    IrBlock::Ptr guard_block;
    if (block_->instrs()) {
        guard_block = ir_block();
        label(guard_block);
    } else {
        guard_block = block_;
    }

    // Recursively emit the boolean guard expression.  
    invert_guard_ = false;
    IrValue::Ptr guardv = emit(statement->guard(), body_block, done_block, false);
    Operand guard = guardv->operand();
    assert(!guardv->type()->is_compound());
    guardv = 0;
    return_ = new IrValue(this, guard, env_->bool_type());
    if (!block_->is_terminated()) {
        if (invert_guard_) {
            bnz(guard, done_block, body_block);
        } else {
            bz(guard, done_block, body_block);
        }
    }

    // Now emit the body of the loop
    label(body_block);
    emit(statement->block());

    // Jump back up to re-evaluate the guard expression
    jump(guard_block);
    label(done_block);    

    return_ = new IrValue(this, Operand(), env_->void_type());
}

void IrGenerator::operator()(Conditional* stmt) {
    // Emit a conditional if/then expression with boolean guard expr.

    IrBlock::Ptr then_block = ir_block();
    IrBlock::Ptr else_block;
    IrBlock::Ptr done_block = ir_block();
    if (stmt->false_branch()) {
        else_block = ir_block();
    } else {
        else_block = done_block;
    }

    // FixMe: This breaks SSA form, because the output is assigned multiple
    // times.  With this code the way it is, the generator only generates SSA
    // for temporary expressions.  This limitation is here as a punt on
    // introducing phi-functions until optimizations are needed, since without
    // inter-block optimizations, SSA is not needed anyway.
    //Operand assign_loc = !!assign_loc_ ? assign_loc_ : temp_inc();
    IrValue::Ptr out;
    if (!!assign_loc_) {
        out = new IrValue(this, assign_loc_, stmt->type(), IrValue::DEAD);
    } else {
        out = assign_loc_alloc(stmt->type(), 0);
    }
    assign_loc_ = Operand();

    // Recursively emit the boolean guard expression.  We need to pass the true
    // and the false block pointers so that the correct code is emitted on each
    // branch.
    invert_guard_ = false;
    IrValue::Ptr guardv = emit(stmt->guard(), then_block, else_block, false);
    Operand guard = guardv->operand();
    assert(!guardv->type()->is_compound());
    guardv = 0;
    return_ = new IrValue(this, guard, env_->bool_type());
    if (!block_->is_terminated()) {
        if (invert_guard_) {
            bnz(guard, else_block, then_block);
        } else {
            bz(guard, else_block, then_block);
        }
    }

    // Now emit the true branch of the conditional
    assign_loc_ = out->operand();
    label(then_block);
    emit(stmt->true_branch());

    // Emit the false branch if it exists; make sure to insert a jump before
    // emitting the false branch
    if (stmt->false_branch()) {
        if (!block_->is_terminated()) {
            jump(done_block);
        }
        assign_loc_ = out->operand();
        label(else_block);
        emit(stmt->false_branch()); 
    }
    label(done_block);

    if (stmt->type()->is_void()) {
        return_ = new IrValue(this, Operand(), env_->void_type());
    }
}

void IrGenerator::operator()(Assignment* expr) {
    // FixMe: This breaks SSA form, because variables are not renamed.  With
    // this code the way it is, the generator only generates SSA for temporary
    // expressions.  This limitation is here as a punt on introducing
    // phi-functions until optimizations are needed, since without
    // optimizations, SSA is not needed anyway.

    String::Ptr id = expr->identifier();
    IrValue::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    Type::Ptr decl = expr->declared_type();
    Expression::Ptr init = expr->initializer();
    bool is_secondary = var && !expr->is_let() && decl->is_top();
    bool is_attr = attr;

    Type::Ptr type;

    if (is_attr) {
        type = attr->type();
        Operand self = variable(env_->name("self"))->operand();
        if (type->is_primitive()) {
            RegisterId reg = temp_inc();
            if (type->is_float()) {
                reg = RegisterId(reg.id(), RegisterId::FLOAT);
            }
            var = new IrValue(this, reg, type, IrValue::VAR|IrValue::DEAD);
        } else if (type->is_ref()) {
            if (!attr->is_weak()) {
                Operand addr = Operand(self.reg(), Address(attr->slot())); 
                refcount_dec(load(addr));
            }
            var = new IrValue(this, temp_inc(), type, IrValue::VAR|IrValue::DEAD);
        } else if (type->is_compound()) {
            Address addr(attr->slot(), 0);
            Operand op = load(Operand(self.reg(), addr));
            var = new IrValue(this, op, type, IrValue::DEAD);
            // Load effective address!
            value_dtor(var->operand(), type); // Delete old value
        }
        assign_loc_ = var->operand();
    } else if (is_secondary) {
        type = var->type();
        if (type->is_primitive()) {
            // Pass
        } else if (type->is_ref()) {
            // Pass
        } else if (type->is_compound()) {
            value_dtor(var->operand(), type); // Delete old value 
        }
        assign_loc_ = var->operand(); 
    } else {
        type = (decl && !decl->is_top()) ? decl.pointer() : init->type();
        var = assign_loc_alloc(type, IrValue::VAR); 
        assign_loc_ = var->operand();
    }

    if (dynamic_cast<Empty*>(init.pointer())) {
        String::Ptr value;
        if (type->is_enum()) {
            value = type->clazz()->default_enum_value();
        } else {
            value = env_->integer("0");
        }
        Operand op = load(Operand(new IntegerLiteral(Location(), value)));
        return_ = new IrValue(this, op, type);
    } else if (type->is_compound()) {
        return_ = emit(init);
    } else if (type->is_primitive()) {
        return_ = copy(emit(init));
    } else {
        return_ = emit(init);
    }
    return_->is_var(true);

    if (is_attr) {
        Operand self = variable(env_->name("self"))->operand();
        if (type->is_primitive()) {
            Operand addr = Operand(self.reg(), Address(attr->slot()));  
            store(addr, return_->operand());
        } else if (type->is_ref()) {
            Operand addr = Operand(self.reg(), Address(attr->slot()));  
            store(addr, return_->operand());
            if (!attr->is_weak()) {
                refcount_inc(return_->operand());
            }
        } else if (type->is_compound()) {
            assign(return_, var);
        }
    } else if (is_secondary) {
        if (return_ != var && type->is_ref()) {
            refcount_dec(var->operand()); // Delete old value
        }
        assign(return_, var);
    } else {
        assign(return_, var);
        variable(IrVariable(id, var));
    }
    assert(type);
    assign_loc_ = Operand();
}

void IrGenerator::operator()(Return* statement) {
    Expression::Ptr expr = statement->expression();
    Type::Ptr type = expr->type();
    IrValue::Ptr ret;
    if (!dynamic_cast<Empty*>(expr.pointer())) {
        // Don't actually return right away; store the result in a pseudo-var
        // and return it after cleanup.
        if (expr->type()->is_compound()) {
            IrValue* out = id_operand(env_->name("ret"));
            assign_loc_ = out->operand();
            ret = emit(expr);
            assign(ret, out);
        } else if (expr->type()->is_ref()) {
            ret = emit(expr);
            if (ret->is_param()) {
                refcount_inc(ret->operand());
            }
        } else {
            ret = emit(expr);
        }
        ret->is_dead(true);
        return_ = new IrValue(this, Operand(), env_->void_type());
    } else {
        ret = new IrValue(this, Operand(), env_->void_type());
        return_ = ret;
    }
    scope_.back()->ret(ret);
}

void IrGenerator::operator()(Match* stmt) {
    // Emit one conditional for each branch in the match statement.
    IrValue::Ptr guardv = emit(stmt->guard());
    Operand guard = guardv->operand();
    assert(!guardv->type()->is_compound());
    guardv = 0;
    return_ = new IrValue(this, guard, env_->bool_type());

    IrBlock::Ptr done_block = ir_block();  
    IrBlock::Ptr next_block = ir_block();

    IrValue::Ptr out;
    if (!!assign_loc_) {
        out = new IrValue(this, assign_loc_, stmt->type(), IrValue::DEAD);
    } else {
        out = assign_loc_alloc(stmt->type(), 0);
    }
    assign_loc_ = Operand();

    for (Expression::Ptr t = stmt->cases(); t; t = t->next()) {
        label(next_block);
        enter_scope();

        Case::Ptr branch = static_cast<Case*>(t.pointer());
        if(dynamic_cast<Empty*>(branch->guard())) {
            assign_loc_ = out->operand();
            emit(branch->block());
            break; 
            // The wildcard pattern prevents all following patterns from
            // executing because it always matches
        }

        IrBlock::Ptr true_block = ir_block();
        if (t->next()) {
            next_block = ir_block();
        } else {
            next_block = done_block;
        }
        
        // Emit the guard expression for this branch.
        Operand result = emit(branch->guard())->operand();  
        bne(guard, result, next_block, true_block);
        label(true_block);

        // Now emit the statements to be run if this branch is true, then jump
        // to the end of the case statement.
        assign_loc_ = out->operand();
        emit(branch->block());
        exit_scope();
        if (!block_->is_terminated() && t->next()) {
            jump(done_block); 
            // Omit the jump if the block is already terminated (e.g., by a
            // return) or if this is the last case
        }
    }
    label(done_block);

    if (stmt->type()->is_void()) {
        return_ = new IrValue(this, Operand(), env_->void_type());
    }
}

void IrGenerator::operator()(Case* statement) {
    // Pass, handled by match
}

void IrGenerator::operator()(Fork* statement) {
    assert(!"Not implemented");
}

void IrGenerator::operator()(Yield* statament) {
    FuncMarshal fm(this);
    fm.call(Operand(env_->name("Coroutine__yield")));
	exception_catch();
}

void IrGenerator::operator()(Function* feature) {
    // If the function is just a prototype, don't emit any code.
    if (!feature->block() || feature->is_native()) { return; }
    if (class_ && class_->is_interface()) { return; }

    // Reset the temporaries for the function.
    feature->ir_block_del_all();
    temp_ = machine_->regs();
    function_ = feature;
    block_ = 0;
    assign_loc_ = Operand();
    label(ir_block());
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
    if (feature->label()->string() == "Boot_main") {
        constants();
    }

    // Generate code for the body of the function.
    emit(feature->block());
    if (!block_->is_ret()) {
        if (function_->is_constructor()) {
            func_return(variable(env_->name("self")));
        } else if (feature->type()->is_void()) {
            func_return(0);
        } else {
        }
    }
    exit_scope();
    feature->temp_regs(temp_);
    assert(local_slots_ == 0 && "Invalid stack alloc");
    assert(arg_slots_ == 0 && "Invalid stack alloc");
}

void IrGenerator::operator()(Attribute* feature) {
    // Pass, handled by constructor
}

void IrGenerator::operator()(Import* feature) {
    // Pass
}

void IrGenerator::operator()(Type* feature) {
    // Pass
}

void IrGenerator::call(Function* func, Expression* args, Expression* recv, Type* ret) {
    // Push objects in anticipation of the call instruction.  Arguments must be
    // pushed in reverse order.
    if (recv) {
        // Look up the function again, in case it needs to re-resolve.  This is
        // necessary for composition, where the first lookup returns the func
        // from the embedded type, and the second lookup returns the generated stub
        // in the composite type.
        Class::Ptr clazz = recv->type()->clazz();
        func = clazz->function(func->name());
    }

    Formal::Ptr formal = func->formals();
    FuncMarshal fm(this);
    Operand receiver;
    IrValue::Ptr valret;
    if (ret->is_compound()) {
        valret = stack_value_temp(ret);
    } else {
        assign_loc_ = Operand();
    } 
    std::vector<IrValue::Ptr> vals;
    for (Expression::Ptr a = args; a; a = a->next()) {
        IrValue::Ptr val = emit(a);
        vals.push_back(val);
        fm.arg(val->operand());
        if(a == args) {
            receiver = val->operand();
        }
        formal = formal->next();
    }
    if (ret->is_compound()) {
        // Push a pointer for the return value at the end of the argument list.
        // The returned value type data will be stored at that location on the
        // stack.
        fm.arg(valret->operand());
    }

    IrValue::Ptr fnptr;
    if (func->is_virtual()) {
        // Dynamic dispatch: call the object dispatch function with the
        // receiver and function name as arguments.
        String::Ptr name = env_->string(func->name()->string());
        FuncMarshal fm(this);
        fm.arg(receiver); // Receiver
        fm.arg(load(Operand(new StringLiteral(Location(), name))));
        fm.call(Operand(env_->name("Object__dispatch")));
        fnptr = pop_ret(env_->int_type());
    } 

    if (func->is_virtual()) {
        // Dynamic dispatch: call the function pointer.
        fm.call(fnptr->operand());
    } else {
        // Static dispatch: insert a call expression, then pop the return value
        // off the stack
        fm.call(Operand(func->label()));
    }

    if (ret->is_void()) {
        valret = new IrValue(this, Operand(), env_->void_type());
    } else if (ret->is_primitive()) {
        valret = pop_ret(ret); 
    } else if (ret->is_compound()) {
        // Return value is allocated on the stack already, and no value is
        // returned by register.
    } else if (ret->is_ref()) {
        valret = pop_ret(ret);
    } else {
        assert(!"Invalid type");
    }
    vals.clear();
	if (func->throw_spec() == Function::THROW) {
		exception_catch();
	}
    return_ = valret;
}

void IrGenerator::exception_catch() {
	// If the function can throw an exception, then generate the landing pad and
	// return.  Currently, only an exception throw is allowed, to support cleaning
	// up coroutines.
	String::Ptr name = env_->name("Exception__current");
	Constant::Ptr static current(new Constant(Location(), env_, name, 0, 0, 0));

	Operand val = load(Operand(name, Address(0)));

	// Branch to the continuation if there was no exception.
	IrBlock::Ptr except_block = ir_block();
	IrBlock::Ptr done_block = ir_block(); 
	bz(val, done_block, except_block);

	// Handle the exception by returning from the function.
	label(except_block);
	func_return(0); // Return no value b/c this is not a normal return.

	// Start a new code block that continues after the exception check.
	label(done_block);
}

void IrGenerator::native_operator(Call* expr) {
    // FixMe: Replace this with a mini-parser that can read three-address-code
    // and output it as an inline function.
    std::string id = expr->function()->name()->string();
    std::vector<IrValue::Ptr> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }
    Operand op1 = args[0]->operand();
    if (id == "@add") {
        Operand op2 = args[1]->operand();
        assert(args.size() == 2);
        return_ = new IrValue(this, add(op1, op2), expr->type()); 
    } else if (id == "@sub") {
        Operand op2 = args[1]->operand();
        assert(args.size() == 2);
        return_ = new IrValue(this, sub(op1, op2), expr->type());
    } else if (id == "@mul") {
        Operand op2 = args[1]->operand();
        assert(args.size() == 2);
        return_ = new IrValue(this, mul(op1, op2), expr->type());
    } else if (id == "@div") {
        Operand op2 = args[1]->operand();
        assert(args.size() == 2);
        return_ = new IrValue(this, div(op1, op2), expr->type());
    } else if (id == "@neg") {
        assert(args.size() == 1);
        return_ = new IrValue(this, neg(op1), expr->type());
    } else if (id == "@mod") {
        Operand op2 = args[1]->operand();
        assert(!"Not implemented");
    } else if (id == "@compl") {
        assert(!"Not implemented");
    } else if (id == "@equal") {
        Operand op2 = args[1]->operand();
        if (!true_) {
            false_ = top_false_ = ir_block();
            true_ = top_true_ = ir_block();
        }
        if (invert_branch_) {
            be(op1, op2, true_, false_);
        } else {
            bne(op1, op2, false_, true_);
        }
    } else if (id == "@less") {
        Operand op2 = args[1]->operand();
        if (!true_) {
            false_ = top_false_ = ir_block();
            true_ = top_true_ = ir_block();
        }
        if (invert_branch_) {
            bl(op1, op2, true_, false_);
        } else {
            bge(op1, op2, false_, true_);
        } 
    } else {
        assert(!"Not implemented");
    }
}

IrBlock* IrGenerator::ir_block() {
    IrBlock* block = new IrBlock();
    block->label(env_->name(".L" + stringify(++label_)));
    return block;
}


IrValue* IrGenerator::variable(String* name) {
    // Look up the variable, starting in the current scope and continuing up
    // the scope stack.
    std::vector<IrScope::Ptr>::reverse_iterator i;
    for (i = scope_.rbegin(); i != scope_.rend(); i++) {
        IrValue* var = (*i)->variable(name);
        if (var) {
            return var;
        }
    }
    return 0;
}

void IrGenerator::variable(IrVariable const& var) {
    assert(scope_.size());
    assert(var.value()->is_var());
    scope_.back()->variable(var);
}

void IrGenerator::enter_scope() {
    scope_.push_back(new IrScope);
}

void IrGenerator::exit_scope() {
    // Pops the symbol table for this scope off the stack, and inserts code
    // to perform cleanup at the end of the scope.
    IrValue::Ptr ret = scope_.back()->ret();

	// Remove variables in reverse order!
    scope_.pop_back();

    if (ret) {
        // Emit code to destroy variables in all the enclosing parent scopes as
        // well; otherwise, objects in those enclosing scopes will not be
        // cleaned up.  Then, emit the 'ret' instruction.
        if (function_->is_constructor()) {
            assert("Bad return val for constructor" && ret->type()->is_void());
            func_return(variable(env_->name("self")));
        } else {
            func_return(ret);
        } 
    }
}

IrValue::Ptr IrGenerator::stack_value(Type* type) {
    // Allocate space for a local value-type variable or temporary on the
    // stack, and return the address of the allocated space.
    calculate_size(type->clazz());
    local_slots_inc(type->clazz()->slots());
    return new IrValue(this, load(Address(-local_slots_, 0)), type);  
    // Translates to LEA
}

IrValue::Ptr IrGenerator::stack_value_temp(Type* type) {
    // Allocates storage for a temporary stack variable.
    IrValue::Ptr val;
    if (!!assign_loc_) {
        val = new IrValue(this, assign_loc_, type, IrValue::DEAD);
    } else {
        val = stack_value(type);
    }
    assign_loc_ = Operand();
    return val;
}

IrValue::Ptr IrGenerator::id_operand(String* id) {
    // Returns an operand for the given id that may be used in the current
    // scope.  Assert if the lookup fails.
    IrValue::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    if (var && !var->operand().reg()) {
        // Variable can't be found in a temporary; it must be an argument
        // passed on the stack.
        assert(!!var->operand().addr() && "No address or reg for operand");
        Operand op = load(Operand(var->operand().addr())); 
        if (var->operand().is_float()) {
            op = Operand(RegisterId(op.reg().id(), RegisterId::FLOAT));
        }
        IrValue::Flags flags = IrValue::DEAD|IrValue::VAR|IrValue::PARAM;
        IrValue::Ptr val = new IrValue(this, op, var->type(), flags);
        variable(IrVariable(id, val));
        return val;
    } else if (var && !!var->operand().reg()) {
        return var;
    } else if (attr) {
        Operand self = variable(env_->name("self"))->operand();
        if (attr->type()->is_compound()) {
            IrValue::Flags flags = IrValue::DEAD|IrValue::VAR|IrValue::PARAM;
            Operand op = load(Operand(self.reg(), Address(attr->slot(), 0)));
            return new IrValue(this, op, attr->type(), flags);
            // Basically an LEA
        } else if (attr->type()->is_float()) {
            IrValue::Flags flags = IrValue::DEAD|IrValue::VAR|IrValue::PARAM;
            Operand op = load(Operand(self.reg(), Address(attr->slot())));
            op.reg(RegisterId(op.reg().id(), RegisterId::FLOAT));
            return new IrValue(this, op, attr->type(), flags);
        } else {
            IrValue::Flags flags = IrValue::DEAD|IrValue::VAR|IrValue::PARAM;
            Operand op = load(Operand(self.reg(), Address(attr->slot())));
            return new IrValue(this, op, attr->type(), flags);
        }
    } else {
        assert(!"IdentifierRef not found");
    }
    return 0;
}

void IrGenerator::bool_expr(Expression* expr) {
    // Emits a boolean expression with short-circuiting, and stores the result
    // in a fixed operand.  Note:  This breaks SSA form, because a value gets
    // assigned different values on different branches.
    if (dynamic_cast<BooleanLiteral*>(expr)) {
        return_ = new IrValue(this, load(Operand(expr)), expr->type(), IrValue::DEAD);
        return;
    }

    // Recursively emit the boolean guard expression.
    IrBlock::Ptr top_true_save = top_true_;
    IrBlock::Ptr top_false_save = top_false_;
    bool invert_guard_save = invert_guard_;
    invert_guard_ = false;
    top_true_ = 0;
    top_false_ = 0;
    IrValue::Ptr guardv = emit(expr, 0, 0, false);
    Operand guard = guardv->operand();
    if (top_true_) {
        IrBlock::Ptr done_block = ir_block();
        if (!block_->is_terminated()) {
            if (invert_guard_) {
                bnz(guard, top_false_, top_true_);
            } else {
                bz(guard, top_false_, top_true_);
            }
        }
        IrValue::Ptr out = new IrValue(this, RegisterId(++temp_, 0), expr->type());
        Location loc;

    
        // Now emit the code for the 'true' branch, i.e., assign 'true' to the
        // return value.
        label(top_true_);
        String::Ptr one = env_->integer("1");
        mov(out->operand(), load(Operand(new IntegerLiteral(loc, one))));
        jump(done_block);
        
        // Now emit the code for the 'false' branch.
        label(top_false_);
        String::Ptr zero = env_->integer("0");
        mov(out->operand(), load(Operand(new IntegerLiteral(loc, zero))));
        
        label(done_block);
        
        return_ = out;
    }
    top_true_ = top_true_save;
    top_false_ = top_false_save;
    invert_guard_ = invert_guard_save;

/*
    //IrBlock::Ptr then_block = ir_block();
    //IrBlock::Ptr else_block = ir_block();
    //IrBlock::Ptr done_block = ir_block();


    IrValue::Ptr out = new IrValue(this, RegisterId(++temp_, 0), expr->type());
    assert(!guardv->type()->is_compound());
    guardv = 0;
    return_ = new IrValue(this, guard, env_->bool_type());
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
    label(then_block);
    String::Ptr one = env_->integer("1");
    mov(out->operand(), load(Operand(new IntegerLiteral(loc, one))));
    jump(done_block);
    
    // Now emit the code for the 'false' branch.
    label(else_block);
    String::Ptr zero = env_->integer("0");
    mov(out->operand(), load(Operand(new IntegerLiteral(loc, zero))));
    
    label(done_block);
    return_ = out;
*/
}

void IrGenerator::func_return(IrValue* retval) {
    // Search backwards through the vector and clean up variables in the 
    // containing scope.  Do NOT clean up variables in the top scope; those
	// variables are parameters and they are anchored (hence j > 1).
    return_ = new IrValue(this, Operand(), env_->void_type());
    for (int j = scope_.size()-1; j > 0; j--) {
        IrScope::Ptr s = scope_[j];
        for (int i = s->variables()-1; i >= 0; i--) {
            IrValue::Ptr var = s->variable(i).value(); 
            Type::Ptr type = var->type();
            assert("Nil operand" && !!var->operand());
            if (var->is_dead()) {
                // No cleanup need be done.
            } else if (type->is_primitive()) {
                // Do nothing, b/c the primitive has no destructor.
            } else if (type->is_ref()) {
                // Emit a branch to check the variable's reference count and
                // free it if necessary.
                if (var != retval) {
                    refcount_dec(var->operand()); 
                }
            } else if (type->is_compound()) {
                // Call the value type's destructor, if it exists.  Do not
                // decrement the local_slots counter here, because this is a
                // return, not a block exit.
                value_dtor(var->operand(), type);
            } else {
                assert(!"Invalid type");
            }
            // This is similar to scope_cleanup, except that it doesn't
            // decrement the local_slots counter.
        }
    }

    // If this is the destructor, then release all the pointers held by the
    // object, and then call free() to release memory if the object was
    // dynamically allocated.
    if (function_->is_destructor()) {
        dtor_epilog(function_);
    }
    
    if (retval && !retval->type()->is_void()) {
        // Return the value by register, if the architecture supports return by
        // register
        if (retval->operand().is_float() && machine_->float_return_regs()) {
            mov(machine_->float_return_reg(0)->id(), retval->operand()); 
        } else if (!retval->operand().is_float() && machine_->int_return_regs()) {
            mov(machine_->int_return_reg(0)->id(), retval->operand()); 
        } else {
            assert(!"No return register");
        }
    }
    ret();
}

void IrGenerator::refcount_inc(Operand var) {
    // Emit code to increment the reference count for the object specified by
    // 'var'.  Insert a call expression to call the refcount_dec function.  If
    // the variable is already in the temp list, then the current function has
    // already incremented the refcount for that object -- so just remove it
    // from the temp list instead of calling refcount_inc.
    FuncMarshal fm(this);
    fm.arg(var);
    fm.call(Operand(env_->name("Object__refcount_inc")));
}

void IrGenerator::refcount_dec(Operand var) {
    // Emit code to decrement the reference count for the object specified by
    // 'temp'.  Insert a call expression to call the refcount_dec function 
    FuncMarshal fm(this);
    fm.arg(var);
    fm.call(Operand(env_->name("Object__refcount_dec")));
}

IrValue::Ptr IrGenerator::pop_ret(Type* type) {
    // Pops the return value of a call from the stack.
    // FixMe: Add this to FuncMarshal instead
    Register* reg = 0;
    if (type->is_float()) {
        reg = machine_->float_return_reg(0); 
    } else {
        reg = machine_->int_return_reg(0);
    }
    assert(reg && "No return regs for this architecture");
    Operand op;
    if (type->is_float()) {
        op = mov(RegisterId(reg->id().id(), RegisterId::FLOAT));
    } else {
        op = mov(reg->id());
    }
    return new IrValue(this, op, type);
}

void IrGenerator::dispatch_table(Class* feature) {
    // Generate the Pearson perfect hash that will be used for vtable lookups.
    if (!feature->is_object()) {
        return;
    }
    DispatchTableGenerator gen(feature);    
}

void IrGenerator::calculate_size(Class* feature) {
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

void IrGenerator::ctor_preamble(Class* clazz) {
    // Emits the memory alloc/vtable setup for the class.

    // Allocate the memory for the new object by calling calloc with the size
    // of the object.
    IrValue::Ptr self;
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr size = env_->integer(stringify(bytes));
    String::Ptr one = env_->integer("1");
    if (clazz->is_object()) {
        FuncMarshal fm(this);
        fm.arg(load(Operand(new IntegerLiteral(Location(), size))));
        fm.arg(load(Operand(new IntegerLiteral(Location(), one))));
        fm.call(Operand(env_->name("Boot_calloc")));
 
        // Obtain a pointer to the 'self' object, and store it in the 'self'
        // variable.
        self = pop_ret(class_->type()); 
        self->is_var(true);
        self->is_dead(true);
        variable(IrVariable(env_->name("self"), self));
       
        // Initialize the vtable pointer
        Operand vtable = Operand(self->operand().reg(), Address(0));
        Operand vtablel(env_->name(clazz->label()->string()+"__vtable"));
        Operand label = load(vtablel);
        store(vtable, label);
        
        // Make sure that the refcount starts out at 1, otherwise the object may
        // be freed before the end of the constructor.
        Operand refcount = Operand(self->operand().reg(), Address(1));
        store(refcount, Operand(new IntegerLiteral(Location(), one)));
    
    } else if (clazz->is_compound()) {
        self = id_operand(env_->name("self"));
        FuncMarshal fm(this);
        fm.arg(self->operand());
        fm.arg(load(Operand(new IntegerLiteral(Location(), size))));
        fm.call(Operand(env_->name("Boot_mzero")));
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
            if (attr->is_component() && attr->type()->is_object_proto()) {
                continue;
            }
            if (init->type()->is_compound()) {
                Address addr(attr->slot(), 0);
                assign_loc_ = load(Operand(self->operand().reg(), addr));
            }
            IrValue::Ptr value = emit(init);
            if (init->type()->is_primitive()) {
                // Don't need to do anything special for primitives
                Operand addr = Operand(self->operand().reg(), Address(attr->slot()));
                store(addr, value->operand());
            } else if (init->type()->is_ref()) {
                Operand addr = Operand(self->operand().reg(), Address(attr->slot()));
                store(addr, value->operand());
                refcount_inc(value->operand());
            } else if (init->type()->is_compound()) {
                // Pass; alue type will be initialized directly in assign_loc above
            } else {
                assert(!"Invalid type");
            }
        }
    }
}

void IrGenerator::copier_preamble(Class* clazz) {
    // Emits the copy constructor preamble, which adjusts refcounts for
    // reference-type attributes of the value type.
    IrValue::Ptr self = id_operand(env_->name("self"));
    IrValue::Ptr other = id_operand(env_->name("val"));
    
    calculate_size(clazz);
    int bytes = clazz->slots() * machine_->word_size();
    String::Ptr size = env_->integer(stringify(bytes));
    FuncMarshal fm(this);
    fm.arg(self->operand());
    fm.arg(other->operand());
    fm.arg(load(Operand(new IntegerLiteral(Location(), size))));
    fm.call(Operand(env_->name("Boot_memcpy")));

    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer());
        if (attr && attr->type()->is_ref()) {
            Operand addr = Operand(self->operand().reg(), Address(attr->slot())); 
            refcount_inc(load(addr)); 
        }
    }
}

void IrGenerator::dtor_epilog(Function* feature) {
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
            value_dtor(load(addr), attr->type()); 
        } else {
            assert(!"Invalid type");
        }
    }

    if (class_->is_primitive()) {
        // Pass
    } else if (class_->is_ref()) {
        FuncMarshal fm(this);
        fm.arg(variable(env_->name("self"))->operand());
        fm.call(Operand(env_->name("Boot_free")));
    } else if (class_->is_compound()) {
        // Pass
    } else {
        assert(!"Invalid type");
    }
}

void IrGenerator::drop_value(Operand val, Type* type) {
    // Frees a temporary value (what happens here depends on the value's type)
    if (type->is_primitive()) {
        // Nothing
    } else if (type->is_ref()) {
        refcount_dec(val);
    } else if (type->is_compound()) {
        value_dtor(val, type);
        calculate_size(type->clazz());
        local_slots_dec(type->clazz()->slots());
    } else if (type->is_void()) {
        // Nothing
    } else {
        assert(!"Invalid type");
    }
}

void IrGenerator::constants() {
    for (Constant::Itr cons = env_->constants(); cons; ++cons) {
        Expression* init = cons->initializer();
        if(IntegerLiteral::Ptr lit = dynamic_cast<IntegerLiteral*>(init)) {
        } else if(FloatLiteral::Ptr lit = dynamic_cast<FloatLiteral*>(init)) {
        } else if(Empty::Ptr lit = dynamic_cast<Empty*>(init)) {
            // FIXME: Extern constants should be loaded from a per-lib startup
            // func/initialization routine.
        } else {
            //store(Operand(cons->label(), Address(0)), emit(cons->initializer()));
            //FIXME: Allow non-literal constants?
        }
    }
}

void IrGenerator::local_slots_inc(int count) {
    local_slots_ += count;
    function_->local_slots(std::max(function_->local_slots(), local_slots_));
}

void IrGenerator::local_slots_dec(int count) {
    local_slots_ -= count;
    assert(local_slots_ >= 0);
}

void IrGenerator::arg_slots_inc(int count) {
    arg_slots_ += count;
    function_->arg_slots(std::max(function_->arg_slots(), arg_slots_));
}

void IrGenerator::arg_slots_dec(int count) {
    arg_slots_ -= count;
}

void IrGenerator::attr_assignment(Assignment* expr) {
    // Assignment to an attribute within a class
}

void IrGenerator::initial_assignment(Assignment* expr) {
    // Assignment to an attribute within a class
}

void IrGenerator::secondary_assignment(Assignment* expr) {
    // Assignment to an attribute within a class
}

IrValue::Ptr IrGenerator::assign_loc_alloc(Type* type, IrValue::Flags flags) {
    // Allocate a variable/location that an expression should return into.
    if (type->is_primitive()) {
        RegisterId reg = temp_inc();
        if (type->is_float()) {
            reg = RegisterId(reg.id(), RegisterId::FLOAT);
        }
        return new IrValue(this, reg, type, flags);
    } else if (type->is_ref()) {
        return new IrValue(this, temp_inc(), type, flags);
    } else if (type->is_compound()) {
        IrValue::Ptr ret = stack_value(type);
        ret->is_var(true);
        return ret;
    } else if (type->is_void()) {
        return new IrValue(this, Operand(), env_->void_type(), flags);
    } else {
        assert(!"Invalid type");
        return 0;
    }
}

void IrGenerator::assign(IrValue* src, IrValue* dst) {
    // Assign the value pointed to by 'src' to the temporary at 'dst.' Usually,
    // 'dst' is a preallocated location on the stack or in the register file
    // that the result of an expression should be stored in.  If src/dst are
    // not already equal, then this function must make a copy.
    Type* type = src->type();
    if (!dst || src->operand() == dst->operand()) {
        // No destination register; just set return_ = src so that the result
        // of the assignment is available to the parent expression
        return_ = src;
    } else if (!src->is_var()) {
        // The source is not a variable.  Mark the source as dead, create a new
        // value at the given destination, and move the src => dst if necessary
        src->is_dead(true);
        mov(dst->operand(), src->operand());
        return_ = dst;
    } else if (type->is_primitive()) {
        // Create a copy of the primitive
        src->is_dead(true);
        mov(dst->operand(), src->operand());
        return_ = dst;
    } else if (type->is_ref()) {
        // Increment the refcount, since the value is assigned to new storage,
        // thus creating a second anchor
        refcount_inc(src->operand());
        mov(dst->operand(), src->operand());
        return_ = dst;
    } else if (type->is_compound()) {
        // Copy the value type to the given destination
        assert(!!dst && "Invalid assign location");
        value_copy(src->operand(), dst->operand(), type); 
        return_ = dst;
    } else if (type->is_void()) {
        // No return value
    } else {
        assert(!"Invalid type");
    }
}

void IrGenerator::value_copy(Operand src, Operand dst, Type* type) {
    // Copies a value from 'src' to 'dst', and then increments the refcount
    // for any non-weak object attributes in 'type.'
    assert(!!src);
    assert(!!dst);
    Function::Ptr copy = type->clazz()->function(env_->name("@copy"));
    assert(copy && "Missing copy constructor");
    FuncMarshal fm(this);
    fm.arg(dst);
    fm.arg(src);
    fm.call(Operand(copy->label())); 
}

void IrGenerator::value_dtor(Operand op, Type* type) {
    // Calls the destructor for the value type at location "op", where "op"
    // is a register containing the pointer to the value type on the stack.
    Function::Ptr dtor = type->clazz()->destructor();
    assert(dtor && "Missing value type destructor");
    FuncMarshal fm(this);
    fm.arg(op);
    fm.call(Operand(dtor->label()));
}

IrValue::Ptr IrGenerator::emit(Expression* expr, IrBlock* yes, IrBlock* no, bool inv) {
    IrBlock* true_save = true_;
    IrBlock* false_save = false_;
    bool invert_branch_save_ = invert_branch_;
    true_ = yes;
    false_ = no;
    invert_branch_ = inv;
    expr->operator()(this);
    true_ = true_save;
    false_ = false_save;
    invert_branch_ = invert_branch_save_;
    return return_;
}

IrValue::Ptr IrGenerator::emit(Expression* expr) {
    if (expr->type()->is_bool()) {
        bool_expr(expr);
    } else {
        expr->operator()(this);
    }
    return return_;
}

IrValue::Ptr IrGenerator::copy(IrValue* val) {
    Operand op = mov(val->operand());
    return new IrValue(this, op, val->type());
}

Operand IrGenerator::emit(Opcode op, Operand t2, Operand t3) {
    assert("Literal or label in non-mem instr" && !t2.object());
    assert("Literal or label in non-mem instr" && !t3.object());
    assert("Address in non-mem instr" && !t2.addr());
    assert("Address in non-mem instr" && !t3.addr());
    Instruction in = block_->instr(op, temp_inc(), t2, t3);
    return in.result();
}

Operand IrGenerator::emit(Opcode op, Operand t2) {
    assert("Literal or label in non-mem instr" && !t2.object());
    assert("Address in non-mem instr" && !t2.addr());
    Instruction in = block_->instr(op, temp_inc(), t2, Operand());
    return in.result();
}

Operand IrGenerator::mov(Operand t2) {
    assert("Literal or label in non-mem instr" && !t2.object());
    assert("Address in non-mem instr" && !t2.addr());
    assert("Nil operand" && !!t2);
    Instruction in = block_->instr(MOV, temp_inc(), t2, Operand());
    return in.result(); 
}

Operand IrGenerator::mov(Operand res, Operand t2) { 
    // FixMe: This could be a problem for SSA analysis.
    assert("Literal or label in non-mem instr" && !res.object());
    assert("Literal or label in non-mem instr" && !t2.object());
    assert("Address in non-mem instr" && !res.addr());
    assert("Address in non-mem instr" && !t2.addr());
    assert("Nil operand" && !!t2);
    assert("Nil operand" && !!res);
    if (res == t2) {
        return res;
    } else {
        Instruction in = block_->instr(MOV, res, t2, Operand());
        return in.result();
    }
}

void IrGenerator::label(IrBlock* block) {
    if (block_) {
        block_->next(block);
    }
    block_ = block;
    function_->ir_block(block);
}

void IrGenerator::store(Operand addr, Operand value) {
    assert("Can't store a label" && !value.label());
    assert("Stored value must be a reg" && (!!value.reg() || value.literal()));
    assert("Can't use an address offset in a STORE instr" && !value.addr());
    assert("Non-indirect store instruction" && addr.is_indirect());
    block_->instr(STORE, Operand(), addr, value);    
}

Operand IrGenerator::load(Operand addr) {
    Instruction in = block_->instr(LOAD, temp_inc(), addr, Operand());
    return in.result();
}

Operand IrGenerator::load(Operand res, Operand addr) {
    // FIXME: This is a problem for true SSA analysis
    Instruction in = block_->instr(LOAD, res, addr, Operand());
    return in.result();
}

void IrGenerator::call(Operand func) {
    assert("Target must be reg or label" && (!!func.reg() || func.label()));
    assert("Literal in call instruction" && !func.literal());
    block_->instr(CALL, Operand(), func, Operand());
}

void IrGenerator::ret() {
    block_->instr(RET, Operand(), Operand(), Operand());
}

void IrGenerator::branch(Opcode op, Operand t2, Operand t3, IrBlock* branch, IrBlock* next) {
    block_->instr(op, Operand(), t2, t3);
    block_->branch(branch);
    block_->next(next);
}

void IrGenerator::bne(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BNE, t2, t3, target, next);
}

void IrGenerator::be(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BE, t2, t3, target, next);
}

void IrGenerator::bnz(Operand t2, IrBlock* target, IrBlock* next) {
    branch(BNZ, t2, Operand(), target, next);
}

void IrGenerator::bz(Operand t2, IrBlock* target, IrBlock* next) {
    branch(BZ, t2, Operand(), target, next);
}

void IrGenerator::bg(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BG, t2, t3, target, next);
}

void IrGenerator::bl(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BL, t2, t3, target, next);
}

void IrGenerator::bge(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BGE, t2, t3, target, next);
}

void IrGenerator::ble(Operand t2, Operand t3, IrBlock* target, IrBlock* next) {
    branch(BLE, t2, t3, target, next);
}

void IrGenerator::jump(IrBlock* target) {
    block_->instr(JUMP, Operand(), Operand(), Operand());
    block_->branch(target);
}

void FuncMarshal::arg(Operand op) {
    // Add an argument to the list, and keep track of the number of float/int
    // args so that they are assigned to argument registers/stack properly.
	assert(!!op && "Nil argument");
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
#ifdef WINDOWS
            int_arg++;
            // Note: On Windows, registers are assigned ONLY for certain arg
            // numbers, so if there are interleaved args, its possible that the
            // first float/int doesn't even get to use an arg register
#endif
        } else {
            reg = gen_->machine_->int_arg_reg(int_arg);
            int_arg++;
#ifdef WINDOWS
            float_arg++;
#endif
        }
        if (reg) {
            //Operand arg = gen_->mov(arg_[i]);
            // FIXME: This line breaks something somehow.
            Operand arg = arg_[i];
            // A MOV is necessary here, as arg_[i] may not be in a register.
            // The store operation does not work if the argument is not a
            // register.
            gen_->mov(reg->id(), arg); // Pass argument by register
#ifdef WINDOWS
            gen_->arg_slots_inc(1);
            gen_->store(Operand(sp, Address(stack_arg)), reg->id());
            stack_arg++;
            // Windows: also pass the value on the stack
#endif
        } else {
            gen_->arg_slots_inc(1);
            gen_->store(Operand(sp, Address(stack_arg)), gen_->mov(arg_[i]));
            ///gen_->store(Operand(sp, Address(stack_arg)), arg_[i]);
            // FIXME: This line breaks something somehow
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
#ifdef WINDOWS
        int_args_++;
#endif
    } else {
        reg = gen_->machine_->int_arg_reg(int_args_++);
#ifdef WINDOWS
        float_args_++;
#endif
    } 
    IrValue::Flags flags = IrValue::DEAD|IrValue::VAR|IrValue::PARAM;
    if (reg) {
        // Variable is passed by register; precolor the temporary for this
        // formal parameter by using a negative number.  Passing NULL for
		// the type of the variable prevents it from being garbage collected
		// at the end of the scope (which is what we want for function 
		// parameters).
        Operand op = gen_->mov(reg->id());
        IrValue::Ptr val = new IrValue(gen_, op, type, flags);
#ifndef WINDOWS
        gen_->variable(IrVariable(name, val));
#endif

        // On Windows, the value is also passed with a backing location on
        // the stack.
#ifdef WINDOWS
        RegisterId id(0, type->is_float() ? RegisterId::FLOAT : 0);
        //Operand op(id, Address(++stack_args_));
        Operand op2 = gen_->load(Operand(id, Address(++stack_args_)));
        IrValue::Ptr val = new IrValue(gen_, op2, type, flags);
        gen_->variable(IrVariable(name, val));
#endif
    } else {
        RegisterId id(0, type->is_float() ? RegisterId::FLOAT : 0);
        //Operand op(id, Address(++stack_args_));
        Operand op = gen_->load(Operand(id, Address(++stack_args_)));
        IrValue::Ptr val = new IrValue(gen_, op, type, flags);
        gen_->variable(IrVariable(name, val));
    }
}


IrValue::IrValue(IrGenerator* gen, Operand op, Type* type, Flags flags) :
    generator_(gen),
    operand_(op),
    type_(type),
    flags_(flags) {

    assert(type && "Missing type");
}

IrValue::IrValue() :
    generator_(0),
    type_(0),
    flags_(IrValue::DEAD) {

    
}

IrValue::~IrValue() {
    if (!is_dead()) {
        generator_->drop_value(operand_, type_);
        operand_ = Operand();
        type_ = 0;
    }
}

IrValue* IrScope::variable(String* name) const { 
    for (int i = 0; i < variable_.size(); i++) {
        if (variable_[i].name() == name) {
            return variable_[i].value();
        }
    }
    return 0;
}

IrScope::~IrScope() {
    // Delete variables in reverse order of creation
    while (variable_.size()) {
        variable_.pop_back();
    }
}

void ir_dump(Function* func) {
    IrBlockPrinter::Ptr print(new IrBlockPrinter(func->env(), 0));
    print->out(Stream::stout());
    print->operator()(func); 
    Stream::stout()->flush();
}
