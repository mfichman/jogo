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

#include "Jogo.hpp"
#include "Object.hpp"
#include "Feature.hpp"
#include "String.hpp"
#include "File.hpp"
#include "OrderedMap.hpp"
#include <map>

enum SubtypeResult { UNCHECKED, YES, NO };
class SubtypeKey {
public:
    SubtypeKey(Type const* t1, Type const* t2) :
        t1_(t1),
        t2_(t2) {
    }
    bool operator==(SubtypeKey const& other) const;
    bool operator<(SubtypeKey const& other) const;
    Type const* t1() const { return t1_; }
    Type const* t2() const { return t2_; }
private:
    Pointer<Type const> t1_;
    Pointer<Type const> t2_;
};

/* Compilation environment; contains symbol table and compilation units */
class Environment : public Object {
public:
    Environment();
    String* name(const std::string& str) const;
    String* integer(const std::string& str) const;
    String* floating(const std::string& str) const;
    String* string(const std::string& str) const;
    Module* module(String* scope) const { return query(module_, scope); }
    File* file(String* name) const { return query(file_, name); }
    Module* root() const { return root_; }
    Feature* feature(String* qn) const;
    String::Itr integers() const { return String::Itr(integer_); }
    String::Itr floats() const { return String::Itr(floating_); }
    String::Itr strings() const { return String::Itr(string_); }
    File::Itr files() const { return File::Itr(file_); }
    Constant::Itr constants() const { return Constant::Itr(constant_); }
    Module::Itr modules() const { return Module::Itr(module_); }
    std::string const& include(int index) const { return include_[index]; }
    std::string const& input(int index) const { return input_[index]; }
    std::string const& lib(int index) const { return lib_[index]; }
    std::string const& output() const { return output_; }
    std::string const& build_dir() const { return build_dir_; }
    std::string const& program_path() const { return program_path_; }
    std::string const& entry_point() const { return entry_point_; }
    std::string const& entry_module() const { return entry_module_; }
    std::string const& generator() const { return generator_; }
    bool make() const { return make_; }
    bool optimize() const { return optimize_; }
    bool link() const { return link_; }
    bool assemble() const { return assemble_; }
    bool execute() const { return execute_; }
    bool dump_ir() const { return dump_ir_; }
    bool dump_liveness() const { return dump_liveness_; }
    bool dump_regalloc() const { return dump_regalloc_; }
    bool dump_reggraph() const { return dump_reggraph_; }
    bool dump_ast() const { return dump_ast_; }
    bool verbose() const { return verbose_; }
    bool gen_library() const;
    bool no_default_libs() const { return no_default_libs_; }
    bool monolithic_build() const { return monolithic_build_; }
    bool is_input(const std::string& input) const;
    
    int errors() const { return errors_; }
    int includes() const { return include_.size(); }
    int inputs() const { return input_.size(); }
    int libs() const { return lib_.size(); }
    void include(const std::string& path) { include_.push_back(path); }
    void input(const std::string& path) { input_.push_back(path); }
    void lib(const std::string& path) { lib_.push_back(path); }
    void output(const std::string& path) { output_ = path; }
    void build_dir(const std::string& path) { build_dir_ = path; }
    void dump_ir(bool dump) { dump_ir_ = dump; }
    void dump_liveness(bool dump) { dump_liveness_ = dump; }
    void dump_regalloc(bool dump) { dump_regalloc_ = dump; }
    void dump_reggraph(bool dump) { dump_reggraph_ = dump; }
    void dump_ast(bool dump) { dump_ast_ = dump; }
    void verbose(bool verbose) { verbose_ = verbose; }
    void make(bool make) { make_ = make; }
    void optimize(bool optimize) { optimize_ = optimize; }
    void link(bool link) { link_ = link; }
    void assemble(bool assemble) { assemble_ = assemble; }
    void execute(bool execute) { execute_ = execute; }
    void no_default_libs(bool no) { no_default_libs_ = no; }
    void monolithic_build(bool monolithic) { monolithic_build_ = monolithic; }
    void generator(const std::string& gen) { generator_ = gen; }
    void program_path(const std::string& path) { program_path_ = path; }
    void entry_point(const std::string& entry) { entry_point_ = entry; }
    void entry_module(const std::string& entry) { entry_module_ = entry; }
    void module(Module* module) { module_[module->name()] = module; }
    void file(File* file) { file_[file->name()] = file; }
    void constant(Constant* cons) { constant_.push_back(cons); }
    void genclass(Class* gen) { genclass_.push_back(gen); }
    void workspace_load();
    void workspace_search(std::string prefix, std::string name);
    void error(const std::string& error) { errors_++; }
    void error() { errors_++; }
    const Location& location() const;
    typedef Pointer<Environment> Ptr;

