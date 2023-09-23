#include "memory.h"
#include "datastructures/dynarr.h"
#include <stdlib.h>
#include <stdint.h>

#define MAXLOADFACTOR 0.75

static Memory *memorytable = NULL;

static void initmemorytable() {
    memorytable = malloc(sizeof (*memorytable)); 

    memorytable->count = 0;
    memorytable->cap = 8;
    
    memorytable->ptrs = malloc(memorytable->cap * sizeof (void **));
}

static size_t hash(void *ptr) {
    // using bit-masking--memorytable->cap is guaranteed to always be a power 
    // of two, allowing us to replace ptr % memory->cap with bit masking.
    uintptr_t address = (uintptr_t) ptr;
    size_t mask = memorytable->cap - 1; 

    return address & mask;
}

static void resize() {
    size_t newcap = memorytable->cap * 2;

    memorytable->ptrs = realloc(memorytable->ptrs, newcap * sizeof (void **));
    if (memorytable->ptrs == NULL) {
        // TODO; handle error
        return;
    } 

    memorytable->cap = newcap;
}

static void insert(void *ptr) {
    if (memorytable->count >= memorytable->cap * MAXLOADFACTOR) {
        resize();
    }

    size_t bucket = hash(ptr);
    memorytable->ptrs[bucket] = ptr;
}

static void delete(void *ptr) {
    size_t bucket = hash(ptr);
    memorytable->ptrs[bucket] = NULL;
}

void *memalloc(size_t size) {
    if (memorytable == NULL) {
        initmemorytable();
    }

    void *ptr = malloc(size); 
    if (ptr == NULL) {
        // TODO; replace with a smarter error handling approach once error.h is 
        // implemented
        return NULL;
    }

    insert(ptr);
    return ptr;
}

void *memrealloc(void *ptr, size_t newsize) {
    if (ptr == NULL) {
        // TODO; replace with a smarter error handling approach once error.h is 
        // implemented
        return NULL;
    }
    void *newptr = realloc(ptr, newsize);
    if (newptr == NULL) {
        // TODO; replace with a smarter error handling approach once error.h is 
        // implemented
        return NULL;
    }

    if (newptr != ptr) {
        delete(ptr);
        insert(newptr);
    }

    return newptr;
}

void *memcalloc(size_t elements, size_t size) {
    void *ptr = calloc(elements, size); 
    if (ptr == NULL) {
        // TODO; replace with a smarter error handling approach once error.h is 
        // implemented
        return NULL;
    }

    insert(ptr);
    return ptr;
}

bool memcheck(void *ptr) {
    return ptr != NULL;
}

void memdeepfree(void **ptr) {
    if (*ptr == NULL) {
        return;
    }

    delete(*ptr);
    free(*ptr);
    *ptr = NULL;
}

void memcleanup() {
    for (int i = 0; i < memorytable->cap; i++) {
        void *ptr = memorytable->ptrs[i];
        memfree(ptr);
    }

    memorytable->count = memorytable->cap = 0;
    free(memorytable->ptrs);
    free(memorytable);
}
