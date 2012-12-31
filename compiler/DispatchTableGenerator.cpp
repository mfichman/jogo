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

#include "DispatchTableGenerator.hpp"
#include <cstdint>
#include <algorithm>

struct SortJumpBuckets {
    bool operator()(JumpBucket const& a, JumpBucket const& b) {
        return a.size() > b.size();
    }
};

uint64_t fnv_hash(uint64_t hash, String* str) {
    std::string const& name = str->string();
    for (int i = 0; i < name.length(); ++i) {
        hash = ((hash * 0x01000193) ^ name[i]);
    }
    return hash;
}


DispatchTableGenerator::DispatchTableGenerator(Class* clazz) : class_(clazz) {
    // Initially set the size of the hash table to the number of functions.
    // The size of the table will grow to prevent collisions.
    if (clazz->is_interface()) { return; }
    Class::Ptr obj = clazz->env()->object_type()->clazz();
    FunctionCounter fc(clazz);
    fc.operator()(clazz);
    if (obj != clazz) {
        fc.operator()(obj);
    }

    // Step 1: Place all keys into buckets using a simple hash.  There will
    // be collisions, but they will be resolved in steps 2-3.
    int n = std::max(10, fc.count() * 2);
    bucket_.resize(n);
    operator()(clazz);
    if (obj != clazz) {
        operator()(obj);
    } 

    // Step 2: Sort buckets and process the ones with the most items first.
    sort(bucket_.begin(), bucket_.end(), SortJumpBuckets()); 

    // Step 3: Attempt to place items in the buckets into empty slots in the
    // second jump table, starting with the largest buckets first.
    std::vector<Function*> value(n);
    std::vector<Function*> slots;
    clazz->jump1(n-1, 0);
    for (int i = 0; i < bucket_.size(); i++) {
        if (bucket_[i].size() <= 0) { break; }
        int d = 1; 
retry:
        // Try to place all the values into empty value slots in the second
        // jump table.  If that doesn't work, then increment the hash mixing
        // value "d"
        slots = value;
        for (int j = 0; j < bucket_[i].size(); j++) {
            uint64_t hash = fnv_hash(d, bucket_[i][j]->name()) % n;
            if (slots[hash]) {
                d++;
                goto retry;
            } else {
                slots[hash] = bucket_[i][j];
            }
        }

        // Success! Record the d-value (i.e., hash mixing value) for the 
        // current bucket, and continue.
        uint64_t hash = fnv_hash(0, bucket_[i][0]->name()) % n;
        clazz->jump1(hash, d);
        value = slots;
    }
    for (int i = 0; i < value.size(); i++) {
        clazz->jump2(i, value[i]);
    }
}

void DispatchTableGenerator::operator()(Class* clazz) {
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        f(this); 
    }
}

void DispatchTableGenerator::operator()(Function* func) {
    // Add the function to the bucket for its hash value
    int const n = bucket_.size();
    if (func->is_constructor() || func->is_destructor()) {
        return;
    }
    if (func != class_->function(func->name())) {
        return;
    }
    uint64_t hash = fnv_hash(0, func->name()) % n;
    bucket_[hash].push_back(func);
}

void FunctionCounter::operator()(Class* clazz) {
    for (Feature::Ptr f = clazz->features(); f; f = f->next()) {
        f(this); 
    }
}

void FunctionCounter::operator()(Function* func) {
    if (func->is_constructor() || func->is_destructor()) {
        return;
    }
    if (func != class_->function(func->name())) {
        return;
    }
    count_++;
}

///*  Dump hash values
//    for (Feature::Ptr f = feature->features(); f; f = f->next()) {
//        if (Function::Ptr func = dynamic_cast<Function*>(f.pointer())) {
//            if (func->is_destructor() || func->is_constructor()) { 
//                continue; 
//            }
//            std::cout << func->name()->string() << std::endl;
//            uint64_t hash1 = fnv_hash(0, func->name());
//            uint64_t d = feature->jump1(hash1 % n);
//            uint64_t hash2 = fnv_hash(d, func->name());
//            std::cout << "   n: " << n << std::endl;
//            std::cout << "   hash1 %n: " << hash1 % n << std::endl;
//            std::cout << "   hash1: " << hash1 << std::endl;
//            std::cout << "   d: " << d << std::endl;
//            std::cout << "   hash2: " << hash2 << std::endl;
//        }
//    }
//    std::cout << feature->jump1s() << std::endl;
//    std::cout << feature->jump2s() << std::endl;
//*/
