#ifndef DEF_CLIST
#define DEF_CLIST

#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 8
#define INFLATE_FACTOR 2
#define SHRINK_FACTOR 0.5

typedef int T;
typedef struct CList CList;

struct CList {
    T *tab;
    size_t size;
    size_t space;
};

CList *clist_new();

void clist_free(CList *);

void clist_resize(CList *, size_t);

static inline void clist_inflate(CList *p_clist) {
    clist_resize(p_clist, (size_t) (INFLATE_FACTOR * p_clist->space + 1));
}

static inline void clist_shrink(CList *p_clist) {
    clist_resize(p_clist, (size_t) (SHRINK_FACTOR * p_clist->space + 1));
}

void clist_push(CList *, T);

#endif  // DEF_CLIST

