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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Parser.hpp"
#include "File.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cassert>

Parser::Parser(Environment* env) :
	env_(env),
    err_(Stream::sterr()),
    lexer_(new Lexer(env)),
    error_(0) {
    // Creates a new parser and begins parsing all the input files specified by
    // the environment.

    if (!env->no_default_libs()) {
        input("Jogo");
    }
    for (int i = 0; i < env->inputs(); i++) {
        input(env->input(i));
    }
    input("Boot::Main");
}

void Parser::library(const std::string& import) {
    // Reads in a library file given by "import" (not including the .jgi
    // extension, which is automatically added)

    std::string file = import + ".jgi";
    std::vector<std::string> tests;

    for (int i = 0; i < env_->includes(); i++) {
        const std::string& prefix = env_->include(i);
        if (File::is_reg(prefix + FILE_SEPARATOR + file)) {
            Parser::file(prefix, file); 
            return;
        }
        tests.push_back(prefix + FILE_SEPARATOR  + file);
    }
    env_->error("Could not find library " + import);
    err_ << "Library '" << import << "' not found:\n";
    error();
    for (int i = 0; i < tests.size(); i++) {
        err_ << "    no file '" << tests[i] << "'\n";
        error();
    }
}


void Parser::input(const std::string& import, bool optional) {
    // Reads in a file or directory after searching through the list of include
    // paths.  The includes paths are searched in the order they are specified,
    // so if duplicate files are present in two include directories, then only
    // the first will be parsed.
    std::string file = Import::file_name(import); 
    std::vector<std::string> tests;
    if (env_->file(name(file)) && import != "Boot::Main") { 
        return; 
    }

    // File has not been loaded yet.  Search through all the given include
    // directories to find it.  
    for (int i = 0; i < env_->includes(); i++) {
        const std::string& prefix = env_->include(i);
        if (File::ext(file) != ".jg") {
            const std::string base = prefix + FILE_SEPARATOR + file;
            if (File::is_dir(base)) {
                Parser::dir(prefix, file);
                return;
            } 
            if (File::is_reg(base + ".jg")) {
                Parser::file(prefix, file + ".jg");
                return;
            }
            if (!env_->is_input(import) && File::is_reg(base + ".jgi")) {
                Parser::file(prefix, file + ".jgi");
                return;
            }
        } else {
            if (File::is_reg(prefix + FILE_SEPARATOR + file)) {
                Parser::file(prefix, file);
                return; 
            } 
        }
        tests.push_back(prefix + FILE_SEPARATOR + file);
        tests.push_back(prefix + FILE_SEPARATOR + file + ".jgi");
        tests.push_back(prefix + FILE_SEPARATOR + file + ".jg");
    }
    // Optional import means a lookup failure doesn't necessarily halt
    // compilation.  This is used to implement constants of the form
    // Module::CONST, where Module may be part of another library, or a
    // separate file that must be loaded. 
    if (optional) {
        return;
    }
    env_->error("Could not find " + import);
    err_ << "Module '" << import << "' not found:\n";
    error();
    for (int i = 0; i < tests.size(); i++) {
        err_ << "    no file '" << tests[i] << "'\n";
        error();
    }
}

void Parser::file_alias(const std::string& import) {
    // Creates an alias for a file, so that the parser doesn't try to load a
    // file for the module again.  This aliases the file for "import" to the
    // current file's name, or the empty string if there is no current file.
    if (!file_->is_interface_file() || import.empty()) {
        return;
    }
    String::Ptr fs = name(Import::file_name(import));
    if (!env_->file(fs)) {
        Module::Ptr module = env_->module(name(import));
        File::Ptr file = new File(fs, file_->path(), module, env_);
        file->is_input_file(false);
        file->is_output_file(false);
        env_->file(file);
    } 
}

