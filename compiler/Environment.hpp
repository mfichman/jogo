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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#pragma once

#include "Apollo.hpp"
#include "Object.hpp"
#include "Feature.hpp"
#include "String.hpp"
#include "File.hpp"
#include <map>


/* Compilation environment; contains symbol table and compilation units */
class Environment : public Object {
public:
    Environment();
    String* name(const std::string& str);
    String* integer(const std::string& str);
    String* floating(const std::string& str);
    String* string(const std::string& str);
    String* integers() const { return integers_; }
    String* floats() const { return floats_; }
    String* strings() const { return strings_; }
    File* files() const { return files_; }
    Module* root() const { return root_; }
    Module* builtins() const { return builtins_; }
    Module* module(String* scope);
    Module* modules() const { return modules_; }
    File* file(String* name);
    Constant* constant(int index) { return constant_[index]; }
    const std::string& include(int index) { return include_[index]; }
    const std::string& input(int index) { return input_[index]; }
    const std::string& lib(int index) { return lib_[index]; }
    const std::string& output() const { return output_; }
    const std::string& output_dir() const { return output_dir_; }
    bool make() const { return make_; }
    bool optimize() const { return optimize_; }
    bool link() const { return link_; }
    bool assemble() const { return assemble_; }
    bool execute() const { return execute_; }
    bool dump_ir() const { return dump_ir_; }
    bool dump_liveness() const { return dump_liveness_; }
    bool dump_ast() const { return dump_ast_; }
    bool verbose() const { return verbose_; }
    const std::string& generator() const { return generator_; }
    
    int errors() { return errors_; }
    int includes() { return include_.size(); }
    int inputs() { return input_.size(); }
    int libs() { return lib_.size(); }
    int constants() { return constant_.size(); }
    void include(const std::string& path) { include_.push_back(path); }
    void input(const std::string& path) { input_.push_back(path); }
    void lib(const std::string& path) { lib_.push_back(path); }
    void output(const std::string& path) { output_ = path; }
    void output_dir(const std::string& path) { output_dir_ = path; }
    void dump_ir(bool dump) { dump_ir_ = dump; }
    void dump_liveness(bool dump) { dump_liveness_ = dump; }
    void dump_ast(bool dump) { dump_ast_ = dump; }
    void verbose(bool verbose) { verbose_ = verbose; }
    void make(bool make) { make_ = make; }
    void optimize(bool optimize) { optimize_ = optimize; }
    void link(bool link) { link_ = link; }
    void assemble(bool assemble) { assemble_ = assemble; }
    void execute(bool execute) { execute_ = execute; }
    void generator(const std::string& gen) { generator_ = gen; }
    void module(Module* module);
    void file(File* name);
    void constant(Constant* cons) { constant_.push_back(cons); }
    void error(const std::string& error) { errors_++; }
    void error() { errors_++; }
    const Location& location() const;
    typedef Pointer<Environment> Ptr;

    Type* void_type() const { return void_type_; }
    Type* bool_type() const { return bool_type_; }
    Type* int_type() const { return int_type_; }
    Type* string_type() const { return string_type_; }
    Type* no_type() const { return no_type_; }
    Type* bottom_type() const { return bottom_type_; }
    Type* nil_type() const { return nil_type_; }
    Type* float_type() const { return float_type_; }
    Type* char_type() const { return char_type_; }
    Type* byte_type() const { return byte_type_; }
    Type* pair_type() const { return pair_type_; }
    Type* self_type() const { return self_type_; }
    Type* any_type() const { return any_type_; }

private:
    std::map<std::string, String::Ptr> name_;
    std::map<std::string, String::Ptr> integer_;
    std::map<std::string, String::Ptr> floating_;
    std::map<std::string, String::Ptr> string_;
    std::map<String::Ptr, Module::Ptr> module_;
    std::map<String::Ptr, File::Ptr> file_;
    std::vector<std::string> include_;
    std::vector<std::string> input_;
    std::vector<std::string> lib_;
    std::vector<Constant::Ptr> constant_;

    File::Ptr builtin_file_;
    File::Ptr files_;
    String::Ptr strings_;
    String::Ptr integers_;
    String::Ptr floats_;
    Module::Ptr root_;
    Module::Ptr builtins_;
    Module::Ptr modules_;
    Type::Ptr void_type_;
    Type::Ptr bool_type_;
    Type::Ptr int_type_;
    Type::Ptr string_type_;
    Type::Ptr no_type_;
    Type::Ptr bottom_type_;
    Type::Ptr nil_type_;
    Type::Ptr float_type_;
    Type::Ptr char_type_;
    Type::Ptr byte_type_;
    Type::Ptr pair_type_;
    Type::Ptr self_type_;
    Type::Ptr any_type_;

    std::string output_;
    std::string output_dir_;
    bool dump_ast_;
    bool dump_ir_;
    bool dump_liveness_;
    bool make_;
    bool optimize_;
    bool link_;
    bool assemble_;
    bool execute_;
    bool verbose_;
    std::string generator_;

    int errors_;
};

