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

int yyparse(Parser* parser, void* scanner);
int yylex_init(void**);
int yylex_destroy(void*);
void yyset_extra(Parser*, void*);

Parser::Parser(Environment* env) :
	env_(env),
    err_(Stream::sterr()),
    lexer_(new Lexer(env)),
    error_(0) {

    input("Primitives");
    input("String");
    input("Coroutine");

    for (int i = 0; i < env->inputs(); i++) {
        is_input_file_ = true;
        input(env->input(i));
    }
}

void Parser::input(const std::string& import) {
    // Reads in a file or directory after searching through the list of include
    // paths.  The includes paths are searched in the order they are specified,
    // so if duplicate files are present in two include directories, then only
    // the first will be parsed.

    std::string file = Import::file_name(import); 
    std::vector<std::string> tests;

    for (int i = 0; i < env_->includes(); i++) {
        const std::string& prefix = env_->include(i);
        if (File::ext(file) != ".ap") {
            if (File::is_reg(prefix + "/" + file + ".ap")) {
                std::vector<std::string>().swap(tests);
                Parser::file(prefix, file + ".ap");
                return;
            }
        } else {
            if (File::is_reg(prefix + "/" + file)) {
                std::vector<std::string>().swap(tests);
                Parser::file(prefix, file);
                return; 
            } 
        }
        if (File::is_dir(prefix + "/" + file)) {
            std::vector<std::string>().swap(tests);
            Parser::dir(prefix, file);
            return;
        }   
        tests.push_back(prefix + "/" + file + ".ap");
        tests.push_back(prefix + "/" + file);
    }
    env_->error("Could not find " + import);
    err_ << "Module '" << import << "' not found:\n";
    error();
    for (int i = 0; i < tests.size(); i++) {
        err_ << "    no file '" << tests[i] << "'\n";
        error();
    }
}

void Parser::file(const std::string& prefix, const std::string& file) {
    // Begin parsing a module file if it doesn't already exist.


    // The module can be loaded from the dir components of the file name
    String* scope = env_->name(Import::scope_name(file));
    module_ = env_->module(scope);
    if (!module_) {
        module_ = new Module(Location(), scope, env_);
        env_->module(module_);
    }

    // Create a file object for this file if it hasn't been parsed yet.
    std::string actual_file = (prefix == ".") ? file : prefix + "/" + file;
    String* fs = env_->name(file);
    file_ = env_->file(fs);
    if (file_) {
        return;
    } else {
        file_ = new File(fs, env_->name(actual_file), module_, env_);
        file_->is_input_file(is_input_file_);
        env_->file(file_);
    }
    is_input_file_ = false;

    // Begin parsing
    lexer_->input(file_);
    error_ = 0;
    module();

    // Now parse other modules that depend on the unit that was added
    for (Feature* f = file_->features(); f; f = f->next()) {
        if (Import* import = dynamic_cast<Import*>(f)) {
            input(import->scope()->string());
        }
    }
}

void Parser::dir(const std::string& prefix, const std::string& dir) {
    // Parses all files in the directory specified by 'dir'.  If 'dir' is not
    // a directory, then no files are parsed.

    for (File::Iterator i(prefix + "/" + dir); i; ++i) {
        std::string name = *i;
        const std::string ext = ".ap";
        if (name.length() <= ext.length()) {
            continue;
        }
        if (!name.compare(name.length() - ext.length(), ext.length(), ext)) {
            file(prefix, dir + "/" + name);
        } 
    }
}

Module* Parser::module() {
    // Parses a module, which can consist of classes, functions, and import
    // statements.
    switch (token()) {
    case Token::IDENTIFIER:
        module_->feature(function());
        break;
    case Token::TYPE:
        module_->feature(clazz()); 
        break;
    case Token::IMPORT:
        module_->feature(import());
        break;
    default:
        err_ << loc() << "Unexpected '" << token() << "'\n";
        error();
        next();
        break;
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
        err_ << loc() << "Expected '" << token << "', got '";
        err_ << lexer_->value() << "'\n";
        error();
        return false;
    }
}

