#include "memory/memory.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *r = memalloc(sizeof (*r));
    int *s = memalloc(sizeof (*s));
    int *t = memalloc(sizeof (*t));
    int *u = memalloc(sizeof (*u));
    int *v = memalloc(sizeof (*v));
    int *w = memalloc(sizeof (*w));
    int *x = memalloc(sizeof (*x));
    int *y = memalloc(sizeof (*y)); 
    int *z = memalloc(sizeof (*z));
    memdump();
    memcleanup();

    return 0;
}
