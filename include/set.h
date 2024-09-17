#ifndef __DAST_SET_H__
#define __DAST_SET_H__

#include "interface/allocator.h"
#include "list/list.h"

typedef struct _dast_set_t dast_set_t;

/* Returns number of bytes that a @dast_set_t instance needs*/
void dast_set_init(
    dast_set_t*       set,
    dast_allocator_t* allocator,
    int               obj_size,
    unsigned long     (hash)(void* key),
    int               (*cmp)(void* key1, void* key2)
);

void dast_set_destroy(dast_set_t* set);

void dast_set_copy(dast_set_t* set, dast_set_t* dst);

void dast_set_deepcopy(dast_set_t* set, dast_set_t* dst, void (*cpy)(void* src, void* dst));

void dast_set_clear(dast_set_t* set);

void dast_set_deepclear(dast_set_t* set, void (*del)(void* obj));

void *dast_set_get(dast_set_t* set, void* key);
void dast_set_add(dast_set_t* set, void* key);
void dast_set_del(dast_set_t* set, void* key);

void* dast_set_cursor(dast_set_t* set);
void dast_set_next(dast_set_t* set, void* cursor);
void dast_set_prev(dast_set_t* set, void* cursor);

void dast_set_union(dast_set_t* set1, dast_set_t* set2, dast_set_t* dst);
void dast_set_intersect(dast_set_t* set1, dast_set_t* set2, dast_set_t* dst);
void dast_set_sub(dast_set_t* set1, dast_set_t* set2, dast_set_t* dst);

#endif /* __DAST_SET_H__ */