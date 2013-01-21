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
#include "SemanticAnalyzer.hpp"
#include "MarkdownGenerator.hpp"

Environment::Ptr env(new Environment());
ArgParser::Ptr argp;

void parse_option(std::string const& flag) {
    if ("output" == flag) {
        env->output(argp->required_arg());
    } else if ("include" == flag) {
        env->include(argp->required_arg());
    } else if ("format" == flag) {
        env->generator(argp->required_arg());
    } else if ("verbose" == flag) {
        env->verbose(true);
    } else {
        argp->bad_option(flag);
    }
}

void parse_short_option(std::string const& flag) {
    // Parses a short-form option ('-x')
    switch (flag[0]) {
    case 'o': parse_option("output"); break;
    case 'i': parse_option("include"); break;
    case 'h': parse_option("help"); break;
    case 'f': parse_option("format"); break;
    case 'v': parse_option("verbose"); break;
    default: argp->bad_short_option(flag); break;
    }
}

void parse_options() {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argp->usage(
        "Usage: jgdoc [OPTIONS] FILE...\n"
        "Generates documentation from comments found in Jogo source files.\n\n"
        "   -o, --output DIR     Write output to DIR.\n"
        "   -i, --include DIR    Add the directory DIR to the search path.\n"
        "   -h, --help           Print this help message.\n"
        "   -f, --format FORMAT  Set output format to FORMAT.\n"
        "   -v, --verbose        Print extra information.\n"
        "   --version            Print the compiler version number.\n");

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

    std::string gen = env->generator();
    if (gen != "Markdown") {
        argp->error("Invalid output format (options: Markdown");
    }
}

int main(int argc, char** argv) {
    // Create a new empty environment to store the AST and symbols tables, and
    // set the options for compilation.
    argp = new ArgParser(env, argc, argv);
    env->output("doc");
    env->generator("Markdown");

    parse_options(); 
    File::mkdir(env->output());
    env->workspace_load();

    // Run the compiler.  Output to a temporary file if the compiler will
    // continue on to another stage; otherwise, output the file directly.
    Parser::Ptr parser(new Parser(env));
    SemanticAnalyzer::Ptr semant(new SemanticAnalyzer(env));
    MarkdownGenerator::Ptr md(new MarkdownGenerator(env));

    return 0;
}
