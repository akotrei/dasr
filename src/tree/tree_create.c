#include "tree/tree.h"
#include "tree/tree_private.h"

unsigned long dast_tree_sizeof() { return sizeof(dast_tree_t); }

dast_tree_t* dast_tree_init(void*              memory,
                            dast_allocator_t* allocator,
                            unsigned long      obj_size,
                            dast_cmp_f         cmp,
                            dast_cpy_f         cpy,
                            dast_del_f         del)
{
    dast_tree_t* tree = memory;
    tree->allocator = allocator;
    tree->obj_size = obj_size;
    tree->length = 0;
    tree->root = 0;
    tree->cmp = cmp;
    tree->cpy = cpy;
    tree->del = del;
    return tree;
}

dast_tree_t* dast_tree_new(dast_allocator_t* allocator,
                           unsigned long      obj_size,
                           dast_cmp_f         cmp,
                           dast_cpy_f         cpy,
                           dast_del_f         del)
{
    void* memory = allocator->allocate(allocator, sizeof(dast_tree_t));
    return dast_tree_init(memory, allocator, obj_size, cmp, cpy, del);
}