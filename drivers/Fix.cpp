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

#include "Environment.hpp"
#include "Parser.hpp"
#include "ArgParser.hpp"
#include "Rewriter.hpp"
#include "SemanticAnalyzer.hpp"
#include <cstdlib>

Environment::Ptr env(new Environment());
ArgParser::Ptr argp;
bool by_name = false;
bool by_location = false;
bool by_offset = false;

std::string name;
std::string replacement;
size_t offset = 0;
size_t line = 0;
size_t column = 0;

void parse_option(std::string const& flag) {
    if ("include" == flag) {
        env->include(argp->required_arg());
    } else if ("name" == flag) {
        name = argp->required_arg(); 
        by_name = true;
    } else if ("location" == flag) {
        char* end = 0;
        line = strtol(flag.c_str(), &end, 0);
        if (*end != ':') {
            argp->error("Invalid location format; must be LINE:COL");
        }
        column = strtol(end+1, &end, 0);
        if (*end != '\0') {
            argp->error("Invalid location format; must be LINE:COL");
        } 
        by_location = true;
    } else if ("offset" == flag) {
        offset = strtol(flag.c_str(), 0, 0);
        by_offset = true;
    } else if ("replacement" == flag) {
        replacement = argp->required_arg();
    } else {
        argp->bad_option(flag); 
    }
}

void parse_short_option(std::string const& flag) {
    // Parses a short-form option ('-x')
    switch (flag[0]) {
    case 'i': parse_option("include"); break;
    case 'n': parse_option("name"); break;
    case 'l': parse_option("location"); break;
    case 'o': parse_option("offset"); break;
    case 'r': parse_option("replacement"); break;
    default: argp->bad_short_option(flag); break;
    }
}

void parse_options() {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argp->usage(
        "Usage: jgfix [-nlo] [OPTIONS] FILE...\n"
        "Refactoring tool for Jogo.\n\n"
        "   -i, --include DIR        Add the directory DIR to the search path.\n"
        "   -n, --name NAME          Rename all occurences of qualifed name NAME.\n"  
        "   -l, --location LINE:COL  Rename the token at line LINE and column COL.\n"
        "   -o, --offset OFFSET      Rename the token at file byte offset OFFSET.\n"
        "   -r, --replacement STR    String to replace all matched text with.\n");
      
    for (ArgToken tok = argp->next(); tok; tok = argp->next()) {
        // Convert abbreviated flags into the longer descriptive form (e.g.,
        // convert -p to --path)
        if (ArgToken::SHORT == tok.type()) {
            parse_short_option(tok.value());
        } else if (ArgToken::LONG == tok.type()) {
            parse_option(tok.value());
        } else {
            env->input(tok.value());
        }
    } 

    if (!by_name && !by_offset && !by_location) {
        argp->error("No refactor mode.");   
    }
}

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    argp = new ArgParser(env, argc, argv);
    env->include(".");
    env->verbose(true);
    parse_options(); 
    //env->workspace_load();

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    Parser::Ptr parser(new Parser(env));
    SemanticAnalyzer::Ptr semant(new SemanticAnalyzer(env));
    Rewriter::Ptr rewriter(new Rewriter(env));
    rewriter->replacement(replacement);
    rewriter->name(name);
    rewriter->rewrite();
   // RefactorPass::Ptr refactor(new RefactorPass(env));

    return 0;
}
