#include "runtime/Primitives.h"
#include <stdlib.h>

typedef struct Object1 {
    Ptr _vtable;
    U64 _refcount;
} Object1;


//extern Ptr Object1__vtable __attribute__((section("data")));
extern void Object1__vtable();

Object1* Object1__init() {
    Object1* self = malloc(sizeof(struct Object1));
    self->_vtable = Object1__vtable;
    self->_refcount = 1; 

    return self;
}
