#ifndef __DAST_TREE_H__
#define __DAST_TREE_H__

#include "interface/iallocator.h"
#include "interface/iiterator.h"
#include "utils/cmp.h"
#include "utils/mem.h"

typedef struct _dast_tree_t dast_tree_t;

/* Returns number of bytes that a @dast_tree_t instance needs */
unsigned long dast_tree_sizeof();

dast_tree_t* dast_tree_init(void*             memory,
                            dast_allocator_t* allocator,
                            unsigned long     obj_size,
                            dast_cmp_f        cmp,
                            dast_cpy_f        cpy,
                            dast_del_f        del);

dast_tree_t* dast_tree_new(dast_allocator_t* allocator,
                           unsigned long     obj_size,
                           dast_cmp_f        cmp,
                           dast_cpy_f        cpy,
                           dast_del_f        del);

void dast_tree_free(dast_tree_t* tree);

void dast_tree_delete(dast_tree_t* tree);

dast_tree_t dast_tree_copy(dast_tree_t* tree, void* memory);

dast_tree_t dast_tree_deepcopy(dast_tree_t* tree, void* memory);

dast_tree_t* dast_tree_clone(dast_tree_t* tree);

dast_tree_t* dast_tree_deepclone(dast_tree_t* tree);

void dast_tree_clear(dast_tree_t* tree);

unsigned long dast_tree_count(dast_tree_t* tree);

unsigned long dast_tree_height(dast_tree_t* tree);

dast_iterator_t* dast_tree_iterator_new(dast_tree_t* tree);

void dast_tree_iterator_delete(dast_iterator_t* iterator);

void dast_tree_insert(dast_tree_t* tree, void* obj);

void* dast_tree_search(dast_tree_t* tree, void* obj);

void dast_tree_remove(dast_tree_t* tree, void* obj);

#endif /* __DAST_TREE_H__ */