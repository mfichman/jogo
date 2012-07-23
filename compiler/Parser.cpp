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

    if (!env->no_default_libs()) {
        input("Apollo");
        env_->lib("Apollo");
    }
    for (int i = 0; i < env->inputs(); i++) {
        input(env->input(i));
    }
    if (!env->gen_library() && !env->no_default_libs()) {
        input("Boot::Main");
    }
}

void Parser::library(const std::string& import) {
    // Reads in a library file given by "import" (not including the .api
    // extension, which is automatically added)

    std::string file = import + ".api";
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


void Parser::input(const std::string& import) {
    // Reads in a file or directory after searching through the list of include
    // paths.  The includes paths are searched in the order they are specified,
    // so if duplicate files are present in two include directories, then only
    // the first will be parsed.
    std::string file = Import::file_name(import); 
    std::vector<std::string> tests;
    if (env_->file(name(file))) {
        return;
    }

    // File has not been loaded yet.  Search through all the given include
    // directories to find it.  
    for (int i = 0; i < env_->includes(); i++) {
        const std::string& prefix = env_->include(i);
        if (File::ext(file) != ".ap") {
            const std::string base = prefix + FILE_SEPARATOR + file;
            if (File::is_dir(base)) {
                Parser::dir(prefix, file);
                return;
            } 
            if (File::is_reg(base + ".ap")) {
                Parser::file(prefix, file + ".ap");
                return;
            }
            if (!env_->is_input(import) && File::is_reg(base + ".api")) {
                Parser::file(prefix, file + ".api");
                return;
            }
        } else {
            if (File::is_reg(prefix + FILE_SEPARATOR + file)) {
                Parser::file(prefix, file);
                return; 
            } 
        }
        tests.push_back(prefix + FILE_SEPARATOR + file);
        tests.push_back(prefix + FILE_SEPARATOR + file + ".api");
        tests.push_back(prefix + FILE_SEPARATOR + file + ".ap");
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
        if (self == "Boot::Main") {
            file_->is_input_file(true);
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

    // Begin parsing
    lexer_->input(file_);
    error_ = 0;
    module();

    // Now parse other modules that depend on the unit that was added
    if (!file_->is_interface_file()) {
        for (int i = 0; i < file_->imports(); i++) {
            input(file_->import(i)->scope()->string());
        }
    }
}

void Parser::dir(const std::string& prefix, const std::string& dir) {
    // Parses all files in the directory specified by 'dir'.  If 'dir' is not
    // a directory, then no files are parsed.

    for (File::Iterator i(prefix + FILE_SEPARATOR + dir); i; ++i) {
        std::string name = *i;
        const std::string ext = ".ap";
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
        // Parse a union declaration, e.g. type = type | type | type
        next();

        // This is the LHS of the union decl, i.e., LHS in type = ... so we
        // don't need an implicit import here 
        Type::Ptr type = new Type(loc, name(qn), 0, env_);
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
        }
    }

    Generic::Ptr generics = generic_list();
    Type::Ptr type = new Type(loc, name(qn), generics, env_);
    
    // Parse the type list
    expect(Token::LESS);
    Type::Ptr mixins = mixin_list();
    expect(Token::LEFT_BRACE);

    // Parse the comment, if present
    String::Ptr comment = Parser::comment(); 
    Feature::Ptr members = feature_list();
    expect(Token::RIGHT_BRACE);
    file_alias(type->qualified_name()->string());
    return new Class(loc, env_, type, mixins, comment, members);
}

Feature* Parser::feature_list() {
    // Parses the list of class members, functions, and attributes
    Feature* members = 0;
    while (token() == Token::IDENTIFIER || token() == Token::OPERATOR
                || token() == Token::CONSTANT) {

        members = append(members, feature());
        if (error_) {
            while (true) {
                if (token() == Token::SEPARATOR) {
                    break;
                }
                if (token() == Token::RIGHT_BRACE) {
                    break;
                }
                if (token() == Token::END) {
                    break;
                }
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
        if (func->name()->string() != "@init") {
            String* id = name("self");
            Formal* self = new Formal(loc, id, env_->self_type());
            self->next(func->formals());
            func->formals(self);
        }
        return func;
    } else if (token() == Token::CONSTANT) {
        return constant();
    } else {
        return attribute(); 
    }
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
    if (token() == Token::ASSIGN) {
        next();
        init = expression();
    }
   
    Constant* con = new Constant(loc, env_, id, flags, init); 
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
    Type::Ptr type = 0;
    if (token() == Token::TYPE || token() == Token::TYPEVAR) {
        type = Parser::type(); 
    }

    // Read the initializer, if present
    Expression::Ptr init = 0;
    if (token() == Token::ASSIGN) {
        next();
        init = expression();
    }  else {
        init = new Empty(loc);
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
    Formal::Ptr formals;
    while (token() == Token::IDENTIFIER) {
        LocationAnchor loc(this);
        String::Ptr nm = identifier();
        Type::Ptr type = env_->any_type();
        if (token() == Token::TYPE || token() == Token::TYPEVAR) { 
            type = Parser::type();
        }
        formals = append(formals, new Formal(loc, nm, type));
        if (token() == Token::COMMA) {
            next(); 
        } else {
            break;
        }
    }

    expect(Token::RIGHT_PARENS);

    // Parse flags
    Feature::Flags flags = Parser::flags();      

    // Parse the return value of the function
    Type::Ptr ret = env_->void_type();
    if (token() == Token::TYPE || token() == Token::TYPEVAR) {
        ret = Parser::type();
    }

    // Parse the function body
    Block::Ptr block;
    if (token() == Token::LEFT_BRACE) {
        block = Parser::block();
    }
    return new Function(loc, env_, id, formals, flags, ret, block);
}

Type* Parser::type() {
    // Parses a type name, including generics.  A type has the following 
    // syntax: id [type (, type)+]?

    // Read in a type variable, i.e., :a, :b, etc.  These only appear in class
    // definitions for classes with type variables.
    LocationAnchor loc(this);
    if (token() == Token::TYPEVAR) {
        String* id = name(value());
        Type* type = new Type(loc, id, 0, env_);
        next();
        return type;
    }
    String* qn = scope();

    // Now read in any generic type parameters for this type.
    Generic::Ptr generics;
    if (token() == Token::LEFT_BRACKET) {
        next();
        while (true) {
            if (token() == Token::TYPE) {
                generics = append(generics, new Generic(type()));
            } else if (token() == Token::TYPEVAR) {
                Type::Ptr type = new Type(location(), name(value()), 0, env_);
                generics = append(generics, new Generic(type));
                next();
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
        return env_->no_type();
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
        Import::Ptr import = new Import(loc, scope, false);
        module_->import(import);
        file_->import(import);
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
    Formal::Ptr self = new Formal(loc, name("self"), env_->self_type());
    self->next(func->formals());
    func->formals(self);

    Type::Ptr type = new Type(loc, name(qn), 0, env_);
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
    Type::Ptr type;
    Expression::Ptr init;
    if (token() == Token::TYPE) {
        type = Parser::type(); 
    }
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
        Expression::Ptr t2 = new Identifier(loc, name(""), id);
        Expression::Ptr t3 = op(loc, "@add", t2, t1);
        return new Assignment(loc, id, 0, t3);
    }
    case Token::DECREMENT: {
        next();
        String::Ptr id = identifier();
        Expression::Ptr t1 = new IntegerLiteral(loc, env_->integer("1"));    
        Expression::Ptr t2 = new Identifier(loc, name(""), id);
        Expression::Ptr t3 = op(loc, "@sub", t2, t1);
        return new Assignment(loc, id, 0, t3);
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
                return new ConstantIdentifier(loc, scope, id); 
            } else {
                implicit_import(type->qualified_name());
                return new Identifier(loc, scope, identifier()); 
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
    while (token() == Token::TYPEVAR) {
        LocationAnchor loc(this);
        Type::Ptr type = new Type(loc, name(value()), 0, env_);
        generics = append(generics, new Generic(type));
        next();
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

Type* Parser::mixin_list() {
    // Parses a list of mixins in a class definition, i.e., A, B, C
    Type* mixins = 0;
    while (token() == Token::TYPE) {
        mixins = append(mixins, Parser::type());
        if (token() == Token::COMMA) {
            next();
        } else {
            return mixins;
        }
    }
    //err_ << location() << "Expected a type name, not '";
    //err_ << token() << "'\n";
    //error();
    return mixins;
}

Expression* Parser::literal() {
    // Parses a literal expression, variable, or parenthesized expression
    Expression* expr;
    switch(token().type()) {
    case Token::FLOAT:
        expr = new FloatLiteral(location(), env_->integer(value()));
        break;
    case Token::INTEGER:
        expr = new IntegerLiteral(location(), env_->integer(value()));
        break;
    case Token::STRING:
        expr = new StringLiteral(location(), env_->string(value()));
        break;
    case Token::NIL:
        expr = new NilLiteral(location());
        break;
    case Token::EOF_LITERAL:
        expr = new IntegerLiteral(location(), env_->integer("-1"));
        expr->type(env_->char_type());
        break;
    case Token::CHAR: {
        int code = String::escape(value());
        expr = new IntegerLiteral(location(), env_->integer(stringify(code)));
        expr->type(env_->char_type());
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
        expr = new Identifier(location(), name(""), name(value()));
        break;
    case Token::CONSTANT:
        expr = new ConstantIdentifier(location(), name(""), name(value()));
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
        return new Empty(location());
    }
    next();
    return expr;
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
    if(!assign->declared_type()) {
        // This forces a new variable to be declared (i.e., if there is a
        // second variable that shadows this assignment in the enclosing scope,
        // then this "let" statement will declare a new variable rather than 
        // reusing the old one).
        assign->declared_type(env_->no_type());
    }
    while (token() == Token::COMMA) {
        assign = append(assign, assignment());
        if(!assign->declared_type()) {
            // This forces a new variable to be declared (i.e., if there is a
            // second variable that shadows this assignment in the enclosing
            // scope, then this "let" statement will declare a new variable
            // rather than reusing the old one).
            assign->declared_type(env_->no_type());
        }
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
    Assignment* t2 = new Assignment(loc, i, env_->no_type(), t1);

    // while (_i.more()) 
    Expression* t3 = new Identifier(loc, name(""), i);
    Expression* t4 = op(loc, "more?", t3, 0); 

    // id = _i.next()    
    Expression* t5 = new Identifier(loc, name(""), i);
    Expression* t6 = op(loc, "next", t5, 0);
    Simple* t7 = new Simple(loc, new Assignment(loc, id, env_->no_type(), t6));

    t7->next(block);
    
    // Loop body
    While* t10 = new While(loc, t4, new Block(loc, 0, t7)); 
    return new Let(loc, t2, t10);
}

void Parser::module_feature(Feature* feature, String* scope) {
    // If the scope of the feature is fully-qualified, then use that to select
    // the module.  Otherwise, insert the feature into the current module. 
    Module::Ptr module = env_->module(scope);
    if (!module) {
        module = new Module(location(), env_, scope);
        env_->module(module);
    } 
    module->feature(feature);
    file_->feature(feature);
}

void Parser::implicit_import(Type* type) {
    // Adds an implicit import for 'type' to the parse tree, if necessary.
    if (!type->is_generic()) {
        implicit_import(type->scope());
    }
}

void Parser::implicit_import(String* scope) {
    // Adds a new import to the module/file if it hasn't been added already. 
    Import::Ptr import = module_->import(scope); 
    if (!scope->string().empty() && !import) {
        import = new Import(location(), scope, true);
        module_->import(import);
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

