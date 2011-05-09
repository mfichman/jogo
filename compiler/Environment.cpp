/*
 * Copyright (c) 2010 Matt Fichman
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
#include <cassert>
#include <stack>

Name* Environment::name(const std::string& str) {
    // Returns a name if it exists, otherwise, a new one is created.

	std::map<std::string, Name::Ptr>::iterator i = name_.find(str);
	if (i == name_.end()) {
		Name* name = new Name(str);
		name_.insert(std::make_pair(str, name));	
		return name;
	} else {
		return i->second;
	}
}

Module* Environment::module(Import* import) {
    // Creates a new module using the path specified by the given import,
    // or returns an existing module if it exists.
        
    return module(import->file_name());
}

Module* Environment::module(const std::string& file_name) {
    // Look up a module by file_name path
    Module* module = root();

    std::string name;
    for (size_t i = 0; i < file_name.size(); i++) {
        if (!isalnum(file_name[i])) {
            Module* next = module->module(Environment::name(name));
            if (!next) {
                next = new Module(Location(), Environment::name(name));
                module->feature(next);
            } 
            module = next;
            if (file_name[i] != '/') {
                return module;        
            }
        } else {
            name += file_name[i];
        }
    }

    return module;
}

