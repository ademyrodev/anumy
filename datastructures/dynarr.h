typedef struct {
    size_t size;
    size_t cap;

    void **items;
} DynArr;

DynArr *dynarr_new(); 

DynArr *dynarr_withcap(size_t);

DynArr *dynarr_fromarray(const void *, size_t);

DynArr *dynarr_cpy(DynArr *);

void dynarr_push(DynArr *, void *);

void dynarr_remove(DynArr *, void *);

void dynarr_removeat(DynArr *, int);

void dynarr_alloc(DynArr *, size_t);

void *dynarr_get(DynArr *, int);

void *dynarr_find(DynArr *, void *);

void *dynarr_peek(DynArr *);

int dynarr_indexof(DynArr *, void *);

void dynarr_print(DynArr *);
