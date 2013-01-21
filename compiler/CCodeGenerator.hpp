/*
 * Copyright (c) 2010 Matt Fichman
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

#pragma once

#include "Jogo.hpp"
#include "Environment.hpp"
#include "Object.hpp"
#include "Scope.hpp"

/* C code generator */
class CCodeGenerator : public TreeNode::Functor {
public:
    CCodeGenerator(Environment* env);
    typedef Pointer<CCodeGenerator> Ptr;

    Stream* out() const { return out_; }
	int indent() const { return indent_; }
    void out(Stream* out) { out_ = out; }
	void indent(int indent) { indent_ = indent; }
    void operator()(File* file);

private:
    void operator()(Constant* feature);
    void operator()(Class* unit);
    void operator()(Module* unit);
    void operator()(Formal* formal);
    void operator()(StringLiteral* expression);
    void operator()(NilLiteral* expression);
    void operator()(IntegerLiteral* expression);
    void operator()(FloatLiteral* expression);
    void operator()(BooleanLiteral* expression);
    void operator()(Binary* expression);
    void operator()(Unary* expression);
    void operator()(Call* expression);
    void operator()(Is* expression);
    void operator()(Member* expression);
    void operator()(Construct* expression);
    void operator()(IdentifierRef* expression);
    void operator()(ConstantRef* expression);
    void operator()(Empty* expression);
    void operator()(Cast* expression);
    void operator()(Box* expression);
    void operator()(Block* statement);
    void operator()(Simple* statement);
    void operator()(Let* let);
    void operator()(While* statement);
    void operator()(Conditional* statement);
    void operator()(Assignment* statement);
    void operator()(Return* statement);
    void operator()(Match* statement);
    void operator()(Case* statement);
    void operator()(Fork* statement);
    void operator()(Yield* statement);
    void operator()(Function* feature);
    void operator()(Attribute* feature);
    void operator()(Import* feature);
    void operator()(Closure* expression);
    void operator()(Type* type);

	void call(Function* function, Expression* expr);
    void dispatch_table(Class* feature);
    void func_sig(Function* feature); 
    void class_decl(Class* feature);
    void class_def(Class* feature);
    void enter_scope();
    void exit_scope();
    void ctor_preamble(Class* feature);
    void scope_cleanup(Variable* variable);
    void native_operator(Call* expr);
    void func_return();
    void dtor_epilog(Function* function);
    void constants();
    void refcount_inc(Operand var);
    void refcount_dec(Operand var);
	void line() { out_ << std::string(indent_, ' '); }
    void brace() { out_ << "{ "; braces_++; }
    void source_line(TreeNode* node);
	Operand emit(Expression* expr);
    Operand emit(Expression* expr, int yes, int no, bool inv);
    Operand alloc_temp(Type* type);
    Variable* variable(String* name);
    void variable(Variable* var);

    Environment::Ptr env_;
    File::Ptr file_;
    Stream::Ptr out_;
    Class::Ptr class_;
    Function::Ptr function_;
    std::vector<Scope::Ptr> scope_;
    std::set<String::Ptr> variable_;
	Operand op_;
    Operand return_;

	int indent_;
    int braces_;

    int label_;

    int true_;
    int false_;
    bool invert_guard_;
    bool invert_branch_;
};

class IndentAnchor {

public:
	IndentAnchor(CCodeGenerator* cgen) : cgen_(cgen) {
		cgen_->indent(cgen_->indent()+4);
	}
	~IndentAnchor() {
		cgen_->indent(cgen_->indent()-4);
	}

private:
	CCodeGenerator* cgen_;
};
