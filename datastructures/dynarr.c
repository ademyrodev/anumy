#include "dynarr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static void dynarr_leftshift(DynArr *self, int index) {
    for (int i = index; i <= self->size - 1; i++) {
        self->items[i] = self->items[i + 1]; 
    }

    self->size--;
}

DynArr *dynarr_new() {
    DynArr *dynarr = malloc(sizeof (*dynarr));    
    if (dynarr == NULL) {
        return NULL; 
    }
    
    dynarr->size = 0;
    dynarr->cap = 4;

    dynarr->items = malloc(dynarr->cap * sizeof (void **));
    if (dynarr->items == NULL) {
        return NULL;
    }

    return dynarr;
}

DynArr *dynarr_withcap(size_t cap) {
    // yeah this body is somewhat similar to the previous function's body but
    // i don't wanna reallocate dynarr->items after creating it with 
    // dynarr_new()
    DynArr *dynarr = malloc(sizeof (*dynarr));    
    if (dynarr == NULL) {
        return NULL; 
    }

    dynarr->size = 0;
    dynarr->cap = cap;

    dynarr->items = malloc(cap * sizeof (void **));
    if (dynarr->items == NULL) {
        free(dynarr);
        return NULL;
    }

    return dynarr;
}

DynArr *dynarr_fromrange(int leftlimit, int rightlimit) {
    DynArr *dynarr = dynarr_new();

    for (int i = leftlimit; i < rightlimit; i++) {
        int *item = malloc(sizeof (*item));
        *item = i;
        dynarr_push(dynarr, item);
    }

    return dynarr;
}

void dynarr_push(DynArr *self, void *item) {
    if (item == NULL || self == NULL) {
        return;
    }

    if (self->size >= self->cap) {
        size_t newcap = self->cap * 2;

        if (!dynarr_alloc(self, newcap)) {
           // handle error (*!)!*(!)!(!)(!)!()8
           return;
        }

        self->cap = newcap;
    }

    self->items[self->size++] = item;
}

// using dynarr_set could potentially mislead future dynarr_push calls.
void dynarr_set(DynArr *self, void *item, int index) {
    if (index < 0 || index >= self->cap || item == NULL || self == NULL) {
        return;
    }

    self->items[index] = item;
    self->itemcount++;
}

void dynarr_remove(DynArr *self, void *item) {
    if (self == NULL) {
        return;
    }
    
    int index = dynarr_indexof(self, item);
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

    self->items = realloc(self->items, bytes * sizeof (void **));
    if (self->items == NULL) {
        return false;
    }

    return true; 
}

void *dynarr_get(DynArr *self, int index) {
    if (index < 0 || index > self->size || self == NULL) {
        return NULL;
    }

    return self->items[index];
}

bool dynarr_exists(DynArr *self, void *item) {
    if (item == NULL || self == NULL) {
        return false;
    }

    for (int i = 0; i <= self->size - 1; i++) {
        if (self->items[i] == item) {
            return true;
        }
    }

    return false;
}

void *dynarr_peek(DynArr *self) {
    if (self == NULL) {
        return NULL;
    }

    return self->items[self->size - 1];
}

int dynarr_indexof(DynArr *self, void *item) {
    if (self == NULL || item == NULL) {
        return -1;
    }

    for (int i = 0; i <= self->size - 1; i++) {
        if (self->items[i] == item) {
            return i;
        }
    }

    return -1;
}

// assuming self contains items of identical types.  if types are mixed, users 
// will have to define their own print functions. 
void dynarr_print(DynArr *self, void (*printer)(void *)) {
    if (self == NULL || self->size <= 0) {
        // might replace it with an error from error.h
        printf("[]\n");
        return;
    }
    
    printf("[");
    for (int i = 0; i <= self->size - 1; i++) {
        printer(self->items[i]);
        printf(i == self->size - 1 ? "]\n" : ", ");
    }
}

static void dynarr_zeroedout(DynArr *self) {
    if (self == NULL) {
        return;
    }

    self->cap = self->size = 0;
    free(self->items);
    free(self);
}

// assuming every item's type in self->item is the same
void dynarr_free(DynArr *self, void (*deallocator)(void *)) {
    if (self == NULL) {
        return;
    }

    for (int i = 0; i < self->size; i++) {
        deallocator(self->items[i]);
    }

    dynarr_zeroedout(self);
}
