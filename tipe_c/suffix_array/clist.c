#include <stdio.h>
#include <stdlib.h>

#include "clist.h"

CList *clist_new() {
    CList *p_clist = (CList *) (malloc(sizeof(*p_clist)));
    p_clist->size = 0;
    p_clist->space = INIT_SIZE;
    p_clist->tab = (T *) (malloc(INIT_SIZE * sizeof(T)));
    return p_clist;
}

void clist_free(CList *p_clist) {
    free(p_clist->tab);
    free(p_clist);
}

void clist_resize(CList *p_clist, size_t new_space) {
    p_clist->space = new_space;
    p_clist->tab = (T *) (realloc(p_clist->tab, p_clist->space * sizeof(T)));
}

void clist_push(CList *p_clist, T elt) {
    if (p_clist->size < p_clist->space) {
        p_clist->tab[p_clist->size] = elt;
        p_clist->size++;
    } else {
        clist_inflate(p_clist);
        clist_push(p_clist, elt);
    }
}

