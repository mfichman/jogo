#include <lexer.hpp>
#include <token.hpp>
#include <parser.hpp>
#include <iostream>


int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Need an input file" << std::endl;
        return 1;
    }

    Input::Ptr input(new Input(argv[1]));
    Lexer::Ptr lexer(new Lexer(input));
    Parser::Ptr parser(new Parser(lexer));

    return 0;
}
