#pragma once

#include <tr1/memory>
#include <fstream>

class Input {
public:
    typedef std::tr1::shared_ptr<Input> Ptr;

    /* Creates a new input source from the given file */
    Input(const std::string& file); 

    /* Returns true if the EOF character has been reached */
    bool eof() const;

    /* Gets and returns the next character from input */
    char top();

    /* Puts the given character on top of the input */
    void top(char c);

private:
    std::fstream in_;

};
