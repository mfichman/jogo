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
	indent_(0),
    braces_(0),
    label_(0), 
    true_(0),
    false_(0),
    invert_guard_(0),
    invert_branch_(0) { 

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
    return_ = alloc_temp(expression->type());
	out_ << "&lit" << (void*)expression->value() << ";\n";
}


void CCodeGenerator::operator()(NilLiteral* expression) {
    return_ = expression;
}


void CCodeGenerator::operator()(IntegerLiteral* expression) {
    //return_ = alloc_temp(expression->type());
	//out_ << expression->value()->string() << ";\n";
    return_ = expression;
}


void CCodeGenerator::operator()(FloatLiteral* expression) {
    return_ = expression;
}


void CCodeGenerator::operator()(BooleanLiteral* expression) {
    return_ = expression;
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
            native_operator(expr);
			return;
		}
	}
	call(expr->function(), expr->arguments());
}


void CCodeGenerator::operator()(Is* expression) {
}


void CCodeGenerator::operator()(Member* expression) {
}


void CCodeGenerator::operator()(Construct* expr) {
    // Look up the function by name in the current context.
    String::Ptr id = env_->name("@init");
    Class::Ptr clazz = expr->type()->clazz();
    Function::Ptr func = clazz->function(id);
    
    std::vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }

    return_ = alloc_temp(clazz->type());
    out_ << func->label() << "(";

    Formal::Ptr formal = func->formals();
    Expression::Ptr arg = expr->arguments();
    for (int i = 0; i < args.size(); i++) {
        if(!formal->is_self() && !formal->type()->equals(arg->type())) {
            // Cast to the appropriate C-type, since C doesn't know anything 
            // about subtypes, etc..
            out_ << "(";
            operator()(formal->type());
            out_ << ")";
        }
        out_ << args[i];
        if (i < args.size() - 1) {
            out_ << ", ";
        }
        formal = formal->next();
        arg = arg->next();
    }
    out_ << ");\n";
}


void CCodeGenerator::operator()(Identifier* expr) {
    // Simply look up the value of the variable as stored previously.
    String::Ptr id = expr->identifier();
    Variable::Ptr var = variable(id);
    Attribute::Ptr attr = class_ ? class_->attribute(id) : 0;
    if (var) {
        return_ = id.pointer();
    } else if (attr) {
        return_ = alloc_temp(expr->type());
        out_ << "self->" << id->string() << ";\n";
    } else {
        return_ = id.pointer();
    }
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

    source_line(statement);
	emit(expr);
	//free_temps();
}


void CCodeGenerator::operator()(Let* let) {
}


void CCodeGenerator::operator()(While* statement) {
}


void CCodeGenerator::operator()(Conditional* statement) {
    // Emit a conditional expression.  This works more or less like the
    // assembly version, using if/goto and labels, because temporaries created
    // in the guard expression must be tracked and freed at the end of the
    // guard expression.

    int then_label = label_++;
    int else_label = 0;
    int done_label = label_++; 
    if (statement->false_branch()) {
        else_label = label_++;
    } else {
        else_label = done_label;
    }

    // Recursively emit the boolean guard expression.  We need to pass the true
    // and false block labels so that the correct code is emitted on each
    // branch.
    invert_guard_ = false;
//    Operand guard = emit(statement->guard(), 
    

}


void CCodeGenerator::operator()(Assignment* expr) {
    // Handle all types of assignment, including member assignment
    Expression::Ptr init = expr->initializer();
    if (dynamic_cast<Empty*>(init.pointer())) {
        return_ = env_->integer("0");
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
        line();
        out_ << id->string() << " = " << return_ << ";\n";
        if (!type->is_value()) {
            refcount_inc(var->operand());
        }
    } else if (attr) {
        // Assignment to an attribute within a class
/*
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
*/
    } else {
        // Assignment to a local var that has not yet been initialized in the
        // current scope.
        Type::Ptr declared = expr->declared_type();
        if (!declared) {
            declared = expr->type();
        }
        line();
        brace();
        operator()(declared);
        out_ << " " << id->string() << " = " << return_ << ";\n";
        variable(new Variable(id, return_, declared));
        if (!declared->is_value()) {
            refcount_inc(return_);
        }
    }
}


