/*
 * Copyright (c) 2011 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
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

#include "Jogo.hpp"
#include "Environment.hpp"
#include <string>

/* Represents an argument */
class ArgToken {
public:
    enum Type { SHORT, LONG, ARG, END };

    ArgToken() : type_(END) {}
    ArgToken(Type const& type, std::string const& value) 
        : type_(type), value_(value) {
    }
    Type type() const { return type_; }
    std::string value() const { return value_; }
    operator bool() const { return type_ != END; }

private:
    Type type_;
    std::string value_;
};


/* Basic support for parsing command-line arguments */
class ArgParser : public Object {
public:
    ArgParser(Environment* env, int argc, char** argv);
    ArgToken next();
    std::string required_arg();
    void usage(std::string const& msg);
    void print_usage();
    void print_version();
    void bad_option(std::string const& opt);
    void bad_short_option(std::string const& opt);
    void error(std::string const& msg);
#ifdef WINDOWS
    static int report_hook(int, char* message, int*);
#endif
    typedef Pointer<ArgParser> Ptr;

private:
    Environment::Ptr env_;
    int argindex_;
    int argc_;
    char** argv_;
    std::string usage_;
};
