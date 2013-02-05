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

#include "Lexer.hpp"
#include <cctype>

Lexer::Lexer(Environment* env) :
    env_(env),
    err_(Stream::sterr()),
    input_(0),
    front_(0),
    char_(0),
    line_(0),
    column_(0),
    string_level_(0),
    ignore_newline_(false),
    expect_comment_(false) {

    for(int i = 0; i < LEXER_LOOKAHEAD; i++) {
        token_[i] = Token();
    }

    keyword_["and"] = Token::AND;
    keyword_["else"] = Token::ELSE;
    keyword_["for"] = Token::FOR;
    keyword_["fork"] = Token::FORK;
    keyword_["func"] = Token::FUNC;
    keyword_["if"] = Token::IF;
    keyword_["in"] = Token::IN;
    keyword_["immutable"] = Token::IMMUTABLE;
    keyword_["embedded"] = Token::EMBEDDED;
    keyword_["import"] = Token::IMPORT;
    keyword_["let"] = Token::LET;
    keyword_["match"] = Token::MATCH;
    keyword_["native"] = Token::NATIVE;
    keyword_["not"] = Token::NOT;
    keyword_["or"] = Token::OR;
    keyword_["private"] = Token::PRIVATE;
    keyword_["public"] = Token::PUBLIC;
    keyword_["static"] = Token::STATIC;
    keyword_["ret"] = Token::RETURN;
    keyword_["until"] = Token::UNTIL;
    keyword_["weak"] = Token::WEAK; 
    keyword_["with"] = Token::WITH;
    keyword_["while"] = Token::WHILE;
    keyword_["xor"] = Token::XORB;
    keyword_["yield"] = Token::YIELD;
    keyword_["is"] = Token::IS;
}

Lexer::~Lexer() {
    if (input_ && input_ != &std::cin) {
        delete input_;
    }
}


void Lexer::input(File* file) {
    location_.file = file;
    if (input_ && input_ != &std::cin) {
        delete input_;
    }
    input_ = new std::ifstream(file->path()->string().c_str());
	if (!*input_) {
		err_ << "Could not open " << file->path()->string() << "\n";
		env_->error();
		return;
	}
    init();
}

void Lexer::input() {
    input_ = &std::cin;
    init();
}

void Lexer::init() {
    // Initializes the lexer and reads the first few tokens.
    location_.first_line = 0;
    location_.first_column = 0;
    location_.first_offset = 0;
    location_.last_line = 0;
    location_.last_column = 0;
    location_.last_offset = 0;
    line_ = 1;
    column_ = -1;
    char_ = 0;
    front_ = 0;
    string_level_ = 0;
    ignore_newline_ = true;
    expect_comment_ = false;
    read();
    for (int i = 0; i < LEXER_LOOKAHEAD; i++) {
        next();
    }
}

const Token& Lexer::token(int index) const {
    // Returns the token at the given index, with '0' being the oldest token.
    // This function will not read ahead more than 4 tokens.
    if (index < 0) {
        index += LEXER_LOOKAHEAD;
    }
    assert("Too much lookahead" && index < LEXER_LOOKAHEAD);
    assert("Invalid index" && index >= 0);
    int i = (front_+index)%LEXER_LOOKAHEAD;
    return token_[i];
}

const std::string& Lexer::value(int index) const {
    // Returns the value corresonding to the token at the given index.
    return token(index).value();
}

const Location& Lexer::loc(int index) const {
    return token(index).location();
}

void Lexer::next() {
    // Top-level lexer routine.  This function reads in characters one at a
    // time and attempts to match them to tokens. 
    token(Token::NONE);

    while (token() == Token::NONE) {
        if (char_ == '\n') {
            location_.first_column = 1;
            location_.first_line = line_+1;
        } else {
            location_.first_column = column_+1;   
            location_.first_line = line_;
        }
        location_.first_offset = offset_;
        token_[front_].location(location_);
        value("");

        if (char_ == '\n' && input_ == &std::cin) {
            token(Token::END);
        } else if (char_ == '\n' && !ignore_newline_) {
            read();
            value("");
            token(Token::SEPARATOR);
            ignore_newline_ = true;
        } else if (char_ == '/' && token(-1).is_operator()
            && token(-1) != Token::RIGHT_PARENS) {
            regex();
            expect_comment_ = false; 
        } else if (isdigit(char_)) {
            number_or_dot();
            expect_comment_ = false;
        } else if (isupper(char_)) {
            type_or_const();
            expect_comment_ = false;
        } else if (islower(char_) || char_ == '_') {
            ident_or_keyword();
            expect_comment_ = false;
        } else if (char_ == '@') {
            special();
            expect_comment_ = false;
        } else if (char_ == '"') {
            string();
            expect_comment_ = false;
        } else if (char_ == '\'') {
            string_or_char(); 
            expect_comment_ = false;
        } else if (char_ == '#') {
            comment();
        } else if (ispunct(char_)) {
            operator_or_typevar();
        } else if (isspace(char_)) {
            read();
            token(Token::NONE);
        } else if (char_ == EOF) {
            token(Token::END);
        } else {
            token(Token::ERROR);
            read();
        }
        location_.last_column = column_;
        location_.last_line = line_;
        location_.last_offset = offset_;
        token_[front_].location(location_);
    }
    //Stream::stout() << token(0) << "\n"; // << ", '" << value(-1) << "'\n";
    //Stream::stout()->flush();
    front_ = (front_+1)%LEXER_LOOKAHEAD;
}