Class* Parser::clazz() {
    // This function parses a class or interface definition, containing methods
    // and attributes.

    // Read in the name of the class, and any generic parameters
    String::Ptr name = scope(); 
    Generic::Ptr generics;
    if (token() == Token::LEFT_BRACKET) {
        next();
        while (token() == Token::TYPEVAR) {
            String::Ptr name = env_->name(lexer_->value());
            Type::Ptr type = new Type(loc(), name, 0, file_, env_);
            generics = append(generics, new Generic(type));
            next();
            if (token() == Token::COMMA) {
                next();
            } else {
                break;
            }
        }
        expect(Token::RIGHT_BRACKET);
    }
    std::string qn = module_->name()->string();
    if (qn.empty()) {
        qn = name->string();
    } else {
        qn += "::" + name->string();
    }
    
    // Parse the type list
    Type::Ptr mixins;
    while (token() == Token::TYPE) {
        mixins = append(mixins, type());
        if (token() == Token::COMMA) {
            next();
        } else {
            break;
        }
    }
    expect(Token::LEFT_BRACE);

    // Parse the comment, if present
    String::Ptr comment = Parser::comment(); 
    Feature::Ptr members = Parser::features(); 
    Type::Ptr type = new Type(loc(), env_->name(qn), generics, file_, env_);
    return new Class(loc(), type, mixins, comment, members);
}