void CCodeGenerator::operator()(Return* statement) {
	Expression::Ptr expr = statement->expression();
    source_line(statement);

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
    // Outputs the true typename for Type as a C type declaration.
    if (type->is_void()) {
        out_ << "void";
    } else if (type->is_enum()) {
        out_ << "Int";
    } else if (type->is_generic()) {
        out_ << "Object";
	} else if (type->is_primitive()) {
		out_ << type->clazz()->label();
    } else if (type->clazz()->label()->string() == "String") {
        out_ << "String";
    } else if (file_ == type->clazz()->file()) {
		out_ << type->clazz()->label();
    } else {
        out_ << "Object";
	}
}


void CCodeGenerator::operator()(File* file) {
    // Outputs code for the given file in a single C translation unit. with no
    // external dependencies.
    if (env_->errors()) { return; }
    file_ = file;
    out_ << "#include <Object.h>\n";
    out_ << "#include <Primitives.h>\n";
    out_ << "#include <String.h>\n";
    out_ << "Ptr Boot_calloc(Int size);\n";
    out_ << "void Boot_free(Ptr memory);\n";

    for (String::Ptr s = env_->strings(); s; s = s->next()) {
        out_ << "static struct String lit" << (void*)s.pointer() << " = { "; 
        out_ << "String__vtable, "; // Vtable
        out_ << "1, "; // Refcount
        out_ << (int)s->string().length() << ", ";
        out_ << "\"" << s->string() << "\" };\n";
    }
    for (int i = 0; i < file->constants(); i++) {
        Constant::Ptr cons = file->constant(i);
        if (cons->type()->is_value() && !cons->type()->is_primitive()) {
            assert(!"Not supported");
        }
        operator()(cons->type());
        out_ << " " << cons->label() << " = 0;\n";
    }

    // Output a typedef for the pointer to an object, if the type is a
    // reference type.  This needs to come first, because reference types
    // can refer to eachother circularly.

    for (int i = 0; i < file->dependencies(); i++) {
        TreeNode* dep = file->dependency(i);
        if (Class* cls = dynamic_cast<Class*>(dep)) {
            class_decl(cls);

        }
    }

    // Now output the data layout of the class as a C type definition..
//    for (int i = 0; i < file->dependencies(); i++) {
//        Feature* feat = file->dependency(i);
//        if (Class* cls = dynamic_cast<Class*>(feat)) {
//            //class_def(cls);
//        }
//    }

    // Output declarations for any functions used by this file.
    for (int i = 0; i < file->dependencies(); i++) {
        TreeNode* dep = file->dependency(i);
        if (Function* func = dynamic_cast<Function*>(dep)) {
            func_sig(func);
            out_ << ";\n";
        } else if (Constant* con = dynamic_cast<Constant*>(dep)) {
            out_ << "extern ";
            operator()(con->type()); // Constant type
            out_ << " " << con->label() << ";\n";
        }
    }
    
    for (int i = 0; i < file->features(); i++) {
        file->feature(i)->operator()(this);
    }
    file_ = 0;
    out_->flush();
}

void CCodeGenerator::operator()(Class* feat) {
    // Output C code for the class given by 'feature'.  This function sets up
    // the vtable for the class.
    class_ = feat;
    class_def(feat);

    if (!feat->is_interface() && !feat->is_value()) {
        dispatch_table(feat);
    }

    for (Feature::Ptr f = feat->features(); f; f = f->next()) {
        f(this);
    }
    class_ = 0;     
}

void CCodeGenerator::operator()(Function* feature) {
    // Emit a function, or extern declaration if the function is native.
    String::Ptr id = feature->name();
	op_ = Operand();
    if (feature->is_virtual()) { return; }
	function_ = feature;
    func_sig(feature);
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

    // If this is main(), then emit the code to load constants.
    String::Ptr name = feature->name();
    Feature::Ptr parent = feature->parent();
    if (name->string() == "main" && parent->name()->string() == "Boot") {
        constants();
    }
    
    // Load constants
    //assert("Load constants" && 0); 

    Block::Ptr block = feature->block();
    block(this);

    if (feature->type()->is_void() || feature->is_constructor()) {
        func_return();
    }

    exit_scope();
	function_ = 0;

	out_ << "}\n";
}

