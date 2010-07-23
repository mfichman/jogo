#include <input.hpp>
#include <stdexcept>
#include <iostream>

Input::Input(const std::string& file) :
    in_(file.c_str(), std::ios::in) {


}

bool Input::eof() const {
    int c = const_cast<std::fstream&>(in_).peek();
    return c == EOF;
}

char Input::top() {
    char c;

    /* Unexpected end of input, throw an exception */
    if (in_.eof()) {
        throw std::runtime_error("Unexpected end of input");
    }

    /* File read error */
    if (in_.fail()) {
        throw std::runtime_error("Error reading input");
    }

    in_.get(c);

    return c;
}

void Input::top(char c) {
    in_.putback(c); 
}
