#include "dynarr.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

DynArr *dynarr_new(size_t itemsize) {
    DynArr *dynarr = malloc(sizeof (*dynarr));    
    if (dynarr == NULL) {
        return NULL; 
    }
    
    dynarr->size = 0;
    dynarr->cap = 4;

    dynarr->items = malloc(dynarr->cap * sizeof (void *));
    if (dynarr->items == NULL) {
        return NULL;
    }

    return dynarr;
}

DynArr *dynarr_withcap(size_t cap, size_t itemsize) {
    // yeah this body is somewhat similar to the previous function's body but
    // i don't wanna reallocate dynarr->items after creating it with 
    // dynarr_new()
    DynArr *dynarr = malloc(sizeof (*dynarr));    
    if (dynarr == NULL) {
        free(dynarr);
        return NULL; 
    }

    dynarr->size = 0;
    dynarr->cap = cap;

    dynarr->items = malloc(cap * sizeof (void *));
    if (dynarr->items == NULL) {
        free(dynarr);
        return NULL;
    }

    return dynarr;
}

DynArr *dynarr_fromarray(const void *array, size_t length) {
    if (array == NULL || length == 0) {
        return NULL;
    }

    DynArr *dynarr = malloc(sizeof (*dynarr));
    if (dynarr == NULL) {
        return NULL; 
    }

    size_t bytes = length * sizeof (void *);

    dynarr->size = dynarr->cap = length;
    dynarr->items = malloc(bytes);

    memcpy(dynarr->items, array, bytes); 

    return dynarr;
}

DynArr *dynarr_cpy(DynArr *source) {
    if (source == NULL || source->items == NULL) {
        return NULL;
    }

    DynArr *copy = malloc(sizeof (*dynarr));
    if (copy == NULL) {
        return NULL;
    }

    copy->size = source->size;
    copy->cap = source->cap;

    size_t bytes = source->cap * sizeof (void *);
    
    // bye bye UB
    copy->items = malloc(bytes);
    if (copy->items == NULL) {
        return NULL; 
    }

    memcpy(copy->items, source->items, bytes);

    return copy;
}

void dynarr_push(DynArr *self, void *item) {
    if (item == NULL || self == NULL) {
        return;
    }

    if (self->size >= self->cap) {
        size_t newcap = self->cap * 2;

        if (!dynarr_alloc(self, newcap)) {
            // handle error (*!)!*(!)!(!)(!)!()9
            return;
        }

        self->cap = newcap;
    }

    self->items[self->size++] = item;
}

void dynarr_remove(DynArr *self, void *item) {
    if (self == NULL) {
        return;
    }
    
    int index = dynarr_find(item);
    if (index == -1) {
        return; 
    }

    free(item);

    dynarr_leftshift(self, index);
}

void dynarr_removeat(DynArr *self, int index) {
    // why am i having so much fun
    if (self == NULL || self->items == NULL) {
        return;
    }

    if (index < 0 || index > self->size) {
        return;
    }
    
    free(self->items[index]);

    dynarr_leftshift(self, index);
}

bool dynarr_alloc(DynArr *self, size_t bytes) {
    if (bytes < self->size || self == NULL) {
        return false;
    }
    
    self->cap = bytes; 

    self->items = realloc(self->items, bytes);
    if (self->items == NULL) {
        return false;
    }

    return true; 
}

static void dynarr_leftshift(DynArr *self, int index) {
    for (int i = index; i < self->size - 1; i++) {
        self->items[i] = self->items[i + 1]; 
    }

    self->size--;
}
