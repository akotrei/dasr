#ifndef __DAST_TREE_H__
#define __DAST_TREE_H__

#include "interface/iallocator.h"
#include "list/list.h"

typedef struct _dast_tree_t dast_tree_t;

/* Returns number of bytes that a @dast_tree_t instance needs*/
unsigned long dast_tree_sizeof();

dast_tree_t* dast_tree_init(void*             memory,
                            dast_allocator_t* allocator,
                            unsigned long     obj_size,
                            int (*cmp_f)(void* l, void* r),
                            void (*cpy_f)(void* obj, void* memory),
                            void (*del_f)(void* obj));

dast_tree_t* dast_tree_new(dast_allocator_t* allocator,
                           unsigned long     obj_size,
                           int (*cmp_f)(void* l, void* r),
                           void (*cpy_f)(void* obj, void* memory),
                           void (*del_f)(void* obj));

void dast_tree_release(dast_tree_t* tree);

void dast_tree_delete(dast_tree_t* tree);

dast_tree_t dast_tree_copy(dast_tree_t* tree, void* memory);

dast_tree_t* dast_tree_clone(dast_tree_t* tree, dast_allocator_t* allocator);

dast_tree_t dast_tree_deepcopy(dast_tree_t* tree, void* memory);

dast_tree_t* dast_tree_deepclone(dast_tree_t*      tree,
                                 dast_allocator_t* allocator);

void dast_tree_clear(dast_tree_t* tree);

unsigned long dast_tree_length(dast_tree_t* tree);

dast_list_t* dast_tree_elements(dast_tree_t* tree);

void dast_tree_add(dast_tree_t* tree, void* obj);

void* dast_tree_find(dast_tree_t* tree, void* obj);

void* dast_tree_pop(dast_tree_t* tree, void* obj, void* memory);

int dast_tree_remove(dast_tree_t* tree, void* obj);

#endif /* __DAST_TREE_H__ */