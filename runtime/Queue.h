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

#ifndef JOGO_QUEUE_H
#define JOGO_QUEUE_H

#include "Primitives.h"

typedef struct Queue* Queue;
struct Queue {
    Ptr _vtable;
    U64 _refcount;
    Int capacity;
    Int front;
    Int back;
    Int count;
    Object* data;
};

Queue Queue__init(Int capacity);
void Queue__destroy(Queue self);
void Queue_enq(Queue self, Object obj);
Object Queue_deq(Queue self);
Object Queue_first__g(Queue self);
Object Queue_last__g(Queue self);
Int Queue_count__g(Queue self);
extern void Queue__vtable();

typedef struct QueueIter* QueueIter;
struct QueueIter {
    Ptr _vtable;
    U64 _refcount;
    Queue queue;
    Int index;
    Int count;
};

Bool QueueIter_more__g(QueueIter self);
Object QueueIter_next(QueueIter self);

#endif
