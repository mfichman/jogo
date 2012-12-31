#include <Primitives.h>
#include <stdio.h>

struct Val {
    Int val;
};
typedef struct Val* Val;

void func1(Val a, Val b, Val c) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    printf("\n");
    fflush(stdout);
}

void func2(Val a, Val b, Val c, Val d) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    printf("\n");
    fflush(stdout);
}

void func3(Val a, Val b, Val c, Val d, Val e) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    Boot_print_int(e->val);
    printf("\n");
    fflush(stdout);
}

void func4(Val a, Val b, Val c, Val d, Val e, Val f) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    Boot_print_int(e->val);
    Boot_print_int(f->val);
    printf("\n");
    fflush(stdout);
}

void func5(Val a, Val b, Val c, Val d, Val e, Val f, Val g) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    Boot_print_int(e->val);
    Boot_print_int(f->val);
    Boot_print_int(g->val);
    printf("\n");
    fflush(stdout);
}

void func6(Val a, Val b, Val c, Val d, Val e, Val f, Val g, Val h) {
    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    Boot_print_int(e->val);
    Boot_print_int(f->val);
    Boot_print_int(g->val);
    Boot_print_int(h->val);
    printf("\n");
    fflush(stdout);
}

void func7(Val a, Val b, Val c, Val d, Val e, Val f, Val g, Val h) {
    Boot_print_int(h->val);
    Boot_print_int(g->val);
    Boot_print_int(f->val);
    Boot_print_int(e->val);
    Boot_print_int(d->val);
    Boot_print_int(c->val);
    Boot_print_int(b->val);
    Boot_print_int(a->val);
    printf("\n");
    fflush(stdout);
}

void func20(Val a, Val b, Val c, Val d, Val e, Val f, Val g, Val h, Val i, Val j,
            Val k, Val l, Val m, Val n, Val o, Val p, Val q, Val r, Val s, Val t) {

    Boot_print_int(a->val);
    Boot_print_int(b->val);
    Boot_print_int(c->val);
    Boot_print_int(d->val);
    Boot_print_int(e->val);
    Boot_print_int(f->val);
    Boot_print_int(g->val);
    Boot_print_int(h->val);
    Boot_print_int(i->val);
    Boot_print_int(j->val);
    Boot_print_int(k->val);
    Boot_print_int(l->val);
    Boot_print_int(m->val);
    Boot_print_int(n->val);
    Boot_print_int(o->val);
    Boot_print_int(p->val);
    Boot_print_int(q->val);
    Boot_print_int(r->val);
    Boot_print_int(s->val);
    Boot_print_int(t->val);
    printf("\n");
    fflush(stdout);
}

void funcret(Val out) {
    out->val = 42;
}