void Parser::file(const std::string& prefix, const std::string& file) {
    // Begin parsing a module file if it doesn't already exist.

    // The module can be loaded from the dir components of the file name
    String::Ptr scope = name(Import::scope_name(file));
    module_ = env_->module(scope);
    if (!module_) {
        module_ = new Module(location(), env_, scope);
        env_->module(module_);
    }

    // Create a file object for this file if it hasn't been parsed yet.
    std::string actual_file;
    if (prefix == ".") {
        actual_file = file;
    } else {
        actual_file = prefix + FILE_SEPARATOR + file;
    }
    String::Ptr fs = name(file);
    file_ = env_->file(fs);
    if (file_) {
        return;
    } else {
        file_ = new File(fs, name(actual_file), module_, env_);
        module_->file(file_);
        
        // The file is an input file if the corresponding scope was specified 
        // on input, or if the parent scope was specified on input.
        std::string parent = scope->string();
        std::string self = File::base_name(File::no_ext_name(file));
        if (!parent.empty()) {
            self = parent + "::" + self;
        }
        for (int i = 0; i < env_->inputs(); i++) {
            std::string input = File::no_ext_name(env_->input(i));
            if (input == self || input == parent) {
                file_->is_input_file(true);
                break;
            }  
        }
        env_->file(file_);
    }
    if (env_->verbose()) {
        std::cout << "Parsing " << actual_file;
        if (file_->is_input_file()) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // If an interface file is loaded, then be sure to link in the
    // corresponding library as well.
    if (file_->is_interface_file()) {
        env_->lib(Import::module_name(file_->name()->string()));
    }

    // Begin parsing
    lexer_->input(file_);
    error_ = 0;
    module();

    // Now parse other modules that depend on the unit that was added
    if (!file_->is_interface_file()) {
        for (Import::Itr im = file_->imports(); im; ++im) {
            input(im->scope()->string(), im->is_optional());
        }
    }
}

void Parser::dir(const std::string& prefix, const std::string& dir) {
    // Parses all files in the directory specified by 'dir'.  If 'dir' is not
    // a directory, then no files are parsed.
	std::string full_path = prefix + FILE_SEPARATOR + dir + FILE_SEPARATOR;
    if (env_->verbose()) {
	    std::cout << "Parsing " << full_path << std::endl;
    }
    for (File::Iterator i(full_path); i; ++i) {
        std::string name = *i;
        const std::string ext = ".jg";
        if (name.length() <= ext.length()) {
            continue;
        }
        if (!name.compare(name.length() - ext.length(), ext.length(), ext)) {
            file(prefix, dir + FILE_SEPARATOR + name);
        } 
    }
}

void Parser::next() { 
    // Read the next token, update the position, and record any bad tokens
    last_location_ = lexer_->loc(); 
    lexer_->next();
    if (token() == Token::ERROR) {
        err_ << location() << "Invalid character: " << token() << "\n";
        error();
    } 
}

Module* Parser::module() {
    // Parses a module, which can consist of classes, functions, and import
    // statements.
    while (token() != Token::END) {
        String* scope = maybe_scope();
        file_alias(scope->string());
        switch (token()) {
        case Token::IDENTIFIER:
        case Token::OPERATOR:
            module_feature(function(), scope);
            break;
        case Token::CONSTANT:
            module_feature(constant(), scope);
            break;
        case Token::TYPE:
            module_feature(clazz(scope), scope); 
            break;
        case Token::IMPORT:
            import();
            break;
        default:
            if (!error_) {
                err_ << location() << "Unexpected " << token() << "\n";
                error();
            }
            next();
            break;
        }
        if (token() == Token::SEPARATOR) {
            next();
        }
    }
    return module_;
}

bool Parser::expect(Token token) {
    // This function will check the current token to make sure that it matches
    // 'token'.  If it doesn't match, an error is printed.  If it does match,
    // the token is consumed.  This is best for operator tokens, e.g., '{'.
    if (token == Parser::token()) {
        next();
        return true;
    } else {
        if (!error_) {
            err_ << location() << "Unexpected " << Parser::token() << "\n";
            error();
        }
        return false;
    }
}

Class* Parser::clazz(String* scope) {
    // This function parses a class or interface definition, containing methods
    // and attributes.
    LocationAnchor loc(this);

    // Read in the name of the class, and any generic parameters
    std::string qn = value();
    if (token() != Token::TYPE) {
        err_ << location() << "Expected a type name, not '";
        err_ << token() << "'\n";
        qn = "";
    }
    if (!scope->string().empty()) {
        qn = scope->string() + "::" + qn;
    }
    next();

    if (token() == Token::ASSIGN) {
        return alternate(name(qn));
    }

    Generic::Ptr generics = generic_list();
    Type::Ptr type = new Type(loc, name(qn), generics, env_);
    type_ = type;
    
    // Parse the prototype for the class.
    expect(Token::LESS);
    Type::Ptr proto = Parser::type();
    if (!proto->is_proto()) {
        err_ << location() << "Invalid prototype\n";
        env_->error();
    }
    expect(Token::LEFT_BRACE);

    // Parse the comment, if present
    String::Ptr comment = Parser::comment(); 
    Feature::Ptr members = feature_list();
    expect(Token::RIGHT_BRACE);
    file_alias(type->qualified_name()->string());
    type_ = 0;
    return new Class(loc, env_, type, proto, comment, members);
}

Class* Parser::alternate(String* name) {
    // Parses the RHS of an alternate type, i.e., a union or an enum that has
    // been declared in the one-line form.
    LocationAnchor loc(this);
    next();

    // This is the LHS of the union decl, i.e., LHS in type = ... so we
    // don't need an implicit import here 
    Type::Ptr type = new Type(loc, name, 0, env_);
    switch (token()) {
    case Token::TYPE:
        file_alias(type->qualified_name()->string());
        return new Class(loc, env_, type, alternate_list());
    case Token::CONSTANT:
        file_alias(type->qualified_name()->string());
		return new Class(loc, env_, type, constant_list()); 
    default:
        err_ << location() << "Expected a type or constant, not ";
        err_ << token() << "'\n";
        error();
        return 0;
    }
}

Feature* Parser::feature_list() {
    // Parses the list of class members, functions, and attributes
    Feature* members = 0;
    while (token() == Token::IDENTIFIER || token() == Token::OPERATOR
        || token() == Token::CONSTANT || token() == Token::TYPE) {
        members = append(members, feature());
        if (error_) {
            while (token() != Token::SEPARATOR && token() != Token::END
                && token() != Token::RIGHT_BRACE) {
                next();
            }
            next();
            error_ = 0;
        } else if (token() == Token::SEPARATOR) {
            next();
        }
    }
    return members; 
}

Feature* Parser::feature() {
    // Parses an attribute or function that is a member of a class.
    LocationAnchor loc(this);
    if (token(1) == Token::LEFT_PARENS) {
        Function* func = function();
        implicit_self(func, type_);
        return func;
    } else if (token() == Token::TYPE) {
        return composite(); 
    } else if (token() == Token::CONSTANT) {
        return constant();
    } else {
        return attribute(); 
    }
}

Attribute* Parser::composite() {
    // Returns an attribute representing an embedded type
    LocationAnchor loc(this);
    Feature::Flags flags = Feature::EMBEDDED|Feature::COMPONENT;
    Type::Ptr type = Parser::type();
    Expression::Ptr empty(new Empty(loc));
    return new Attribute(loc, env_, type->name(), flags, type, empty); 
}

Constant* Parser::constant() {
    // Parses a constant, either within a class or in the namespace of a module.
    LocationAnchor loc(this);
    String* id = 0;
    if (token() == Token::CONSTANT) {
        id = name(value());
        next(); 
    } else {
        err_ << location() << "Expected a constant\n";
        error();
        id = name("");
    }
    Feature::Flags flags = Parser::flags();

    // Read the initializer, which is optional.
    Expression::Ptr init;
    Type::Ptr type = env_->top_type();
    if (token() == Token::ASSIGN) {
        next();
        init = expression();
    } else if (token() == Token::TYPE) {
        type = Parser::type();
        init = new Empty(loc);
    }
   
    Constant* con = new Constant(loc, env_, id, flags, type, init); 
    file_->constant(con);
    return con;
}

Attribute* Parser::attribute() {
    // Parses an attribute of a class.  The signature of an attribute is:
    // id flag* type? ('=' expression)?
    LocationAnchor loc(this);
    String::Ptr id = identifier();
    Feature::Flags flags = Parser::flags();

    // Read the explicit type, if present
    Type::Ptr type = env_->top_type();
    if (token() == Token::TYPE || token() == Token::COLON) {
        type = Parser::type(); 
    }

    // Read the initializer, if present
    Expression::Ptr init = 0;
    if (token() == Token::ASSIGN) {
        next();
        init = expression();
    } else {
        init = new Empty(loc);
        if (type->is_top()) {
            err_ << location() << "Expected '=' or a type name\n"; 
            error();
            init->type(env_->top_type());
        }
    }
    expect(Token::SEPARATOR);

    return new Attribute(loc, env_, id, flags, type, init);
}

Function* Parser::function() {
    // Parses a free function, which does not belong to any class.  A function
    // has the following syntax: name([id type]*) flags? type? block?
    LocationAnchor loc(this);
    String::Ptr id = name("");
    if (token() == Token::IDENTIFIER) {
        id = identifier();
    } else if (token() == Token::OPERATOR) {
        id = name(value());
        next();
    } else if (token() == Token::FUNC) {
        id = name("@call"); // Closure
        next();
    }
    expect(Token::LEFT_PARENS);

    // Now parse the formal list of arguments, e.g., "a Int, b String"
    Formal::Ptr formals = formal_list();
    expect(Token::RIGHT_PARENS);

    // For each @case, use a different mangled name based on the function arg
    // type name
    if (id->string() == "@case") {
        Class::Ptr cls = formals?formals->type()->clazz():0;
        std::string nm = cls?cls->label()->string():"";
        if (formals && formals->type()->is_generic()) {
            nm = formals->type()->name()->string();
        }
        id = name(std::string("@case_")+nm);
    }


    // Parse flags
    Feature::Flags flags = Parser::flags();      

    // Parse the return value of the function
    Type::Ptr ret = env_->void_type();
    if (token() == Token::TYPE || token() == Token::COLON) {
        ret = Parser::type();
    }

    // Parse the function body
    Block::Ptr block;
    if (token() == Token::LEFT_BRACE) {
        block = Parser::block();
    }
    return new Function(loc, env_, id, formals, flags, ret, block);
}

Formal* Parser::formal_list() {
    // Parses a list of formal arguments, i.e., (name [Type],)* name[Type]
    LocationAnchor loc(this);
    Formal* formals = 0;
    while (token() == Token::IDENTIFIER) {
        LocationAnchor loc(this);
        String::Ptr nm = identifier();
        Type::Ptr type = env_->any_type();
        if (token() == Token::TYPE || token() == Token::COLON) { 
            type = Parser::type();
        }
        formals = append(formals, new Formal(loc, nm, type));
        if (token() == Token::COMMA) {
            next(); 
        } else {
            break;
        }
    }
    return formals;
}

Type* Parser::type() {
    // Parses a type name, including generics.  A type has the following 
    // syntax: id [type (, type)+]?

    // Read in a type variable, i.e., :a, :b, etc.  These only appear in class
    // definitions for classes with type variables.
    LocationAnchor loc(this);
    if (token() == Token::COLON) {
        next();
        String::Ptr id = env_->name(":"+identifier()->string());
        return new Type(loc, id, 0, env_);
    }
    String::Ptr qn = scope();

    // Now read in any generic type parameters for this type.
    Generic::Ptr generics;
    if (token() == Token::LEFT_BRACKET) {
        next();
        while (true) {
            if (token() == Token::TYPE) {
                generics = append(generics, new Generic(type()));
            } else if (token() == Token::COLON) {
                next();
                String::Ptr id = env_->name(":"+identifier()->string());
                Type::Ptr type = new Type(location(), id, 0, env_);
                generics = append(generics, new Generic(type));
            } else {
                break;
            }
            if (token() == Token::COMMA) {
                next();
            } else {
                break;
            }
        } 
        expect(Token::RIGHT_BRACKET);
    }
    if (qn->is_empty()) {
        return env_->top_type();
    } else {
        Type* type = new Type(loc, qn, generics, env_); 
        implicit_import(type);
        return type;
    }
}

String* Parser::maybe_scope() {
    // Parses a scope name, i.e., (id ::)+.  If the tokens at the current input
    // do not match this pattern, then this function returns the scope of the
    // current module.  This function does NOT parse the last component after
    // the last :: operator.
    std::string scope;
    while (token() == Token::TYPE && token(1) == Token::SCOPE) {
        scope += value();
        next();
        next();
    }
    return scope.empty() ? module_->name() : name(scope);
}

String* Parser::scope() {
    // Parses a scope name, e.g.: id (:: id)?  If the token stream is invalid,
    // then the name returned is the empty string.
    std::string scope;
    while (token() == Token::TYPE) {
        scope += value();
        next();
        if (token() == Token::SCOPE && token(1) == Token::TYPE) {
            scope += "::";
            next();
        } else {
            break;
        }
    }

    if (scope.empty() && !error_) {
        err_ << location() << "Expected a type name, not '";
        err_ << token() << "'\n";
        error();
    }
    return name(scope);
}

Block* Parser::block() {
    // Parses a block, e.g. { stmt* }.  If a parse error occurs, the routine
    // will attempt to find the next '}' and bail out, returning as much code
    // as it can.
    LocationAnchor loc(this);
    expect(Token::LEFT_BRACE);
    String::Ptr comment = Parser::comment();
    Statement::Ptr statements;
    while (token() != Token::RIGHT_BRACE && token() != Token::END) {
        statements = append(statements, statement());
        if (error_) {
            while (token() != Token::SEPARATOR && token() != Token::END
                && token() != Token::RIGHT_BRACE) {

                next();
            }
            next();
            error_ = 0;
        } else if (token() == Token::SEPARATOR) {
            next();
        } else {
            break;
        }
    }
    expect(Token::RIGHT_BRACE);
    return new Block(loc, comment, statements);
}

String* Parser::comment() {
    // Parses a block comment, which can be attached to functions/classes
    std::string text;
    while (token() == Token::COMMENT) {
        std::string line = value();
        if (line.empty()) {
            text += "\n\n" + line;
        } else if (text.empty()) {
            text = line;
        } else {
            text += " " + line;
        }
        next();
    }
    return new String(text);
}

Statement* Parser::statement() {
    LocationAnchor loc(this);
    switch(token().type()) {
    case Token::IF: return conditional();
    case Token::WHILE: return while_loop();
    case Token::FOR: return for_loop();
    case Token::LET: return let();
    case Token::MATCH: return match();
    case Token::RETURN: {
        next();
        if (token() == Token::SEPARATOR) {
            return new Return(loc, new Empty(loc));
        } else {
            Expression::Ptr expr = expression();
            return new Return(loc, expr);
        }
        break;
    }
    case Token::YIELD: {
        next();
        return new Yield(loc, 0);
    }
    default: {
        Expression::Ptr expr = expression();
        return new Simple(loc, expr);
    }
    }
}

Match* Parser::match() {
    // Parses a match expression, i.e., match expr { ... }
    LocationAnchor loc(this);
    expect(Token::MATCH);
    Expression::Ptr guard = expression();
    expect(Token::LEFT_BRACE);
    Case::Ptr cases = case_list();
    expect(Token::RIGHT_BRACE);
    return new Match(loc, guard, cases);
}

Case* Parser::case_list() {
    // Parses a single case block, e.g., expr: statement*
    LocationAnchor loc(this);
    
    Case* cases = 0;
    while (token() != Token::RIGHT_BRACE && token() != Token::END) {
        cases = append(cases, single_case());
    }
    return cases;
}

Case* Parser::single_case() {
    // Matches a single case block, i.e., guard: statement, statement, etc.
    LocationAnchor loc(this);
    expect(Token::WITH);
    Statement::Ptr statements;
    Expression::Ptr guard = expression();
    expect(Token::COLON);

    while (token() != Token::RIGHT_BRACE && token() != Token::WITH 
        && token() != Token::END) {

        statements = append(statements, statement());
        if (error_) {
            while (token() != Token::SEPARATOR && token() != Token::END
                && (token() == Token::RIGHT_BRACE || token() == Token::WITH)) {

                next();
            }
            next();
            error_ = 0;
        } else if (token() == Token::SEPARATOR) {   
            next();
        } else {
            break;
        }
    }
    return new Case(loc, guard, statements);
}

void Parser::import() {
    // Parses an import declaration, i.e., import type (, type)*
    LocationAnchor loc(this);
    expect(Token::IMPORT);
    if (token() != Token::TYPE) {
        err_ << location() << "Expected a type name in import statement\n";
        error();
    }

    while (token() == Token::TYPE) {
        // Add a new non-qualified import; any type that is in the imported
        // module will be added to the file's namespace.  If a qualified import
        // already exists, then make that import non-qualified.
        String::Ptr scope = Parser::scope();
        file_->import(new Import(loc, scope, 0));
        if (token() == Token::COMMA) {
            next();
        } else {
            break;
        }
    }
}

String* Parser::identifier() {
    // Parses an identifier, i.e., [a-z][A-Z_0-9]* (leading lowercase letter)
    if (token() == Token::IDENTIFIER) {
        String* id = name(value());
        next();
        return id;
    } else {
        err_ << location() << "Expected an identifier\n";
        error();
        String* id = name("");
        return id;
    }
}

Feature::Flags Parser::flags() {
    // Parses keyword flags, e.g., native, immutable, private, weak
    Feature::Flags flags = 0;
    while (true) {
        switch (token().type()) {
        case Token::NATIVE: next(); flags |= Feature::NATIVE; break;
        case Token::IMMUTABLE: next(); flags |= Feature::IMMUTABLE; break;
        case Token::EMBEDDED: next(); flags |= Feature::EMBEDDED; break;
        case Token::PRIVATE: next(); flags |= Feature::PRIVATE; break;
        case Token::WEAK: next(); flags |= Feature::WEAK; break;
        default: return flags;
        }

    }
    return flags;
}

Expression* Parser::expression_list() {
    // Parses an argument list of expressions: expr? (, expr)?
    if (token() == Token::RIGHT_PARENS || token() == Token::RIGHT_BRACKET) {
        return 0;
    }

    Expression* args = 0;
    while (true) {
        args = append(args, expression());
        if (token() == Token::COMMA) {
            next();
        } else {
            break;
        }
    }
    return args;
}

Expression* Parser::expression() {
    if (token() == Token::IDENTIFIER) {
        if (token(1) == Token::ASSIGN || token(1) == Token::TYPE) {
            return assignment();
        }
    } else if (token() == Token::FUNC) {
        return closure();
    }
    return logical_or();
}

Expression* Parser::closure() {
    // Parses a closure expression: func(arg? (, arg)?) { ... }
    LocationAnchor loc(this);
    Function::Ptr func = function(); 
    std::string qn = module_->name()->string();
    if (qn.empty()) {
        qn = "_Closure" + stringify(func);
    } else {
        qn += "::_Closure" + stringify(func);
    }
    Type::Ptr type = new Type(loc, name(qn), 0, env_);
    Formal::Ptr self = new Formal(loc, name("self"), type);
    self->next(func->formals());
    func->formals(self);

    Type::Ptr object = new Type(loc, name("Object"), 0, env_);
    Class::Ptr clazz = new Class(loc, env_, type, object, 0, func);
    clazz->flags(Feature::CLOSURE);
    module_->feature(clazz);
    file_->feature(clazz);
    return new Closure(loc, func, clazz);
}

Assignment* Parser::assignment() {
    // Parses an assignment, either a = expr, or a type = expr
    LocationAnchor loc(this);
    String::Ptr id = identifier();
    Type::Ptr type = env_->top_type();
    if (token() == Token::TYPE) {
        type = Parser::type(); 
    }
    Expression::Ptr init;
    if (token() == Token::ASSIGN) {
        next();
        init = expression();
    } else {
        init = new Empty(loc);
    }
    return new Assignment(loc, id, type, init);
}

Expression* Parser::logical_or() {
    // Parse a logical or, i.e., an expression of the form: expr (or expr)+
    LocationAnchor loc(this);
    Expression* expr = logical_and();    
    while (token() == Token::OR) {
        next();
        Expression* right = logical_and();
        expr = new Binary(loc, name("or"), expr, right);
    }
    return expr;
}

Expression* Parser::logical_and() {
    // Parse a logical and, i.e., an expression of the form: expr (and expr)+
    LocationAnchor loc(this);
    Expression* expr = bitwise_or();
    while (token() == Token::AND) {
        next();
        Expression* right = bitwise_or();
        expr = new Binary(loc, name("and"), expr, right);
    }
    return expr;
}

Expression* Parser::bitwise_or() {
    // Parse a bitwise or, i.e., an expression of the form: expr (| expr)+
    LocationAnchor loc(this);
    Expression* expr = bitwise_xor();
    while (token() == Token::ORB) {
        next();
        Expression* right = bitwise_xor();
        expr = op(loc, "@bitor", expr, right);
    }
    return expr;
}

Expression* Parser::bitwise_xor() {
    // Parse a bitwise xor, i.e., an expression of the form: expr (xor expr)+
    LocationAnchor loc(this);
    Expression* expr = bitwise_and();
    while (token() == Token::XORB) {
        next();
        Expression* right = bitwise_and();
        expr = op(loc, "@bitxor", expr, right);
    }
    return expr;
}

Expression* Parser::bitwise_and() {
    // Parse a bitwise and, i.e., an expression of the form: expr (& expr)+
    LocationAnchor loc(this);
    Expression* expr = equality();
    while (token() == Token::ANDB) {
        next();
        Expression* right = equality();
        expr = op(loc, "@bitand", expr, right);
    }
    return expr;
}

Expression* Parser::equality() {
    // Parse an equality, i.e., an expression of the form: expr (== expr)+
    LocationAnchor loc(this);
    Expression* expr = relational();
    while (true) {
        switch (token().type()) {
        case Token::EQUAL: {
            next();
            Expression* right = relational();
            expr = op(loc, "@equal", expr, right);
            break;
        }
        case Token::NOT_EQUAL: {
            next(); 
            Expression* right = relational();
            expr = op(loc, "not", op(loc, "@equal", expr, right));
            break;
        }
        case Token::IS: {
            next();
            expr = new Is(loc, expr, type());
            break;
        }
        case Token::MATCH_OP: {
            next();
            Expression* right = relational();
            expr = op(loc, "@match", expr, right);
            break;
        }
        default: 
            return expr;
        }
    }
    return expr;
}

Expression* Parser::relational() {
    // Parse a relational expr, i.e., an expression of the form: expr (& expr)+
    LocationAnchor loc(this);
    Expression* expr = shift();
    while (true) {
        switch (token().type()) {
        case Token::GREATER_OR_EQ: {
            next();
            Expression::Ptr right = shift();
            expr = op(loc, "not", op(loc, "@less", expr, right));
            break;
        } 
        case Token::GREATER: {
            next();
            Expression::Ptr right = shift();
            Expression::Ptr less = op(loc, "@less", expr, right);
            Expression::Ptr equal = op(loc, "@equal", expr, right);
            String::Ptr id = name("or");
            Expression::Ptr child = new Binary(loc, id, less, equal);
            expr = op(loc, "not", child); 
            break;
        }
        case Token::LESS: {
            next();
            Expression::Ptr right = shift();
            expr = op(loc, "@less", expr, right);
            break;
        }
        case Token::LESS_OR_EQ: {
            next();
            Expression::Ptr right = shift();
            Expression::Ptr less = op(loc, "@less", expr, right);
            Expression::Ptr equal = op(loc, "@equal", expr, right);
            String::Ptr id = name("or");
            expr = new Binary(loc, id, less, equal);
        }
        default: return expr;
        }
    }
    return expr;
}

Expression* Parser::shift() {
    // Parse a shift, i.e., an expression of the form: expr ( << expr)+
    LocationAnchor loc(this);
    Expression* expr = addition();
    while (true) {
        switch(token().type()) {
        case Token::LEFT_SHIFT: { 
            next();
            Expression* right = addition();
            expr = op(loc, "@shift", expr, right); 
            break;
        }
        case Token::RIGHT_SHIFT: {
            next(); 
            Expression* right = addition();
            expr = op(loc, "@unshift", expr, right); 
            break;
        }
        default: return expr;
        }
    }    
    return expr;
}

Expression* Parser::addition() {
    // Parse an add/sub, i.e., an expression of the form: expr (+ expr)+
    LocationAnchor loc(this);
    Expression* expr = mult();
    while (true) {
        switch(token().type()) {
        case Token::ADD: {
            next(); 
            Expression* right = mult();
            expr = op(loc, "@add", expr, right);
            break;
        }
        case Token::SUB: {
            next();
            Expression* right = mult();
            expr = op(loc, "@sub", expr, right);
            break;
        }
        default: return expr;
        }
    }    
    return expr;
}

Expression* Parser::mult() {
    // Parse an add/sub, i.e., an expression of the form: expr (+ expr)+
    LocationAnchor loc(this);
    Expression* expr = unary();
    while (true) {
        switch(token().type()) {
        case Token::MUL: {
            next(); 
            Expression* right = unary();
            expr = op(loc, "@mul", expr, right);
            break;
        }
        case Token::DIV: {
            next();
            Expression* right = unary();
            expr = op(loc, "@div", expr, right);
            break;
        }
        case Token::MOD: {
            next();
            Expression* right = unary();
            expr = op(loc, "@mod", expr, right);
            break;
        }
        default: return expr;
        }
    }    
    return expr;
}

Expression* Parser::unary() {
    // Unary boolean not and logical complement (e.g.: (not|~|-) expr)
    LocationAnchor loc(this);
    switch (token().type()) {
    case Token::NOT: {
        next(); 
        Expression* right = unary(); 
        return op(loc, "not", right);
    }
    case Token::COMPL: {
        next();
        Expression* right = unary();
        return op(loc, "@compl", right, 0);
    }
    case Token::SUB: {
        next();
        Expression* right = unary();
        return op(loc, "@neg", right, 0);
    }
    default: return increment();
    }
    return increment();
}

Expression* Parser::increment() {
    // Increment or decrement expression, i.e., (++|--)? expr.  This can only
    // be called on an identifier.
    LocationAnchor loc(this);
    switch (token().type()) {
    case Token::INCREMENT: {
        next();
        String::Ptr id = identifier();
        Expression::Ptr t1 = new IntegerLiteral(loc, env_->integer("1"));    
        Expression::Ptr t2 = new IdentifierRef(loc, name(""), id);
        Expression::Ptr t3 = op(loc, "@add", t2, t1);
        return new Assignment(loc, id, env_->top_type(), t3);
    }
    case Token::DECREMENT: {
        next();
        String::Ptr id = identifier();
        Expression::Ptr t1 = new IntegerLiteral(loc, env_->integer("1"));    
        Expression::Ptr t2 = new IdentifierRef(loc, name(""), id);
        Expression::Ptr t3 = op(loc, "@sub", t2, t1);
        return new Assignment(loc, id, env_->top_type(), t3);
    }
    default: return call();
    }
    return call();
}

Expression* Parser::call() {
    // Parses a call expression, i.e.: expr ( arg? (, arg)* ) +
    LocationAnchor loc(this);
    Expression* expr = member();
    while (token() == Token::LEFT_PARENS) {
        next();
    
        // Read in the arguments to the function.
        Expression::Ptr args = expression_list();
        expect(Token::RIGHT_PARENS);
        if (token() == Token::FUNC) {
            args = append(args, closure());
        }
        expr = new Call(loc, expr, args);  
    }
    return expr;
}

Expression* Parser::member() {
    // Parses the member operator, i.e., '.', such as: expr [. ident]?  Also
    // parses the [] operator, and () operator
    LocationAnchor loc(this);
    Expression* expr = construct();
    while (true) {
        switch (token().type()) {
        case Token::LEFT_PARENS: {
            // Parse the () operator; read in the arguments to the function.
            next();
            Expression::Ptr args = expression_list();
            expect(Token::RIGHT_PARENS);
            if (token() == Token::FUNC) {
                args = append(args, closure());
            }
            expr = new Call(loc, expr, args);  
            break;
        }
        case Token::LEFT_BRACKET: {
            // Parse the [] array access operator, both for assignments and
            // for reads.
            next();
            Expression* args = expression_list();
            Member* mem = 0;
            expect(Token::RIGHT_BRACKET);
            if (token() == Token::ASSIGN) {
                next();
                args = append(args, expression());
                mem = new Member(loc, expr, name("@insert")); 
            } else {
                mem = new Member(loc, expr, name("@index"));    
            }
            expr = new Call(loc, mem, args); 
            break;
        }
        case Token::DOT: {
            // Parse the '.' member access operator
            next(); 
            String* id = identifier();
            if (token() == Token::ASSIGN) {
                next();
                String* set = name(id->string()+"=");
                Member* mem = new Member(loc, expr, set);
                Expression* args = expression();
                return new Call(loc, mem, args);
            } else {
                expr = new Member(loc, expr, id);
            }
            break;
        }
        default: return expr;
        }
    }
    return expr;
}

Expression* Parser::construct() {
    // Constructor expression, i.e., type ( arg? (, arg)*).  This function
    // also handles qualified constants and identifiers, since these
    // expressions all start with the same prefix.
    LocationAnchor loc(this);
    if (token() == Token::TYPE) {
        Type::Ptr type = Parser::type();
        if (token() == Token::SCOPE) {
            next();
            String::Ptr scope = type->qualified_name();
            if (token() == Token::CONSTANT) {
                String::Ptr id = env_->name(value());
                next();
                implicit_import(scope, Import::QUALIFIED|Import::OPTIONAL);
                return new ConstantRef(loc, scope, id); 
            } else {
                implicit_import(scope);
                return new IdentifierRef(loc, scope, identifier()); 
            }
        }
        
        // Read in the ctor arguments
        expect(Token::LEFT_PARENS); 
        Expression::Ptr args = expression_list();
        expect(Token::RIGHT_PARENS);
        if (token() == Token::FUNC) {
            args = append(args, closure());
        }
        return new Construct(loc, type, args);
    } else {
        return literal();
    }
}

Generic* Parser::generic_list() {
    // Returns a list of generic variables, including the brackets.  For
    // example: [:a, :b, :c]
    if (token() != Token::LEFT_BRACKET) {
        return 0;
    }
    next();

    // Parse one generic parameter per iteration of this loop
    Generic* generics = 0;
    while (token() == Token::COLON) {
        LocationAnchor loc(this);
        next();
        String::Ptr id = env_->name(":"+identifier()->string());
        Type::Ptr type = new Type(loc, id, 0, env_);
        generics = append(generics, new Generic(type));
        if (token() == Token::COMMA) {
            next();
        } else {
            break;
        }
    }
    if (!generics) {
        err_ << location() << "Expected a generic type, not '";
        err_ << token() << "'\n";
        error();
    }
    expect(Token::RIGHT_BRACKET);
    return generics;
}

Feature* Parser::constant_list() {
    // Parses a list of alternates in a union def, i.e., A | B | C
    Feature* alt = 0; 
    while (token() == Token::CONSTANT) {
        alt = append(alt, static_cast<Feature*>(Parser::constant())); 
        if (token() == Token::ORB) {
            next(); 
        } else {
            return alt;
        }
    } 
    err_ << location() << "Expected a type name, not '";
    err_ << token() << "'\n";
    error();
    return alt;
}

Type* Parser::alternate_list() {
    // Parses a list of alternates in a union def, i.e., A | B | C
    Type* alt = 0; 
    while (token() == Token::TYPE) {
        alt = append(alt, Parser::type()); 
        if (token() == Token::ORB) {
            next(); 
        } else {
            return alt;
        }
    } 
    err_ << location() << "Expected a type name, not '";
    err_ << token() << "'\n";
    error();
    return alt;
}

Expression* Parser::byte_or_char_literal() {
    // Parses the byte literal and converts it to canonical decimal form.
    String::Ptr val(new String(value()));
    uint64_t code = 0;
    try {
        std::string const unescaped = val->unescaped();
        if (unescaped.size() > 1) {
            if (token().type() == Token::CHAR) {
                err_ << location() << "Character literal is too long\n";
            } else if (token().type() == Token::BYTE) {
                err_ << location() << "Byte literal is too long\n";
            } else {
                assert(!"Unreachable");
            }
        } else {
            code = (uint8_t)unescaped[0];
        }
    } catch (StringEscapeError const& err) {
        err_ << location() << err.message() << "\n";
        error(); 
    }
    String::Ptr integer = env_->integer(stringify(code));
    return new IntegerLiteral(location(), integer);
}

Expression* Parser::literal() {
    // Parses a literal expression, variable, or parenthesized expression
    Expression* expr = 0;
    switch(token().type()) {
    case Token::LEFT_BRACKET:
        return array_literal();
        break;
    case Token::LEFT_BRACE:
        return hash_literal();
        break;
    case Token::FLOAT:
        expr = new FloatLiteral(location(), env_->integer(value()));
        break;
    case Token::INTEGER:
        expr = new IntegerLiteral(location(), env_->integer(value()));
        break;
    case Token::STRING: {
        try {
            String::Ptr str = env_->string(value());
            str->unescaped();
            expr = new StringLiteral(location(), str);
        } catch (StringEscapeError const& err) {
            err_ << location() << err.message() << "\n";
            expr = new StringLiteral(location(), env_->string(""));
        }
        break;
    }
    case Token::NIL:
        expr = new NilLiteral(location());
        break;
    case Token::EOF_LITERAL:
        expr = new IntegerLiteral(location(), env_->integer("-1"));
        expr->type(env_->char_type());
        break;
    case Token::CHAR: {
        expr = byte_or_char_literal();
        expr->type(env_->char_type());
        break;
    }
    case Token::BYTE: {
        expr = byte_or_char_literal();
        expr->type(env_->byte_type());
        break;
    }
    case Token::BOOL:
        if (value() == "true") {
            expr = new BooleanLiteral(location(), env_->integer("1"));
        } else if (value() == "false") {
            expr = new BooleanLiteral(location(), env_->integer("0"));
        }
        break;
    case Token::IDENTIFIER:
        expr = new IdentifierRef(location(), name(""), name(value()));
        break;
    case Token::CONSTANT:
        expr = new ConstantRef(location(), name(""), name(value()));
        break;
    case Token::LEFT_PARENS:
        next();
        expr = expression();
        expect(Token::RIGHT_PARENS);
        return expr;  
    case Token::STRING_BEGIN:
        return string();
    case Token::REGEX:
        return regex(); 
    default:
        if (!error_) {
            err_ << location() << "Unexpected " << token() << "\n";
            error();
        }
        return new ParseError(location());
    }
    next();
    return expr;
}

Expression* Parser::array_literal() {
    // Returns an expression constructing an Array from an array literal.
    LocationAnchor loc(this);
    Expression::Ptr args;
    expect(Token::LEFT_BRACKET);
    while (token() != Token::RIGHT_BRACKET) {
        args = append(args, expression());
        if (token() != Token::COMMA) {
            break;
        }
        next();
    }
    expect(Token::RIGHT_BRACKET);
    return new ArrayLiteral(loc, args);
}

Expression* Parser::hash_literal() {
    // Returns an expression constructing a Hash from a hash literal.
    LocationAnchor loc(this);
    Expression::Ptr args;
    expect(Token::LEFT_BRACE);
    while (token() != Token::RIGHT_BRACE) {
        args = append(args, pair());
        if (token() != Token::COMMA) {
            break;
        }
        next();
    }
    expect(Token::RIGHT_BRACE);
    return new HashLiteral(loc, args);
}

Expression* Parser::pair() {
    // Returns a Pair expression, e.g., expr1 : expr2
    LocationAnchor loc(this);
    Type::Ptr type(new Type(loc, name("Pair"), 0, env_));
    Expression::Ptr args = expression();  
    expect(Token::COLON);
    args = append(args, expression());
    return new Construct(loc, type, args);
     
}

Expression* Parser::regex() {
    // Returns an expression constructing a Regex object from a Regex literal.
    LocationAnchor loc(this);
    Type::Ptr type = new Type(loc, name("Regex::Regex"), 0, env_);
    Expression::Ptr arg = new StringLiteral(loc, env_->string(value()));
    next();
    implicit_import(type);
    return new Construct(loc, type, arg);
}

Expression* Parser::string() {
    // String interpolation: ".*#{ expr } sdlkfjldkf #{
    LocationAnchor loc(this);
    Expression* expr = new StringLiteral(location(), env_->string(value()));
    expect(Token::STRING_BEGIN);
    
    while (true) {
        Expression* sub = expression();
        Expression* tmp = op(loc, "str?", sub, 0); 
        expr = op(loc, "@add", expr, tmp);
        if (token() == Token::STRING_BEGIN) { 
            tmp = new StringLiteral(location(), env_->string(value()));
            next();
            expr = op(loc, "@add", expr, tmp);
        } else {
            break;
        }
    }
    Expression::Ptr tmp = new StringLiteral(location(), env_->string(value()));
    expect(Token::STRING_END);
    expr = op(loc, "@add", expr, tmp);
    return expr;
}

Statement* Parser::conditional() {
    // Conditional statement: if expr block (else if expr block)? (else block)?
    LocationAnchor loc(this);
    expect(Token::IF); 
    Expression::Ptr guard = expression();
    Statement::Ptr true_branch = block();
    Statement::Ptr false_branch;
    if (token() == Token::SEPARATOR && token(1) == Token::ELSE) {
        next(); 
    }
    if (token() == Token::ELSE) {
        next();
        if (token() == Token::IF) {
            false_branch = conditional();
        } else {
            false_branch = block();
        }
    }
    return new Conditional(loc, guard, true_branch, false_branch);
}

Statement* Parser::let() {
    // Parses a let expression: let (guard)+ { block }
    LocationAnchor loc(this);
    expect(Token::LET);
    Assignment::Ptr assign = assignment(); 
    while (token() == Token::COMMA) {
        next();
        assign = append(assign, assignment());
    } 
    return new Let(loc, assign, block()); 
}

Statement* Parser::while_loop() {
    // While loop: while expr block
    LocationAnchor loc(this);
    expect(Token::WHILE);
	Expression* expr = expression(); // Must be separate (order of eval)
    return new While(loc, expr, block()); 
}

Statement* Parser::for_loop() {
    // For loop: for x in expr block.  We have to build up the syntactic sugar
    // for the loop by using an iterator and a while loop.
    LocationAnchor loc(this);
    expect(Token::FOR);
    String* id = identifier();
    expect(Token::IN);
    Expression* guard = expression();
    Statement* block = Parser::block();
    String* i = name("_i");

    // _i = guard.iter()
    Expression* t1 = op(loc, "iter", guard, 0);
    Assignment* t2 = new Assignment(loc, i, env_->top_type(), t1);

    // while (_i.more()) 
    Expression* t3 = new IdentifierRef(loc, name(""), i);
    Expression* t4 = op(loc, "more?", t3, 0); 

    // id = _i.next()    
    Expression* t5 = new IdentifierRef(loc, name(""), i);
    Expression* t6 = op(loc, "next", t5, 0);
    Assignment* t7 = new Assignment(loc, id, env_->top_type(), t6);
    Let* t8 = new Let(loc, t7, block);
    
    // Loop body
    While* t10 = new While(loc, t4, new Block(loc, 0, t8)); 
    return new Let(loc, t2, t10);
}

void Parser::module_feature(Feature* feature, String* scope) {
    // If the scope of the feature is fully-qualified, then use that to select
    // the module.  Otherwise, insert the feature into the current module. 
    if (!feature) {
        return;
    }
    std::string parent = Import::parent_scope(scope->string());
    std::string sub = Import::sub_scope(scope->string());
    // First attempt a look up of the form [Module::]+Class
    Module::Ptr module = env_->module(env_->name(parent));
    if (module) {
        String::Ptr id = env_->name(sub);
        Class::Ptr clazz = dynamic_cast<Class*>(module_->feature(id));
        if (clazz) {
            implicit_self(feature, clazz->type());
            clazz->feature(feature);
            return;
        } 
    }
    // Now do a lookup of the form [Module::]*Module
    module = env_->module(scope);
    if (!module) {
        module = new Module(location(), env_, scope);
        env_->module(module);
    } 
    module->feature(feature);
    file_->feature(feature);
}

void Parser::implicit_import(Type* type, Flags flags) {
    // Adds an implicit import for 'type' to the parse tree, if necessary.
    if (!type->is_generic()) {
        implicit_import(type->scope(), flags);
    }
}

void Parser::implicit_import(String* scope, Flags flags) {
    // Adds a new import to the module/file if it hasn't been added already. 
    Import::Ptr import = file_->import(scope); 
    if (!scope->string().empty() && !import) {
        import = new Import(location(), scope, flags);
        //module_->import(import);
        file_->import(import);
        file_alias(scope->string());
    }
}

Expression* Parser::op(const LocationAnchor& loc, const std::string& op, 
                       Expression* a, Expression* b) {
    // Binary expression: Note that eventually this may be replaced by an 
    // actual Operator node
    Member::Ptr mem = new Member(loc, a, name(op));
    return new Call(loc, mem, b);     
}

Expression* Parser::op(const LocationAnchor& loc, const std::string& op,
                       Expression* expr) {
    // Unary expression
    return new Unary(loc, name(op), expr);
}

void Parser::implicit_self(Feature* feature, Type* type) {
    // Adds the implicit 'self' parameter to a function 
    Function* func = dynamic_cast<Function*>(feature);
    if (!func) { return; }
    if (func->name()->string() == "@init") { return; }
    String* id = name("self");
    Formal* self = new Formal(feature->location(), id, type);
    self->next(func->formals());
    func->formals(self);
}
