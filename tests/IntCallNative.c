#include <Primitives.h>
#include <Boot/Boot.h>
#include <stdio.h>

void func1(Int a, Int b, Int c) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    printf("\n");
    fflush(stdout);
}

void func2(Int a, Int b, Int c, Int d) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    printf("\n");
    fflush(stdout);
}

void func3(Int a, Int b, Int c, Int d, Int e) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    Boot_print_int(e);
    printf("\n");
    fflush(stdout);
}

void func4(Int a, Int b, Int c, Int d, Int e, Int f) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    Boot_print_int(e);
    Boot_print_int(f);
    printf("\n");
    fflush(stdout);
}

void func5(Int a, Int b, Int c, Int d, Int e, Int f, Int g) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    Boot_print_int(e);
    Boot_print_int(f);
    Boot_print_int(g);
    printf("\n");
    fflush(stdout);
}

void func6(Int a, Int b, Int c, Int d, Int e, Int f, Int g, Int h) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    Boot_print_int(e);
    Boot_print_int(f);
    Boot_print_int(g);
    Boot_print_int(h);
    printf("\n");
    fflush(stdout);
}

void func7(Int a, Int b, Int c, Int d, Int e, Int f, Int g, Int h) {
    Boot_print_int(h);
    Boot_print_int(g);
    Boot_print_int(f);
    Boot_print_int(e);
    Boot_print_int(d);
    Boot_print_int(c);
    Boot_print_int(b);
    Boot_print_int(a);
    printf("\n");
    fflush(stdout);
}

void func20(Int a, Int b, Int c, Int d, Int e, Int f, Int g, Int h, Int i, Int j,
            Int k, Int l, Int m, Int n, Int o, Int p, Int q, Int r, Int s, Int t) {

    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
    Boot_print_int(d);
    Boot_print_int(e);
    Boot_print_int(f);
    Boot_print_int(g);
    Boot_print_int(h);
    Boot_print_int(i);
    Boot_print_int(j);
    Boot_print_int(k);
    Boot_print_int(l);
    Boot_print_int(m);
    Boot_print_int(n);
    Boot_print_int(o);
    Boot_print_int(p);
    Boot_print_int(q);
    Boot_print_int(r);
    Boot_print_int(s);
    Boot_print_int(t);
    printf("\n");
    fflush(stdout);
}

Int funcret() {
    return 42;
}
