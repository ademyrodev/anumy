#pragma once
#include <stdbool.h>

typedef struct {
    size_t size;
    size_t cap;

    // for dynarr_insert
    size_t itemcount;
    void **items;
} DynArr;

extern DynArr *dynarr_new(); 

extern DynArr *dynarr_withcap(size_t);

extern DynArr *dynarr_fromrange(int, int);

extern void dynarr_push(DynArr *, void *);

extern void dynarr_edit(DynArr *, void *, int);

extern void dynarr_remove(DynArr *, void *);

extern void dynarr_removeat(DynArr *, int);

extern void *dynarr_get(DynArr *, int);

extern void *dynarr_peek(DynArr *);

extern int dynarr_indexof(DynArr *, void *);

extern bool dynarr_exists(DynArr *, void *);

extern bool dynarr_alloc(DynArr *, size_t);

extern void dynarr_print(DynArr *, void (*printer)(void *));

extern void dynarr_free(DynArr *, void (*deallocator)(void *));

