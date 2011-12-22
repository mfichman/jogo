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

#include "Queue.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>

#define QUEUE_DEFAULT_SIZE 16

Queue Queue__init(Int capacity) {
    // Allocate space for the queue struct, then allocate an array for the
    // actual data.
    Queue ret = calloc(sizeof(struct Queue), 1);
    if (!ret) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->_vtable = Queue__vtable;
    ret->_refcount = 1;
    if (capacity <= 0) {
        capacity = QUEUE_DEFAULT_SIZE;
    }
    ret->data = calloc(sizeof(Object), capacity);
    if (!ret->data) {
        fprintf(stderr, "Out of memory");
        fflush(stderr);
        abort();
    }
    ret->capacity = capacity;
    ret->front = 0; // Points to the next open slot
    ret->back = 0; // Points to the 
    return ret;
}

void Queue__destroy(Queue self) {
    Int i = 0;
    for (; i < self->count; ++i) {
        Object__refcount_dec(self->data[i]);
    }
    free(self->data);
    free(self);
}

void Queue_enq(Queue self, Object obj) {
    // Enqueue a new object at the head of the array, resizing the backing
    // buffer if necessary  
    if (!obj) { return; }    
    if ((self->front == self->back) && self->count) {
        // Queue needs to be resized, so double the size of the array, and
        // then copy the array elements over.
        Int i = 0;
        Int j = 0;
        Object* temp = 0;
        self->capacity *= 2;
        temp = calloc(sizeof(Object), self->capacity);
        if (!temp) {
            fprintf(stderr, "Out of memory");       
            fflush(stderr);
            abort();
        }
        
        // Copy data to the new buffer
        for (i = self->front; i != self->back; i = ((i+1) % self->capacity)) {
            temp[j] = self->data[i];  
            ++j;
        }
        self->front = self->count+1;
        self->back = 0;
        free(self->data);
        self->data = temp;
    } 
    self->front = (self->front+1) % self->capacity;
    self->data[self->front] = obj; 
    self->count++;
    Object__refcount_inc(obj); 
}

Object Queue_deq(Queue self) {
    // Dequeue an object from the tail of the array.  Return nil if the queue 
    // is empty.
    Object obj = 0;
    if (!self->count) { return 0; }

    obj = self->data[self->back];
    self->data[self->back] = 0;
    self->back = (self->back+1) % self->capacity;
    self->count--;
    return obj;
}

Object Queue_first__g(Queue self) {
    Object obj = self->data[self->front]; 
    Object__refcount_inc(obj);
    return obj;
}

Object Queue_last__g(Queue self) {
    Object obj = self->data[self->back];
    Object__refcount_inc(obj);
    return obj;
}

Bool QueueIter_more__g(QueueIter self) {
    return self->count < self->queue->count; 
}

Object QueueIter_next(QueueIter self) {
    if (self->count < self->queue->count) {
        Object obj = self->queue->data[self->index];
        self->index = (self->index+1) % self->queue->capacity;
        self->count++;
        Object__refcount_inc(obj);
        return obj;
    } else { 
        return 0;
    }
}
