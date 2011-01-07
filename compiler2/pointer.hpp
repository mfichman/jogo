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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#ifndef POINTER_HPP
#define POINTER_HPP

template <typename T>
class Pointer {
public:
    Pointer() : object_(0) {}

    Pointer(T* object) : object_(object) {
        if (object_) {
            object_->refcount(object_->refcount() + 1);
        }
    } 

    Pointer(const Pointer<T>& other) : object_(other.object_) {
        if (object_) {
            object_->refcount(object_->refcount() + 1);
        }
    }

    Pointer<T>& operator=(const Pointer<T>& other) {
        if (object_) {
            object_->refcount(object_->refcount() - 1);
        }
        object_ = other.object_;
        if (object_) {
            object_->refcount(object_->refcount() + 1);
            if (object_->refcount() <= 0) {
                delete object_;
            }
        }
		return *this;
    }

    template <typename F>
    void operator()(F* functor) {
        if (object_) {
            object_->operator()(functor);
        }
    }
    

    T* pointer() const { return object_; }
    T* operator->() const { return object_; }
    operator T*() const { return object_; }

private:
    mutable T* object_;
};



#endif
