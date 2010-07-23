#pragma once

#include <tr1/memory>
#include <input.hpp>
#include <token.hpp>


class Lexer {
public:
    typedef std::tr1::shared_ptr<Lexer> Ptr;

    /* 
     * Creates a new lexer with the given input source.  The input source
     * provides characters to the lexer from which tokens can be generated.
     */
    Lexer(Input::Ptr input); 

    /* Returns the next token by reading characters from the input source. */
    Token token();

    /* Returns the value of the current token */
    const std::string& value();

    /* Returns the current line number */
    size_t line_number() const;

    /* Returns the current character number */
    size_t char_number() const;

private:
    Token token_string();
    Token token_word();
    Token token_type();
    Token token_number();
    Token token_comment();

    Input::Ptr input_;
    std::string value_;
    size_t line_number_;
    size_t char_number_; 
};
