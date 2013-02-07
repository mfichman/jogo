#include <Primitives.h>
#include <Boot/Module.h>
#include <stdio.h>

void func1(Float a, Float b, Float c) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    printf("\n");
    fflush(stdout);
}

void func2(Float a, Float b, Float c, Float d) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    printf("\n");
    fflush(stdout);
}

void func3(Float a, Float b, Float c, Float d, Float e) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    Boot_print_float(e);
    printf("\n");
    fflush(stdout);
}

void func4(Float a, Float b, Float c, Float d, Float e, Float f) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    Boot_print_float(e);
    Boot_print_float(f);
    printf("\n");
    fflush(stdout);
}

void func5(Float a, Float b, Float c, Float d, Float e, Float f, Float g) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    Boot_print_float(e);
    Boot_print_float(f);
    Boot_print_float(g);
    printf("\n");
    fflush(stdout);
}

void func6(Float a, Float b, Float c, Float d, Float e, Float f, Float g, Float h) {
    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    Boot_print_float(e);
    Boot_print_float(f);
    Boot_print_float(g);
    Boot_print_float(h);
    printf("\n");
    fflush(stdout);
}

void func7(Float a, Float b, Float c, Float d, Float e, Float f, Float g, Float h) {
    Boot_print_float(h);
    Boot_print_float(g);
    Boot_print_float(f);
    Boot_print_float(e);
    Boot_print_float(d);
    Boot_print_float(c);
    Boot_print_float(b);
    Boot_print_float(a);
    printf("\n");
    fflush(stdout);
}

void func20(Float a, Float b, Float c, Float d, Float e, Float f, Float g, Float h, Float i, Float j,
            Float k, Float l, Float m, Float n, Float o, Float p, Float q, Float r, Float s, Float t) {

    Boot_print_float(a);
    Boot_print_float(b);
    Boot_print_float(c);
    Boot_print_float(d);
    Boot_print_float(e);
    Boot_print_float(f);
    Boot_print_float(g);
    Boot_print_float(h);
    Boot_print_float(i);
    Boot_print_float(j);
    Boot_print_float(k);
    Boot_print_float(l);
    Boot_print_float(m);
    Boot_print_float(n);
    Boot_print_float(o);
    Boot_print_float(p);
    Boot_print_float(q);
    Boot_print_float(r);
    Boot_print_float(s);
    Boot_print_float(t);
    printf("\n");
    fflush(stdout);
}

Float funcret() {
    return 42.1;
}