void CCodeGenerator::call(Function* function, Expression* args) {
	std::vector<Operand> val;
	for (Expression::Ptr a = args; a; a = a->next()) {
		val.push_back(emit(a));
	}

    if (!function->type()->is_void()) {
        return_ = alloc_temp(function->type());
    } else {
        line();
    }
	out_ << function->label() << "(";
    
    Expression::Ptr arg = args;
    Formal::Ptr formal = function->formals();
	for (int i = 0; i < val.size(); i++) {
        if(!formal->is_self() && !formal->type()->equals(arg->type())) {
            // Cast to the appropriate C-type, since C doesn't know anything 
            // about subtypes, etc..
            out_ << "(";
            operator()(formal->type());
            out_ << ")";
        }
		out_ << val[i];
		if (i < val.size() - 1) {
			out_ << ", ";
		}
        formal = formal->next();
        arg = arg->next();
	}
	out_ << ");\n";
}

Operand CCodeGenerator::emit(Expression* expr) {
	// Emit a single expression as a 
    expr->operator()(this);
	return return_;
}

Operand CCodeGenerator::emit(Expression* expr, int yes, int no, bool invert) {
    int true_save = true_;
    int false_save = false_;
    bool invert_branch_save = invert_branch_;
    true_ = yes;
    false_ = no;
    invert_branch_ = invert;
    expr->operator()(this);
    true_ = true_save;
    false_ = false_save;
    invert_branch_ = invert_branch_save;
    return return_;
}

Operand CCodeGenerator::alloc_temp(Type* type) {
    line();
    brace();
    operator()(type);
    op_ = Operand(RegisterId(op_.reg().id()+1, 0));
    out_ << " " << op_ << " = ";
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
    } else {
        func_return();
        scope_.pop_back();
    }
    if (braces_) {
        line();
        for (int i = 0; i < braces_; i++) {
            out_ << "}";
        }
        out_ << "\n";
    }
    braces_ = 0;
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

void CCodeGenerator::variable(Variable* var) {
    assert(scope_.size());
    scope_.back()->variable(var);
}

Variable* CCodeGenerator::variable(String* name) {
    // Look up the variable, starting in the current scope and continuing up
    // the scope stack.
    std::vector<Scope::Ptr>::reverse_iterator i;
    for (i = scope_.rbegin(); i != scope_.rend(); i++) {
        Variable* var = (*i)->variable(name);
        if (var) {
            return var;
        }
    }
    return 0;
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

    // If this is the destructor, then release all pointers held by the object,
    // and call free() to release memory if the object was dynamically
    // allocated.
    if (function_->is_destructor()) {
        dtor_epilog(function_);
    }

    // Emit an actual return.  Emit code to return the value saved in the var
    // '_ret' if the variable has been set.
    if (function_->is_constructor()) {
	    line();
        out_ << "return self;\n";
    } else if (!function_->type()->is_void()) {
	    line();
        out_ << "return " << scope_.back()->return_val() << ";\n";
    }
}

void CCodeGenerator::func_sig(Function* feature) {
    // Output a function or constructor signature
    if (feature->is_constructor()) {
        Class* clazz = static_cast<Class*>(feature->parent());
        operator()(clazz->type());
    } else {
        operator()(feature->type());
    }
    if (feature->label()->string() == "main") {
        out_ << " main_(";
    } else if (feature->label()->string() == "Boot_main") {
        out_ << " main(";
    } else {
        out_ << " " << feature->label() << "("; // Function name
    }
    for (Formal* f = feature->formals(); f; f = f->next()) {
        // Arguments
        if (f->is_self()) {
            Class* parent = static_cast<Class*>(feature->parent());
            if (feature->file() == file_ || parent->is_value()) {
                out_ << feature->parent()->label();
            } else {
                out_ << "Object";
            }
        } else {
            operator()(f->type());
        }
        out_ << " " << f->name();
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
    line();
    out_ << "Object__refcount_inc((Object)" << var << ");\n";
}

void CCodeGenerator::refcount_dec(Operand var) {    
    line();
    out_ << "Object__refcount_dec((Object)" << var << ");\n";
}

void CCodeGenerator::ctor_preamble(Class* clazz) {
    // Emits the memory alloc/vtable setup for the class.  Also initializes 
    // the reference count for dynamically allocated types.
    if (clazz->is_object()) {
        line();
        operator()(clazz->type());
        out_ << " self = Boot_calloc(sizeof(struct ";
        operator()(clazz->type());
        out_ << "));\n";
        line();
        out_ << "self->_refcount = 1;\n"; 
        line();
        out_ << "self->_vtable = ";
        operator()(clazz->type());
        out_ << "__vtable;\n";
    }

    // Emit initializer code for initialized attributes
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f.pointer())) {
            Expression::Ptr init = attr->initializer();
            if (!init || dynamic_cast<Empty*>(init.pointer())) {
                continue;
            }
            line();
            Operand value = emit(init); 
            out_ << "self->" << attr->name() << " = " << value << ";\n";
            //free_temps();
        }
    }
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
        line();
        out_ << "Object__refcount_dec((Object)";
        out_ << "self->" << attrs[i]->name()->string() << ");\n";
    }

    if (class_->is_object()) {
        line();
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

    // Emit the forward declarations for the vtable
    func_sig(dtor);
    out_ << ";\n";
    for (int i = 0; i < feature->jump2s(); i++) {
        if (feature->jump2(i)) {
            func_sig(feature->jump2(i));
            out_ << ";\n";
        }
    }

    out_ << "Int " << name << "__vtable[] = {\n"; 
    out_ << "    (Int)" << dtor->label() << ",\n";
    out_ << "    " << feature->jump1s() << ",\n";
    
    // Emit the first jump table
    for (int i = 0; i < feature->jump1s(); i++) {
        out_ << "    " << feature->jump1(i) << ",\n";
    } 

    // Emit the second jump table
    for (int i = 0; i < feature->jump2s(); i++) {
        if (feature->jump2(i)) {
            out_ << "    (Int)" << feature->jump2(i)->label() << ",\n";
        } else {
            out_ << "    0,\n"; 
        }
    }
    out_ << "};\n";
}

