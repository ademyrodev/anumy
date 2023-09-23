// not using datastructures/hashtab.h to avoid unnecessary overhead.
// not using datastructures/dynarr.h to avoid unnecessary overhead and circular
// imports
#pragma once
#include <stddef.h>
#include <stdbool.h>

// had to break my conventions there--sorry.
#define memfree(ptr) memdeepfree((void **)&(ptr)); 

typedef struct {
    size_t count;
    size_t cap;

    void **ptrs;
} Memory;

extern void *memalloc(size_t);

extern void *memrealloc(void *, size_t);

extern void *memcalloc(size_t, size_t);

extern bool memcheck(void *);

extern void memdeepfree(void **);

extern void memcleanup();

extern void memdump();
