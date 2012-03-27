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

#include "CGenerator.hpp"


CGenerator::CGenerator(Environment* env) :
    env_(env) {

}

void CGenerator::operator()(Attribute* attr) {

}

void CGenerator::operator()(Constant* feature) {
}


void CGenerator::operator()(Module* unit) {
}


void CGenerator::operator()(Formal* formal) {
}


void CGenerator::operator()(StringLiteral* expression) {
}


void CGenerator::operator()(NilLiteral* expression) {
}


void CGenerator::operator()(IntegerLiteral* expression) {
}


void CGenerator::operator()(FloatLiteral* expression) {
}


void CGenerator::operator()(BooleanLiteral* expression) {
}


void CGenerator::operator()(Binary* expression) {
}


void CGenerator::operator()(Unary* expression) {
}


void CGenerator::operator()(Call* expression) {
}


void CGenerator::operator()(Is* expression) {
}


void CGenerator::operator()(Member* expression) {
}


void CGenerator::operator()(Construct* expression) {
}


void CGenerator::operator()(Identifier* expression) {
}


void CGenerator::operator()(ConstantIdentifier* expression) {
}


void CGenerator::operator()(Empty* expression) {
}


void CGenerator::operator()(Cast* expression) {
}


void CGenerator::operator()(Box* expression) {
}


void CGenerator::operator()(Block* statement) {
}


void CGenerator::operator()(Simple* statement) {
}


void CGenerator::operator()(Let* let) {
}


void CGenerator::operator()(While* statement) {
}


void CGenerator::operator()(Conditional* statement) {
}


void CGenerator::operator()(Assignment* statement) {
}


void CGenerator::operator()(Return* statement) {
}


void CGenerator::operator()(Match* statement) {
}


void CGenerator::operator()(Case* statement) {
}


void CGenerator::operator()(Fork* statement) {
}


void CGenerator::operator()(Yield* statement) {
}


void CGenerator::operator()(Import* feature) {
}


void CGenerator::operator()(Closure* expression) {
}


void CGenerator::operator()(Type* type) {
}


void CGenerator::operator()(File* file) {
    // Outputs code for the given file in a single C translation unit. with no
    // external dependencies.
    if (env_->errors()) { return; }
    file_ = file;
    out_ << "#pragma once\n";
    out_ << "#include <Object.h>\n";
    out_ << "#include <Primitives.h>\n";

    for (int i = 0; i < file->dependencies(); i++) {
        Feature* feat = file->dependency(i);
        if (Function* func = dynamic_cast<Function*>(feat)) {
            function_signature(func);
            out_ << ";\n";
        } else if (Constant* con = dynamic_cast<Constant*>(feat)) {
            out_ << "extern ";
            out_ << con->type()->clazz()->label() << " "; // Constant type
            out_ << con->label() << ";\n";
        }
    }
    
    for (Feature::Ptr f = env_->modules(); f; f = f->next()) {
        f(this);
    }
    file_ = 0;
}

void CGenerator::operator()(Class* feature) {
    // Output C code for the class given by 'feature'.  This function sets up
    // the vtable for the class.
    class_ = feature;
    
    if (!feature->is_interface() && !feature->is_mixin()) {
        if (feature->location().file == file_) {
            dispatch_table(feature);
        }
    }

    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
        f(this);
    }
    class_ = 0;     
}

void CGenerator::operator()(Function* feature) {
    // Emit a function, or extern declaration if the function is native.
    String::Ptr id = feature->name();
    if (feature->is_virtual() || feature->location().file != file_) {
        return;
    }
    function_signature(feature);
    if (feature->is_native()) {
        out_ << ";\n";
        return;
    }
    out_ << " {\n"; 

    enter_scope();

    // Set up the 'self' variable with the constructor, if necessary; this 
    // allocates memory for the object using calloc.
    if (feature->is_constructor()) {
        ctor_preamble(class_);
    } 
    
    // Load constants
    assert("Load constants" && 0); 

    Block::Ptr block = feature->block();
    block(this);
    exit_scope();
}

void CGenerator::enter_scope() {
    scope_.push_back(new Scope);
}

void CGenerator::exit_scope() {
    // Pops the symbol table for this scope off the stack, and inserts code to
    // perform cleanup at the end of the scope.
    Scope::Ptr scope = scope_.back();

    if (!scope->has_return()) {
        // Free variables in reverse order!
        for (int i = scope->variables()-1; i >= 0; i--) {
            scope_cleanup(scope->variable(i));
        }
    }
    func_return();
    scope_.pop_back();
}

void CGenerator::scope_cleanup(Variable* var) {
    // Emits the code to clean up the stack when exiting a block.  This
    // includes decrementing reference counts, and calling destructors for
    // value types.
    Type::Ptr type = var->type();
    if (type && !type->is_primitive()) {
        if (type->is_value()) {
            assert(!"Need to figure out how to do value types");
            // Call destructor
        } else {
            // Emit a branch to check the variable's reference count and free
            // it if necessary.
            refcount_dec(var->name());
        }
    }
}

void CGenerator::func_return() {
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
    if (function_->is_constructor()) {
        out_ << "return self;\n";
    } else {
        out_ << "return t" << scope_.back()->return_val() << "\n;";
    }

    // If this is the destructor, then release all pointers held by the object,
    // and call free() to release memory if the object was dynamically
    // allocated.
    if (function_->is_destructor()) {
        dtor_epilog(function_);
    }
}

void CGenerator::function_signature(Function* feature) {
    out_ << feature->type()->clazz()->label() << " "; // Return type
    out_ << feature->label() << "("; // Function name
    for (Formal* f = feature->formals(); f; f = f->next()) {
        // Arguments
        out_ << f->type()->clazz()->label() << " " << f->name();
        if (f->next()) {
            out_ << ", ";
        }
    }
    out_ << ")";
}

void CGenerator::refcount_inc(Operand var) {
    out_ << "Object__refcount_inc(t" << var << ");\n";
}

void CGenerator::refcount_dec(Operand var) {    
    out_ << "Object__refcount_dec(t" << var << ");\n";
}

void CGenerator::ctor_preamble(Class* feature) {

}

void CGenerator::dtor_epilog(Function* feature) {
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
        out_ << "Object__refcount_dec(";
        out_ << "self->" << attrs[i]->label() << ");\n";
    }

    if (class_->is_object()) {
        out_ << "free(self);\n";
    }
}

void CGenerator::dispatch_table(Class* feature) {
    // Output the class dispatch table for calling methods with dynamic
    // dispatch.  The format is as follows: 
    //
    //     vtable[0] is the destructor
    //     vtable[1] is the hash function
    //     vtable[2] is the equals function
    //     vtable[3] is the number of 'slots' in the vtable (n)
    //     vtable[2..n+3] are the hash mixing values
    //     vtable[2n+2..2n+3] are the actual method pointers

    String* name = feature->label();
    Function* dtor = feature->function(env_->name("@destroy"));

    out_ << "Int " << name << "__vtable[] = {"; 
    out_ << "    (Int)" << dtor->label() << ",\n";
    out_ << "    " << feature->jump1s() << ",\n";
    
    // Emit the first jump table
    for (int i = 0; i < feature->jump1s(); i++) {
        out_ << "    " << feature->jump1(i) << "\n";
    } 

    // Emit the second jump table
    for (int i = 0; i < feature->jump2s(); i++) {
        if (feature->jump2(i)) {
            out_ << "    " << feature->jump2(i)->label() << "\n,";
        } else {
            out_ << "    0\n,"; 
        }
    }
    out_ << "};\n";
}
