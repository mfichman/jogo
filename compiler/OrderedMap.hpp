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

#include "Jogo.hpp"
#include "Iterator.hpp"
#include <map>
#include <functional>
#include <unordered_map>

/* Ordered map that preserves the insertion order of elements (insert-only) */
template <typename K, typename V>
class OrderedMap {
public:
    void insert(V* value) { 
        list_ = append(list_, value);
        map_.insert(std::make_pair(value->name(), value));
    }

    typename V::Itr iterator() const { 
        return typename V::Itr(list_); 
    }

    class Compare {
    public:
        bool operator()(K k1, K k2) const {
            //return k1->string() < k2->string(); 
            return k1 < k2;
        }        
    };

    class Hash {
    public:
        size_t operator()(K k1) const {
            return (size_t)k1.pointer();
        }
    };

    V* value(K name) const {
        typename std::map<K, typename V::Ptr, Compare>::const_iterator i =
            map_.find(name);
        if (i == map_.end()) {
            return 0;
        } else {
            return i->second;
        }
    }

private:
    mutable std::map<K, typename V::Ptr, Compare> map_;
    typename V::Ptr list_;
};