Feature* Parser::features() {
    // Parses the list of class members, functions, and attributes
    Feature::Ptr members;
    while (token() != Token::RIGHT_BRACE) {
        members = append(members, feature());
        if (error_) {
            while (token() != Token::SEPARATOR) {
                if (token() == Token::RIGHT_BRACE) {
                    break;
                }
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
    return members; 
}

Feature* Parser::feature() {
    // Parses an attribute or function.
    
    return 0;
}

Function* Parser::function() {
    // Parses a free function, which does not belong to any class.  A function
    // has the following syntax: name([id type]*) flags? type? block?
    String* id = env_->name("");
    if (token() == Token::IDENTIFIER) {
        id = identifier();
    } else if (token() == Token::FUNC) {
        id = env_->name("@call"); // Closure
    }
    expect(Token::LEFT_PARENS);

    // Now parse the formal list of arguments, e.g., "a Int, b String"
    Formal::Ptr formals;
    while (token() == Token::IDENTIFIER) {
        String* id = identifier();
        Type* type = env_->any_type();
        if (token() == Token::TYPE || token() == Token::TYPEVAR) { 
            type = Parser::type();
        }
        formals = append(formals, new Formal(loc(), id, type));
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
    } else if (token() != Token::SEPARATOR) {
        err_ << loc() << "Parse error\n";
        error();
    }
    return new Function(loc(), id, formals, flags, ret, block);
}

Type* Parser::type() {
    // Parses a type name, including generics.  A type has the following 
    // syntax: id [type (, type)+]?
    String* qn = scope();

    // Now read in any generic type parameters for this type.
    Generic::Ptr generics;
    if (token() == Token::LEFT_BRACKET) {
        next();
        while (true) {
            if (token() == Token::TYPE) {
                generics = append(generics, new Generic(type()));
            } else if (token() == Token::TYPEVAR) {
                String::Ptr name = env_->name(lexer_->value());
                Type::Ptr type = new Type(loc(), name, 0, file_, env_);
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
    if (qn->string().empty()) {
        return env_->no_type();
    } else {
        return new Type(loc(), qn, generics, file_, env_); 
    }
}

String* Parser::scope() {
    // Parses a scope name, e.g.: id (:: id)?  If the token stream is invalid,
    // then the name returned is the empty string.
    std::string scope;
    while (token() == Token::TYPE) {
        scope += lexer_->value();
        next();
        if (token() == Token::SCOPE) {
            scope += "::";
            next();
        } else {
            break;
        }
    }

    if (scope.empty()) {
        err_ << loc() << "Expected a type name, not '" << token() << "'\n";
        error();
    }
    return env_->name(scope);
}

Block* Parser::block() {
    // Parses a block, e.g. { stmt* }.  If a parse error occurs, the routine
    // will attempt to find the next '}' and bail out, returning as much code
    // as it can.
    expect(Token::LEFT_BRACE);
    String::Ptr comment = Parser::comment();
    Statement::Ptr statements;
    while (token() != Token::RIGHT_BRACE) {
        statements = append(statements, statement());
        if (error_) {
            while (token() != Token::SEPARATOR) {
                if (token() == Token::RIGHT_BRACE) {
                    break;
                }
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
    return new Block(loc(), comment, statements);
}

String* Parser::comment() {
    // Parses a block comment, which can be attached to functions/classes
    std::string text;
    while (token() == Token::COMMENT) {
        std::string line = lexer_->value();
        if (line.empty()) {
            text += "\n\n" + line;
        } else {
            text += " " + line;
        }
    }
    return new String(text);
}

Statement* Parser::statement() {
    switch(token().type()) {
    case Token::RETURN:
        next();
        if (token() == Token::SEPARATOR) {
            return new Return(loc(), new Empty(loc()));
        } else {
            return new Return(loc(), expression());
        }
        break;
    default:
        return new Simple(loc(), expression());
    }
}

Import* Parser::import() {

    return 0;
}

String* Parser::identifier() {
    // Parses an identifier, i.e., [a-z][A-Z_0-9]* (leading lowercase letter)
    if (token() == Token::IDENTIFIER) {
        String* id = env_->name(lexer_->value());
        next();
        return id;
    } else {
        err_ << loc() << "Expected an identifier";
        error();
        String* id = env_->name("");
        return id;
    }
}

Feature::Flags Parser::flags() {

    return 0;
}



Expression* Parser::expression() {
    return logical_or();
}

Expression* Parser::assignment() {
    // Parses an assignment, either a = expr or a type = expr
    return 0;
}

Expression* Parser::logical_or() {
    // Parse a logical or, i.e., an expression of the form: expr (or expr)+
    Expression* expr = logical_and();    
    while (token() == Token::OR) {
        next();
        expr = op(expr, "or", logical_and());
    }
    return expr;
}

Expression* Parser::logical_and() {
    // Parse a logical and, i.e., an expression of the form: expr (and expr)+
    Expression* expr = bitwise_or();
    while (token() == Token::AND) {
        next();
        expr = op(expr, "and", bitwise_or());
    }
    return expr;
}

Expression* Parser::bitwise_or() {
    // Parse a bitwise or, i.e., an expression of the form: expr (| expr)+
    Expression* expr = bitwise_xor();
    while (token() == Token::ORB) {
        next();
        expr = op(expr, "@bitor", bitwise_xor());
    }
    return expr;
}

Expression* Parser::bitwise_xor() {
    // Parse a bitwise xor, i.e., an expression of the form: expr (xor expr)+
    Expression* expr = bitwise_and();
    while (token() == Token::XORB) {
        next();
        expr = op(expr, "@bitxor", bitwise_and());
    }
    return expr;
}

Expression* Parser::bitwise_and() {
    // Parse a bitwise and, i.e., an expression of the form: expr (& expr)+
    Expression* expr = equality();
    while (token() == Token::ANDB) {
        next();
        expr = op(expr, "@bitand", equality());
    }
    return expr;
}

Expression* Parser::equality() {
    // Parse an equality, i.e., an expression of the form: expr (== expr)+
    Expression* expr = relational();
    while (token() == Token::EQUAL) {
        next();
        expr = op(expr, "@equal", relational());
    }
    return expr;
}

Expression* Parser::relational() {
    // Parse a relational expr, i.e., an expression of the form: expr (& expr)+
    Expression* expr = shift();
    while (true) {
        switch (token().type()) {
        case Token::GREATER_OR_EQ: {
            next();
            expr = op("not", op(expr, "@less", shift()));
            break;
        } 
        case Token::GREATER: {
            next();
            Expression::Ptr right = shift();
            Expression::Ptr less = op(expr, "@less", right);
            Expression::Ptr equal = op(expr, "@equal", right);
            Expression::Ptr child = op(less, "or", equal);
            expr = op("not", child); 
            break;
        }
        case Token::LESS: {
            next();
            expr = op(expr, "@less", shift()); 
            break;
        }
        case Token::LESS_OR_EQ: {
            next();
            Expression::Ptr right = shift();
            Expression::Ptr less = op(expr, "@less", right);
            Expression::Ptr equal = op(expr, "@equal", right);
            expr = op(less, "or", equal);
        }
        default: return expr;
        }
    }
    return expr;
}

Expression* Parser::shift() {
    // Parse a shift, i.e., an expression of the form: expr ( << expr)+
    Expression* expr = addition();
    while (true) {
        switch(token().type()) {
        case Token::LEFT_SHIFT: 
            next();
            expr = op(expr, "@shift", addition()); 
            break;
        case Token::RIGHT_SHIFT: 
            next(); 
            expr = op(expr, "@unshift", addition()); 
            break;
        default: 
            return expr;
        }
    }    
    return expr;
}

Expression* Parser::addition() {
    // Parse an add/sub, i.e., an expression of the form: expr (+ expr)+
    Expression* expr = mult();
    while (true) {
        switch(token().type()) {
        case Token::ADD: next(); expr = op(expr, "@add", mult()); break;
        case Token::SUB: next(); expr = op(expr, "@sub", mult()); break;
        default: return expr;
        }
    }    
    return expr;
}

Expression* Parser::mult() {
    // Parse an add/sub, i.e., an expression of the form: expr (+ expr)+
    Expression* expr = unary();
    while (true) {
        switch(token().type()) {
        case Token::MUL: next(); expr = op(expr, "@mul", unary()); break;
        case Token::DIV: next(); expr = op(expr, "@div", unary()); break;
        case Token::MOD: next(); expr = op(expr, "@mod", unary()); break;
        default: return expr;
        }
    }    
    return expr;
}

Expression* Parser::unary() {
    // Unary boolean not and logical complement
    switch (token().type()) {
    case Token::NOT: next(); return op("not", unary());
    case Token::COMPL: next(); return op("@compl", unary());
    case Token::SUB: next(); return op("@neg", unary());
    default: return call();
    }
    return call();
}

Expression* Parser::call() {
    // Parses a call expression, i.e.: expr ( arg? (, arg)* ) +
    Expression* expr = member();
    while (token() == Token::LEFT_PARENS) {
        next();
    
        // Read in the arguments to the function.
        Expression::Ptr args;
        while (token() != Token::RIGHT_PARENS) {
            args = append(args, expression());
            if (token() == Token::COMMA) {
                next();
            } else {
                break;
            }
        }
        expect(Token::RIGHT_PARENS);
        expr = new Call(loc(), file_, expr, args);  
    }
    return expr;
}

Expression* Parser::member() {
    // Parses the member operator, i.e., '.', such as: expr [. ident]?
    Expression* expr = literal();
    while (token() == Token::DOT) {
        next();
        expr = new Member(loc(), expr, identifier());
    }
    return expr;
}

Expression* Parser::literal() {
    // Parses a literal expression, variable, or parenthesized expression
    Expression* expr;
    if (token() == Token::FLOAT) {
        expr = new FloatLiteral(loc(), env_->integer(lexer_->value()));
    } else if (token() == Token::INTEGER) {
        expr = new IntegerLiteral(loc(), env_->integer(lexer_->value()));
    } else if (token() == Token::STRING) {
        expr = new StringLiteral(loc(), env_->string(lexer_->value()));
    } else if (token() == Token::IDENTIFIER) {
        expr = new Identifier(loc(), env_->name(lexer_->value()));
    } else if (token() == Token::LEFT_PARENS) {
        expr = expression();
        expect(Token::RIGHT_PARENS);
        return expr;  
    } else {
        err_ << loc() << "Expected a literal value or identifier";
        error();
        return new Empty(loc());
    }
    next();
    return expr;
}

/*
Expression* Parser::increment() {
    // Increment or decrement expression, i.e., (++|--)? expr.  This can only
    // be called on an identifier.
    String::Ptr op;
    if (token() == Token::INCREMENT) {
        op = env_->name("@add");
    } else if (token() == Token::DECREMENT) {
        op = env_->name("@sub");
    } else {
        return unary();
    }
    next();

    if (token() != Token::IDENTIFIER) {
        err_ << loc() << "Invalid expression\n";
        return new Empty(loc());
    }
    String::Ptr id = identifier();
    Expression::Ptr t1 = new IntegerLiteral(loc(), env_->integer("1"));    
    Expression::Ptr t2 = new Identifier(loc(), id);
    Expression::Ptr t3 = new Member(loc(), t2, env_->name("@sub"));
    Expression::Ptr t4 = new Call(loc(), file_, t3, t1);
    return new Assignment(loc(), id, 0, t4);
}
*/

Expression* Parser::op(Expression* a, const std::string& op, Expression* b) {
    // Binary expression: Note that eventually this may be replaced by an 
    // actual Operator node
    Member::Ptr mem = new Member(loc(), a, env_->name(op));
    return new Call(loc(), file_, mem, b);     
}

Expression* Parser::op(const std::string& op, Expression* expr) {
    // Unary expression
    return new Unary(loc(), env_->name(op), expr);
}
