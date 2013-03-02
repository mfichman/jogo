#include <Primitives.h>
#include <stdint.h>

void printc(Char ch) {
    printf("%llx\n", ch);
}

void printi(Int num) {
    printf("%llx\n", (uint64_t)num);
}

void printb(Byte byte) {
    printf("%llx\n", byte);
}


