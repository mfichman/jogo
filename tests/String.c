
#include "runtime/String.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

String* String__init(const Char* literal) {
    String* ret = malloc(sizeof(String) + strlen(literal) + 1);
    ret->_vtable = 0;
    ret->_refcount = 1;
    ret->length = strlen(literal);
    strcpy(ret->data, literal);
    return ret;
}

int main() {
    String* str1 = String__init("hello "); 
    assert(String__length__g(str1) == 6);
    String* str2 = String__init("world!");
    assert(String__length__g(str2) == 6);

    String* str3 = String__plus(str1, str2);
    assert(String__length__g(str3) == 12);
    assert(strncmp(str3->data, "hello world!", 12) == 0);

    String* str4 = String__init("hello world!");
    String* str5 = String__init("hello world?");
    assert(String__equal(str3, str4));
    assert(!String__equal(str3, str5));

    return 0;
}