void Lexer::comment() {
    // Reads a comment.  If the comment is expected, then the comment will
    // generate a Token::COMMENT token; otherwise, it will generate a
    // Token::NONE token, causing the next real token to be read.
    while (char_ != '\n' && char_ != EOF) {
        read();
    }
    value(value().substr(1, value().length()-1));
    std::string tmp = value();
    tmp.erase(0, value().find_first_not_of(' '));
    if (expect_comment_) {
        token(Token::COMMENT);
    } else {
        token(Token::NONE);
    }
    if (ignore_newline_) {
        read();
    }
    value(tmp);
}

void Lexer::regex() {
    // Reads a full regex, e.g., /.*/, with escape characters.
    read();
    while (char_ != '/') {
        if (char_ == '\\') {
            read();
            token_[front_].value() += '\\';
        } else {
            read();
        }
    }
    read(); 
    token(Token::REGEX);
    value(value().substr(1, value().length()-2));
    ignore_newline_ = false;
}

void Lexer::number_or_dot() {
    // Reads a number, e.g. \d*.\d+; if the value is a single '.' then this
    // reads in a dot
    while (isdigit(char_)) {
        read();
    } 
    if (char_ == '.') {
        // Read in the decimal point and everything after it.
        read();
		if (!isdigit(char_) && value()[0] == '.') {
		    return token(Token::DOT);
		}
        if (islower(char_) && !value().empty()) {
            // Put the '.' back, b/c we have a situation like 7.sin that must
            // be resolved. 
            input_->putback(char_); 
            char_ = '.';
            token_[front_].value(value().substr(0, value().length()-1));
            ignore_newline_ = false;
            token(Token::INTEGER);
            return;
        }
        while (isdigit(char_)) {
            read();
        }
        if (value() == ".") {
            ignore_newline_ = true;
            token(Token::DOT);
        } else {
            ignore_newline_ = false;
            token(Token::FLOAT);
        }
    } else if (char_ == 'x' && value() == "0") {
        // Read in a hexadecimal integer
        read();
        while (ishexnumber(char_)) {
            read();
        }
        ignore_newline_ = false;
        token(Token::INTEGER);
    } else {
        ignore_newline_ = false;
        token(Token::INTEGER);
    }
}

void Lexer::type_or_const() {
    // Reads in an identifier of the form [[:upper:]][[:alnum:]_]+
    bool is_const = true;
    while (isalnum(char_) || char_ == '_') {
        if (islower(char_)) {
            is_const = false;
        }
        read();
    }
    ignore_newline_ = false;
    expect_comment_ = false;
    if (is_const) {
        token(Token::CONSTANT);
    } else {
        token(Token::TYPE);
    }
}

void Lexer::ident_or_keyword() {
    // Reads in an identifier, of the form [[:lower:]][[:alnum:]_]+
    while (isalnum(char_) || char_ == '_') {
        read();
    }
    ignore_newline_ = false;    

    if (char_ == '?' || char_ == '!' || char_ == '=') {
        read();
    }

    std::map<std::string, Token::Type>::iterator i = keyword_.find(value());
    if (i != keyword_.end()) {
        token(i->second);
    } else if (value() == "true" || value() == "false") {
        token(Token::BOOL);
    } else if (value() == "nil") {
        token(Token::NIL);
    } else if (value() == "eof") {
        token(Token::EOF_LITERAL);
    } else {
        token(Token::IDENTIFIER); 
    }
}

void Lexer::special() {
    // Reads an operator handler, i.e., @add or @init
    read();
    while (isalnum(char_) || char_ == '_') {
        read();
    }
    ignore_newline_ = false;
    token(Token::OPERATOR);
}

