/*
 * Copyright (c) 2013 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "ArgParser.hpp"
#include "Environment.hpp"
#include "WinDebugger.hpp"
#include <iostream>
#include <string>
#include <cctype>

Environment::Ptr env = new Environment();
ArgParser::Ptr argp;


#ifdef WINDOWS
Debugger::Ptr debugger = new WinDebugger();
#else
Debugger::Ptr debugger;
#endif

void parse_options() {
    // Parse command line options, and update the environment to match the 
    // specified options.
    argp->usage(
        "Usage: jgdebug [OPTIONS] exe\n"
        "Debugs a Jogo executable.\n\n"
        "--version          Print the debugger version number.\n");

    for (ArgToken tok = argp->next(); tok; tok = argp->next()) {
        if (ArgToken::SHORT == tok.type()) {
            argp->error("Invalid option");
        } else if (ArgToken::LONG == tok.type()) {
            argp->error("Invalid option");
        } else {
            env->input(tok.value());
        }
    } 
}

void spaces() {
    while (true) {
        int ch = std::cin.peek();
        if (std::cin.bad()) {
            exit(0);
        } else if (ch == '\n') {
            return;
        } else if (isspace(ch)) {
            std::cin.get();
            return;
        } else {
            return;
        }
    }
}

std::string arg() {
    // Reads in the command token 
    spaces();
    std::string cmd;
    while (true) {
        int ch = std::cin.peek();
        if (std::cin.bad()) {
            exit(0);
        } else if (!isspace(ch)) {
            cmd += ch;
            std::cin.get();
        } else {
            break;
        }
    }
    return cmd;
}

void discard() {
    while (true) {
        int ch = std::cin.peek();
        if (std::cin.bad()) {
            exit(0);
        } else if (ch == '\n') {
            return;
        } else {
            std::cin.get();
        }
    }
}

void wait() {
    switch (debugger->wait()) {
    case Debugger::NONE: assert(!"None"); break;
    case Debugger::BREAKPOINT:
        std::cout << "Breakpoint" << std::endl; 
        break;
    case Debugger::ACCESS_VIOLATION: 
        std::cout << "Access violation" << std::endl; 
        break;
    case Debugger::STEP: 
        std::cout << "Step" << std::endl;
        break;
    case Debugger::INTERRUPT:
        std::cout << "Interrupt" << std::endl;
        break;
    case Debugger::EXIT:
        std::cout << "Process exited" << std::endl;
        break;
    default: assert(!"Unknown event"); break;
    }
}

void run() {
    // Read in the arguments for 'run'
    std::vector<std::string> args;
    std::vector<char const*> argv;
    while (std::cin.good() && std::cin.peek() != '\n') {
        std::string next = arg();
        if (!next.empty()) {
            args.push_back(next);
            argv.push_back(args.back().c_str());
        }
    }
    debugger->exec(&argv.front(), argv.size());
    wait();    
}

void cont() {
    discard();
    debugger->cont();
    wait();
}

void breakpoint() {
}

void kill() {
}

void detach() {
}

void attach() {
}

int main(int argc, char** argv) {
    argp = new ArgParser(env, argc, argv);
    parse_options();
    argp->print_version();
    while (std::cin.good()) {
        std::cout << ">> ";
        std::string line;
        
        std::string cmd = arg(); 
        if (cmd.empty()) {
            // Pass: Do nothing
        } else if (cmd == "run") {
            run();
        } else if (cmd == "break") {
            breakpoint();
        } else if (cmd == "kill") {
            kill();
        } else if (cmd == "detach") {
            detach();
        } else if (cmd == "attach") {
            attach();
        } else if (cmd == "continue") {
            cont();
        } else if (cmd == "exit") {
            exit(0);
        } else if (std::cin.eof()) {
            break; // EOF
        } else {
            std::cout << "Unknown command: " << cmd << std::endl;
            discard();
        }
        if (std::cin.peek() == '\n') {
            std::cin.get();
        }
        
    }
    return 0;
}
