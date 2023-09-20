#pragma once
#include <stdbool.h>

typedef struct {
    size_t size;
    size_t cap;

    void **items;
} DynArr;

DynArr *dynarr_new(); 

DynArr *dynarr_withcap(size_t);

DynArr *dynarr_fromrange(int, int);

void dynarr_push(DynArr *, void *);

void dynarr_remove(DynArr *, void *);

void dynarr_removeat(DynArr *, int);

void *dynarr_get(DynArr *, int);

void *dynarr_peek(DynArr *);

int dynarr_indexof(DynArr *, void *);

bool dynarr_exists(DynArr *, void *);

bool dynarr_alloc(DynArr *, size_t);

void dynarr_print(DynArr *, void (*printer)(void *));

void dynarr_free(DynArr *, void (*deallocator)(void *));