void Lexer::operator_or_typevar() {
    // Reads in a single or double-character operator.
    
    ignore_newline_ = true;

    switch (char_) {
    case ',': read(); token(Token::COMMA); break;
    case '.': number_or_dot(); break;
    case '/': read(); token(Token::DIV); break;
    case '%': read(); token(Token::MOD); break;
    case '^': read(); token(Token::XORB); break;
    case '|': read(); token(Token::ORB); break;
    case '&': read(); token(Token::ANDB); break;
    case '*': 
        read(); 
        if (char_ == '*') {
            read();
            token(Token::POW);
        } else {
            token(Token::MUL);
        } 
        break;
    case '[': read(); token(Token::LEFT_BRACKET); break;
    case ']': 
        ignore_newline_ = false;
        read(); 
        token(Token::RIGHT_BRACKET); 
        break;
    case '(': read(); token(Token::LEFT_PARENS); break;
    case ')': 
        ignore_newline_ = false;
        read(); 
        token(Token::RIGHT_PARENS); 
        break;
    case '~': read(); token(Token::COMPL); break;
    case '{': 
        read(); 
        token(Token::LEFT_BRACE);
        expect_comment_ = true;
        break;
    case '}': 
        expect_comment_ = false;
        if (string_level_ > 0) {
            string(); 
        } else {
            read(); 
            token(Token::RIGHT_BRACE);
        }
        ignore_newline_ = false;
        break;
    case '>':
        read();
        if (char_ == '=') {
            read();
            token(Token::GREATER_OR_EQ);
        } else if (char_ == '>') {
            read();
            token(Token::RIGHT_SHIFT);
        } else {
            token(Token::GREATER);
        }
        break;
    case '<':
        read();
        if (char_ == '=') {
            read(); 
            token(Token::LESS_OR_EQ);
        } else if (char_ == '<') {  
            read();
            token(Token::LEFT_SHIFT);
        } else {
            token(Token::LESS);
        }
        break;
    case '=':
        read();
        if (char_ == '=') {
            read();
            token(Token::EQUAL);
        } else if (char_ == '~') {
            read();
            token(Token::MATCH_OP);
        } else {
            token(Token::ASSIGN);
        }
        break;
    case '!':
        read();
        if (char_ == '=') {
            read();
            token(Token::NOT_EQUAL);
        } else {
            token(Token::ERROR);
        }       
        break;
    case '+':
        read();
        if (char_ == '+') {
            read();     
            token(Token::INCREMENT);
        } else {
            token(Token::ADD);
        }
        break;
    case '-':
        read();
        if (char_ == '-') {
            read(); 
            token(Token::DECREMENT);
        } else {
            token(Token::SUB);
        }
        break;
    case ':':
        read();
        /*if (islower(char_)) {
            while (islower(char_)) {
                read();
            }
            token(Token::TYPEVAR);
            ignore_newline_ = false;
        } else*/ if (char_ == ':') {
            read();
            token(Token::SCOPE);
        } else {
            token(Token::COLON);
        } 
        break;
    default:
        ignore_newline_ = false;
        read();
        token(Token::ERROR);
        break;
    }
}

void Lexer::string_or_char() {
    // Reads a string or a character, if the suffix 'c' is present.
    read();
    while (char_ != '\'') {
        if (char_ == '\\') {
            read();
        }
        read();
    }
    ignore_newline_ = false;
    read();
    if (char_ == 'c') {
        read();
        token(Token::CHAR);
        value(value().substr(1, value().length()-2));
    } else {
        token(Token::STRING);
        value(value().substr(1, value().length()-2));
    }
}

void Lexer::string() {
    // Reads a double-quoted string, which may end in "#{" if there is a
    // string interpolation.
    if (char_ == '}') {
        string_level_--;
    }

    read();
    while (char_ != '"' && char_ != EOF) {
        if (char_ == '#') {
            read();
            if (char_ == '{') {
                read();
                ignore_newline_ = true;
                token(Token::STRING_BEGIN);
                string_level_++;
                value(value().substr(1, value().length()-3));
                return;
            }
        } else if (char_ == '\\') {
            read();
        } 
        read();
    }
    ignore_newline_ = false;
    if (value()[0] == '}') {
        ignore_newline_ = false;
        token(Token::STRING_END);
    } else {
        token(Token::STRING);
    }
    read();
    value(value().substr(1, value().length()-2));
}

void Lexer::read() {
    // Reads one character of input from the stream.  Updates the line number
    // and column number.
    if (char_ == '\n') {
        line_++;
        column_ = 0;
    } else {
        column_++;
    }   
    offset_++;
    if (char_) {
        token_[front_].value() += char_;
    }
    char_ = input_->get(); 
}

