#include <Primitives.h>
#include <stdio.h>
#include <stdint.h>

void printc(Char ch) {
#ifdef LINUX
    printf("%lx\n", (uint64_t)ch);
#else
    printf("%llx\n", (uint64_t)ch);
#endif
}

void printi(Int num) {
#ifdef LINUX
    printf("%lx\n", (uint64_t)num);
#else
    printf("%llx\n", (uint64_t)num);
#endif
}

void printb(Byte byte) {
#ifdef LINUX
    printf("%lx\n", (uint64_t)byte);
#else
    printf("%llx\n", (uint64_t)byte);
#endif
}