void CCodeGenerator::class_def(Class* feature) {
    // Outputs the C type definition for the class, including all field
    // definitions.

    // The types listed here have custom implementations
    if (feature->label()->string() == "String") { return; }
    if (feature->label()->string() == "Object") { return; }
    if (feature->is_primitive()) { return; }

    out_ << "struct " << feature->label()->string() << " {\n";
	IndentAnchor indent(this);
    line(); out_ << "Ptr _vtable;\n";
    line(); out_ << "Int _refcount;\n";
    for (Feature* f = feature->features(); f; f = f->next()) {
        if (Attribute::Ptr attr = dynamic_cast<Attribute*>(f)) {
            line();
            operator()(attr->type());
            out_ << " " << attr->name()->string() << ";\n";
        }
    }
    out_ << "};\n";
    if (feature->is_value()) {
        out_ << "typedef struct " << feature->label()->string();
        out_ << " " <<  feature->label()->string() << ";\n"; 
    } else {
      //  out_ << "typedef struct " << feature->label()->string();
      //  out_ << "* " << feature->label()->string() << ";\n";
    }
}


void CCodeGenerator::class_decl(Class* feature) {
    if (feature->label()->string() == "String") { return; }
    if (feature->label()->string() == "Object") { return; }
    if (feature->is_enum()) {
        out_ << "typedef Int " << feature->label()->string();
        out_ << ";\n";
    }


    if (feature->is_value()) { return; }
    out_ << "typedef struct " << feature->label()->string();
    out_ << "* " << feature->label()->string() << ";\n";
}

void CCodeGenerator::constants() {
    for (int i = 0; i < env_->constants(); i++) {
        Constant::Ptr cons = env_->constant(i);
        Operand ret = emit(cons->initializer());
        out_ << cons->label() << " = " << ret << ";\n";
    }
}

void CCodeGenerator::native_operator(Call* expr) {

    std::string id = expr->function()->name()->string();  
    std::vector<Operand> args;
    for (Expression::Ptr a = expr->arguments(); a; a = a->next()) {
        args.push_back(emit(a));
    }   

    return_ = alloc_temp(expr->type());
    if (id == "@add") {
        out_ << args[0] << "+" << args[1] << ";\n";
    } else if (id == "@sub") {
        out_ << args[0] << "-" << args[1] << ";\n";
    } else if (id == "@mul") {
        out_ << args[0] << "*" << args[1] << ";\n";
    } else if (id == "@div") {
        out_ << args[0] << "/" << args[1] << ";\n";
    } else if (id == "@neg") {
        out_ << "-" << args[0] << ";\n";
    } else if (id == "@mod") {
        out_ << args[0] << "%" << args[1] << ";\n";
    } else if (id == "@compl") {
        out_ << "~" << args[0] << ";\n";
    } else if (id == "@equal") {
        out_ << args[0] << "==" << args[1] << ";\n";
    } else if (id == "@less") {
        out_ << args[0] << "<" << args[1] << ";\n";
    }
}


void CCodeGenerator::source_line(TreeNode* node) {
    Location loc = node->location();
    //out_ << "#line " << loc.first_line << " \"";
    //out_ << loc.file->path()->string() << "\"\n";
}