Stream::Ptr operator<<(Stream::Ptr out, const Token& token) {
    switch (token) {
    case Token::REGEX: return out << "regex";
    case Token::CONSTANT: return out << "constant";
    case Token::OR: return out << "'or'"; 
    case Token::AND: return out << "'and'";
    case Token::XORB: return out << "'xor'";
    case Token::ANDB: return out << "'&'";
    case Token::ORB: return out << "'|'";
    case Token::ASSIGN: return out << "'='";
    case Token::EQUAL: return out << "'=='";
    case Token::NIL: return out << "'nil'";
    case Token::NOT_EQUAL: return out << "'!='";
    case Token::LESS: return out << "'<'";
    case Token::GREATER: return out << "'>'";
    case Token::LESS_OR_EQ: return out << "'<='";
    case Token::GREATER_OR_EQ: return out << "'>='";
    case Token::COMPARE: return out << "'<>'";
    case Token::LEFT_SHIFT: return out << "'<<'";
    case Token::RIGHT_SHIFT: return out << "'>>'";
    case Token::BOOL: return out << "'" << token.value() << "'"; 
    case Token::IS: return out << "'is'";
    case Token::EOF_LITERAL: return out << "'eof'";
    case Token::ADD: return out << "'+'";
    case Token::SUB: return out << "'-'";
    case Token::MUL: return out << "'*'";
    case Token::DIV: return out << "'/'";
    case Token::MOD: return out << "'%'";
    case Token::NOT: return out << "'not'";
    case Token::POW: return out << "'^'";
    case Token::INCREMENT: return out << "'++'";
    case Token::DECREMENT: return out << "'--'";
    case Token::IDENTIFIER: return out << "identifier";
    case Token::TYPE: return out << "type name";
    case Token::OPERATOR: return out << "'" << token.value() << "'";
    case Token::COMMENT: return out << "comment";
    case Token::TYPEVAR: return out << "typevar";
    case Token::STRING: return out << "string";
    case Token::PUBLIC: return out << "'public'";
    case Token::PRIVATE: return out << "'private'";
    case Token::STATIC: return out << "'static'";
    case Token::NATIVE: return out << "'native'";
    case Token::WEAK: return out << "'weak'";
    case Token::IMMUTABLE: return out << "'immutable'";
    case Token::EMBEDDED: return out << "'embedded'";
    case Token::IMPORT: return out << "'import'";
    case Token::FUNC: return out << "'func'";
    case Token::VOID: return out << "Void";
    case Token::SEPARATOR: return out << "end of line";
    case Token::BACKTICK: return out << "'`'";
    case Token::SEMICOLON: return out << "';'";
    case Token::MATCH: return out << "'match'";
    case Token::WITH: return out << "'with'";
    case Token::WHILE: return out << "'while'";
    case Token::ELSE: return out << "'else'";
    case Token::UNTIL: return out << "'until'";
    case Token::IF: return out << "'if'";
    case Token::DO: return out << "'do'";    
    case Token::FLOAT: return out << "float";
    case Token::INTEGER: return out << "integer";
    case Token::COMMA: return out << "','";  
    case Token::COLON: return out << "':'";
    case Token::LEFT_BRACKET: return out << "'['";
    case Token::RIGHT_BRACKET: return out << "']'";
    case Token::LEFT_PARENS: return out << "'('";
    case Token::RIGHT_PARENS: return out << "')'";
    case Token::LEFT_BRACE: return out << "'{'";
    case Token::RIGHT_BRACE: return out << "'}'";
    case Token::MATCH_OP: return out << "'=~'";
    case Token::COMPL: return out << "'~'";
    case Token::STRING_BEGIN: return out << "string";
    case Token::STRING_END: return out << "string";
    case Token::CHAR: return out << "'char'";
    case Token::RETURN: return out << "'return'";
    case Token::FOR: return out << "'for'";
    case Token::SCOPE: return out << "'::'";
    case Token::LET: return out << "'let'";
    case Token::IN: return out << "'in'";
    case Token::YIELD: return out << "'yield'";
    case Token::FORK: return out << "'fork'";
    case Token::ERROR: return out << "'" << token.value() << "'";
    case Token::END: return out << "end of file";
    case Token::NONE: return out << "'none'";
    case Token::DOT: return out << "'.'";
    }
    return out;
}

bool Token::is_operator() const {
    switch (*this) {
    case Token::AND:
    case Token::XORB:
    case Token::ANDB:
    case Token::ASSIGN:
    case Token::EQUAL:
    case Token::LESS:
    case Token::GREATER:
    case Token::LESS_OR_EQ:
    case Token::COMPARE:
    case Token::LEFT_SHIFT:
    case Token::RIGHT_SHIFT:
    case Token::IS:
    case Token::ADD:
    case Token::SUB:
    case Token::MUL:
    case Token::DIV:
    case Token::MOD:
    case Token::NOT:
    case Token::POW:
    case Token::INCREMENT:
    case Token::DECREMENT:
    case Token::LEFT_PARENS:
    case Token::RIGHT_PARENS:
    case Token::COMPL:
    case Token::IF:
    case Token::WHILE:
    case Token::ELSE:
        return true;
    default:
        return false;
    }
}
