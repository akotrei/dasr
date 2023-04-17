#include "tree/tree.h"
#include "tree/tree_private.h"

unsigned long dast_tree_sizeof() { return sizeof(dast_tree_t); }

dast_tree_t* dast_tree_init(void*              memory,
                            dast_iallocator_t* allocator,
                            unsigned long      obj_size,
                            int (*cmp_f)(void* l, void* r),
                            void (*cpy_f)(void* obj, void* memory),
                            void (*del_f)(void* obj))
{
    dast_tree_t* tree = memory;
    tree->allocator = allocator;
    tree->obj_size = obj_size;
    tree->length = 0;
    tree->root = 0;
    tree->cmp_f = cmp_f;
    tree->cpy_f = cpy_f;
    tree->del_f = del_f;
    return tree;
}

dast_tree_t* dast_tree_new(dast_iallocator_t* allocator,
                           unsigned long      obj_size,
                           int (*cmp_f)(void* l, void* r),
                           void (*cpy_f)(void* obj, void* memory),
                           void (*del_f)(void* obj))
{
    void* memory = allocator->allocate(allocator, sizeof(dast_tree_t));
    return dast_tree_init(memory, allocator, obj_size, cmp_f, cpy_f, del_f);
}