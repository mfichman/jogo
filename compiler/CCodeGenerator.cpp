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

#include "CCodeGenerator.hpp"


CCodeGenerator::CCodeGenerator(Environment* env) :
    env_(env),
	indent_(0) {

}

void CCodeGenerator::operator()(Attribute* attr) {

}

void CCodeGenerator::operator()(Constant* feature) {
}


void CCodeGenerator::operator()(Module* unit) {
    for (Feature::Ptr f = unit->features(); f; f = f->next()) {
        f(this);
    }
}


void CCodeGenerator::operator()(Formal* formal) {
}


void CCodeGenerator::operator()(StringLiteral* expression) {
	out_ << "lit" << (void*)expression->value();
}


void CCodeGenerator::operator()(NilLiteral* expression) {
	out_ << "0";
}


void CCodeGenerator::operator()(IntegerLiteral* expression) {
	out_ << expression->value()->string();
}


void CCodeGenerator::operator()(FloatLiteral* expression) {
	out_ << expression->value()->string();
}


void CCodeGenerator::operator()(BooleanLiteral* expression) {
	out_ << expression->value()->string();
}


void CCodeGenerator::operator()(Binary* expression) {
}


void CCodeGenerator::operator()(Unary* expression) {
}


void CCodeGenerator::operator()(Call* expr) {
	// The call expression may actually be a native operator, so check the type
	// first.
	if (expr->arguments()) {
		Type::Ptr type = expr->arguments()->type();
		String::Ptr id = expr->function()->name();
		if (id->string()[0] == '@' && type->is_primitive()) {
			assert("native_operator");
			return;
		}
	}
	call(expr->function(), expr->arguments());
}


void CCodeGenerator::operator()(Is* expression) {
}


void CCodeGenerator::operator()(Member* expression) {
}


void CCodeGenerator::operator()(Construct* expression) {
}


void CCodeGenerator::operator()(Identifier* expression) {
}


void CCodeGenerator::operator()(ConstantIdentifier* expression) {
}


void CCodeGenerator::operator()(Empty* expression) {
}


void CCodeGenerator::operator()(Cast* expression) {
}


void CCodeGenerator::operator()(Box* expression) {
}


void CCodeGenerator::operator()(Block* statement) {
	enter_scope();
	for (Statement::Ptr s = statement->children(); s; s = s->next()) {
		s(this);
	}
	exit_scope();
}


void CCodeGenerator::operator()(Simple* statement) {
	Expression::Ptr expr = statement->expression();
	emit(expr);
	//free_temps();
}


void CCodeGenerator::operator()(Let* let) {
}


void CCodeGenerator::operator()(While* statement) {
}


void CCodeGenerator::operator()(Conditional* statement) {
}


void CCodeGenerator::operator()(Assignment* statement) {
}


void CCodeGenerator::operator()(Return* statement) {
	Expression::Ptr expr = statement->expression();
	if (!dynamic_cast<Empty*>(expr.pointer())) {
		Operand ret = emit(expr);
		scope_.back()->return_val(ret);

		// Increment the refcount if the returned value is an object.  The 
		// refcount must be incremented so that the object won't be freed
		// when the function returns.  It is the caller's responsibility to 
		// correctly free the returned object.
		if (!expr->type()->is_value()) {
			refcount_inc(ret);
		}
	}
	scope_.back()->has_return(true);
	//free_temps();
}


void CCodeGenerator::operator()(Match* statement) {
}


void CCodeGenerator::operator()(Case* statement) {
}


void CCodeGenerator::operator()(Fork* statement) {
}


void CCodeGenerator::operator()(Yield* statement) {
}


void CCodeGenerator::operator()(Import* feature) {
}


void CCodeGenerator::operator()(Closure* expression) {
}


void CCodeGenerator::operator()(Type* type) {
}


void CCodeGenerator::operator()(File* file) {
    // Outputs code for the given file in a single C translation unit. with no
    // external dependencies.
    if (env_->errors()) { return; }
    file_ = file;
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

void CCodeGenerator::operator()(Class* feature) {
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

void CCodeGenerator::operator()(Function* feature) {
    // Emit a function, or extern declaration if the function is native.
    String::Ptr id = feature->name();
	IndentAnchor indent(this);
	op_ = Operand();
    if (feature->is_virtual() || feature->location().file != file_) {
        return;
    }
	function_ = feature;
    function_signature(feature);
    if (feature->is_native()) {
        out_ << ";\n";
		function_ = 0;
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
    //assert("Load constants" && 0); 

    Block::Ptr block = feature->block();
    block(this);
    exit_scope();
	function_ = 0;

	out_ << "}\n";
}

void CCodeGenerator::call(Function* function, Expression* args) {
	std::vector<Operand> val;
	for (Expression::Ptr a = args; a; a = a->next()) {
		val.push_back(emit(a));
	}

	line();
	out_ << function->label() << "(";

	for (int i = 0; i < val.size(); i++) {
		out_ << val[i];
		if (i < val.size() - 1) {
			out_ << ", ";
		}
	}
	out_ << ")";
}

Operand CCodeGenerator::emit(Expression* expr) {
	// Emit a single expression as a 
	Expression::Ptr expression = expr;
	++op_;
	if (!expression->type()->is_void()) {
		line();
		out_ << expression->type()->clazz()->label();
		out_ << " " << op_ << " = ";
	}
	expression(this);
	out_ << ";\n";
	return op_;
}

void CCodeGenerator::enter_scope() {
    scope_.push_back(new Scope);
}

void CCodeGenerator::exit_scope() {
    // Pops the symbol table for this scope off the stack, and inserts code to
    // perform cleanup at the end of the scope.
    Scope::Ptr scope = scope_.back();

    if (!scope->has_return()) {
        // Free variables in reverse order!
        for (int i = scope->variables()-1; i >= 0; i--) {
            scope_cleanup(scope->variable(i));
        }
		scope_.pop_back();
		return;
    }

    func_return();
    scope_.pop_back();
}

void CCodeGenerator::scope_cleanup(Variable* var) {
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

void CCodeGenerator::func_return() {
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
	line();
    if (function_->is_constructor()) {
        out_ << "return self;\n";
    } else {
        out_ << "return " << scope_.back()->return_val() << ";\n";
    }

    // If this is the destructor, then release all pointers held by the object,
    // and call free() to release memory if the object was dynamically
    // allocated.
    if (function_->is_destructor()) {
        dtor_epilog(function_);
    }
}

void CCodeGenerator::function_signature(Function* feature) {
	if (feature->type()->is_void()) {
		out_ << "void ";
	} else {
        out_ << feature->type()->clazz()->label() << " "; // Return type
	}
    out_ << feature->label() << "("; // Function name
    for (Formal* f = feature->formals(); f; f = f->next()) {
        // Arguments
		if (f->type()->is_self()) {
			out_ << feature->parent()->label();
		} else {
			out_ << f->type()->clazz()->label() << " " << f->name();
		}
        if (f->next()) {
            out_ << ", ";
        }
    }
	if (!feature->formals()) {
		out_ << "void";
	}
    out_ << ")";
}

void CCodeGenerator::refcount_inc(Operand var) {
    out_ << "Object__refcount_inc(t" << var << ");\n";
}

void CCodeGenerator::refcount_dec(Operand var) {    
    out_ << "Object__refcount_dec(t" << var << ");\n";
}

void CCodeGenerator::ctor_preamble(Class* feature) {

}

void CCodeGenerator::dtor_epilog(Function* feature) {
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
        out_ << "Boot_free(self);\n";
    }
}

void CCodeGenerator::dispatch_table(Class* feature) {
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
