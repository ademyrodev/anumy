#include "memory.h"
#include "datastructures/dynarr.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define MAXLOADFACTOR 0.75

#define FNVOFFSETBASIS 2166136261u
#define FNVPRIME 16777619

static Memory *memorytable = NULL;

static void initmemorytable() {
    memorytable = malloc(sizeof (*memorytable)); 

    memorytable->count = 0;
    memorytable->cap = 8;
    
    memorytable->ptrs = malloc(memorytable->cap * sizeof (void **));
}

static size_t hash(void *ptr) {
    // hashing addresses with FNV-1a
    uint32_t hash = FNVOFFSETBASIS;
    uint8_t *data = (uint8_t *) &ptr;
    
    for (int i = 0; i < sizeof (ptr); i++) {
        hash ^= data[i];
        hash ^= FNVPRIME;
    }

    return (size_t) hash;
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

static size_t find(void *ptr) {
    size_t hashvalue = hash(ptr);

    size_t mask = memorytable->cap - 1;
    size_t bucket = (size_t) hashvalue & mask;

    while (1) {
        // printf("bucket %zu\n", bucket);
        void *entry = memorytable->ptrs[bucket];

        if (entry == ptr || entry == NULL) {
            return bucket;
        }

        bucket = (bucket + 1) & mask;
    }
}

static void insert(void *ptr) {
    if (memorytable->count + 1 > memorytable->cap * MAXLOADFACTOR) {
        resize();
    }

    size_t bucket = find(ptr);
    memorytable->ptrs[bucket] = ptr;
    memorytable->count++;
}

static void delete(void *ptr) {
    size_t bucket = find(ptr);
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

void memdump() {
    for (int i = 0; i < memorytable->cap; i++) {
        void *ptr = memorytable->ptrs[i];        
        if (ptr != NULL) {
            printf("block %p, size %zu\n", ptr, sizeof (ptr));
        }
    }    
}
