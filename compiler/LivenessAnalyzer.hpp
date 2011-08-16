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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#pragma once

#include "Apollo.hpp"
#include "Environment.hpp"
#include "BasicBlock.hpp"
#include "Machine.hpp"
#include "Object.hpp"
#include <bitset>
#include <map>

#define APOLLO_MAX_REG 32

/* Computes liveness information for a function */
class LivenessAnalyzer : public Object {
public:
    LivenessAnalyzer(Machine* mach) : machine_(mach) {}
    bool live(const Instruction& inst, int temporary);

    typedef std::bitset<APOLLO_MAX_REG> Set;

    const Set& live_in(const Instruction& inst) {
        return in_[&inst];  
    } // Only here for printer
    const Set& live_out(const Instruction& inst) {
        return out_[&inst];
    }
    
    void operator()(Function* feature);
    typedef Pointer<LivenessAnalyzer> Ptr;

private:
    void operator()(BasicBlock* block); 

    Machine::Ptr machine_;
    Function::Ptr function_;
    bool finished_;
    bool entry_block_;
    std::map<const Instruction*, Set > in_;
    std::map<const Instruction*, Set > out_;
    int round_;
};
