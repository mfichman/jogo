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

#include "Pointer.hpp"

template <typename T>
class BaseGenericIter : Object {
public:
    typedef Pointer<BaseGenericIter<T> > Ptr;
    virtual ~BaseGenericIter() {}
    virtual operator bool() const=0;
    virtual T* operator->()=0;
    virtual void operator++()=0;
};

template <typename T, typename K, typename I>
class GenericIter : public BaseGenericIter<T> {
public: 
    GenericIter(I begin, I end) :
        begin_(begin),
        end_(end) {
    }

    operator bool() const { return begin_ != end_; }
    T* operator->() { return begin_.operator*().pointer(); }
    void operator++() { ++begin_; }

private:
    I begin_;
    I end_;
};

template <typename T, typename K>
class GenericIter<T, K, typename std::map<K,typename T::Ptr>::iterator > 
    : public BaseGenericIter<T> {
public: 
    typedef typename std::map<K, typename T::Ptr>::iterator I;
    GenericIter(I begin, I end) :
        begin_(begin),
        end_(end) {
    }

    operator bool() const { return begin_ != end_; }
    T* operator->() { return begin_->second; }
    void operator++() { ++begin_; }

private:
    I begin_;
    I end_;
};

template <typename T>
class ListIter : public BaseGenericIter<T> {
public: 
    ListIter(T* head) : head_(head) {}
    operator bool() const { return head_; }
    T* operator->() { return head_; }
    void operator++() { head_ = head_->next(); }

private:
    T* head_;
};

template <typename T>
class Iterator {
public:
    template <typename I>
    Iterator(I begin, I end) : 
        iter_(new GenericIter<T,T,I>(begin, end)) {
    }
    template <typename C>
    Iterator(C& coll) :
        iter_(new GenericIter<T,T,typename C::iterator>(coll.begin(), coll.end())) {
    }
    template <typename K>
    Iterator(std::map<K, typename T::Ptr>& coll) :
        iter_(new GenericIter<T,K,typename std::map<K, typename T::Ptr>::iterator>(
            coll.begin(), coll.end())) {
    }
    Iterator(T* head) : 
        iter_(new ListIter<T>(head)) { 
    }
    Iterator(typename T::Ptr head) : 
        iter_(new ListIter<T>(head)) { 
    }
    operator bool() const { return iter_->operator bool(); }
    T* operator->() const { return iter_->operator->(); }
    operator T*() const { return iter_->operator->(); }
    void operator++() { return iter_->operator++(); }
    T* pointer() const { return iter_->operator->(); }

    template <typename F>
    void operator()(F* functor) {
        pointer()->operator()(functor);
    }
private:
    BaseGenericIter<T>* iter_;
};