    Type* void_type() const { return void_type_; }
    Type* bool_type() const { return bool_type_; }
    Type* int_type() const { return int_type_; }
    Type* string_type() const { return string_type_; }
    Type* top_type() const { return top_type_; }
    Type* bottom_type() const { return bottom_type_; }
    Type* nil_type() const { return nil_type_; }
    Type* float_type() const { return float_type_; }
    Type* char_type() const { return char_type_; }
    Type* byte_type() const { return byte_type_; }
    Type* pair_type() const { return pair_type_; }
    Type* any_type() const { return any_type_; }
    Type* enum_type() const { return enum_type_; }
    Type* object_type() const { return object_type_; }
    Type* functor_type() const { return functor_type_; }
    Type* value_type() const { return value_type_; }
    Type* interface_type() const { return interface_type_; }
    Type* union_type() const { return union_type_; }
    Type* appendable_type() const { return appendable_type_; }
    SubtypeResult subtype(Type const* t1, Type const* t2) const;
    void subtype(Type const* t1, Type const* t2, SubtypeResult res);

private:
    mutable std::map<std::string, String::Ptr> name_;
    mutable std::map<std::string, String::Ptr> integer_;
    mutable std::map<std::string, String::Ptr> floating_;
    mutable std::map<std::string, String::Ptr> string_;
    mutable std::map<String::Ptr, Module::Ptr> module_;
    mutable std::map<String::Ptr, File::Ptr> file_;
    mutable std::vector<std::string> include_;
    mutable std::vector<std::string> input_;
    mutable std::vector<std::string> lib_;
    mutable std::vector<Constant::Ptr> constant_;

    File::Ptr builtin_file_;
    Module::Ptr root_;
    Type::Ptr void_type_;
    Type::Ptr bool_type_;
    Type::Ptr int_type_;
    Type::Ptr string_type_;
    Type::Ptr top_type_;
    Type::Ptr bottom_type_;
    Type::Ptr nil_type_;
    Type::Ptr float_type_;
    Type::Ptr char_type_;
    Type::Ptr byte_type_;
    Type::Ptr pair_type_;
    Type::Ptr any_type_;
    Type::Ptr enum_type_;
    Type::Ptr object_type_;
    Type::Ptr functor_type_;
    Type::Ptr value_type_;
    Type::Ptr interface_type_;
    Type::Ptr union_type_;
    Type::Ptr appendable_type_;

    std::string output_;
    std::string build_dir_;
    std::string program_path_;
    std::string entry_point_;
    std::string entry_module_;
    bool dump_ast_;
    bool dump_ir_;
    bool dump_liveness_;
    bool dump_regalloc_;
    bool dump_reggraph_;
    bool make_;
    bool optimize_;
    bool link_;
    bool assemble_;
    bool execute_;
    bool verbose_;
    bool monolithic_build_;
    bool no_default_libs_;
    std::string generator_;
    std::map<SubtypeKey, SubtypeResult> subtype_;
    std::vector<Class::Ptr> genclass_;

    int errors_;
};

