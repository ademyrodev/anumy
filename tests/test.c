#include <stdio.h>
#include <stdlib.h>
#include "datastructures/dynarr.c"

void printint(void *ptr) {
    int *i = (int *) ptr;
    printf("%d", *i);
}

int main(void) {
    DynArr *dynarr = dynarr_fromrange(0, 10);
    dynarr_print(dynarr, printint);

    DynArr *dynarrcopy = dynarr_cpy(dynarr);
    dynarr_print(dynarrcopy, printint);

    dynarr_free(dynarr, free);
}
