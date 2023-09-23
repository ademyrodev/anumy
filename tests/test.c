#include "memory/memory.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *x = memalloc(sizeof (*x));
    printf("x from main before being freed: %p\n", x);
    memfree(x);
    printf("x from main after being freed: %p\n", x);
    memcleanup();

    return 0;
}
