#include "tree/tree.h"
#include "tree/tree_private.h"

dast_u64_t dast_tree_sizeof() { return sizeof(dast_tree_t); }

dast_tree_t* dast_tree_init_on(void*             memory,
                               dast_allocator_t* allocator,
                               dast_u64_t        obj_size,
                               dast_cmp_t        cmp,
                               dast_cpy_t        cpy,
                               dast_del_t        del)
{
    dast_tree_t* tree = memory;
    tree->allocator = allocator;
    tree->obj_size = obj_size;
    tree->size = 0;
    tree->root = 0;
    tree->cmp = cmp;
    tree->cpy = cpy;
    tree->del = del;
    tree->min = 0;
    tree->max = 0;
    return tree;
}

dast_tree_t* dast_tree_init(dast_allocator_t* allocator,
                            dast_u64_t        obj_size,
                            dast_cmp_t        cmp,
                            dast_cpy_t        cpy,
                            dast_del_t        del)
{
    void* memory = allocator->allocate(allocator, sizeof(dast_tree_t));
    return dast_tree_init_on(memory, allocator, obj_size, cmp, cpy, del);
}