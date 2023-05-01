#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/cmp.h"

#include "../dev/debug.h"

void dast_tree_add(dast_tree_t* tree, void* obj)
{
    dast_cmp_f cmp = tree->cmp;
    int is_l_more_than_r;
    dast_knot_t* knot;
    dast_knot_t** root = &(tree->root);
    dast_knot_t* parent = 0;
    dast_iallocator_t* allocator = tree->allocator;

    while (*root)
    {
        parent = (*root);
        is_l_more_than_r = cmp((char*)(parent) + sizeof(dast_knot_t), obj);
        if (is_l_more_than_r >= 0)
        {
            root = &(parent->left);
        }
        else
        {
            root = &(parent->right);
        }
    }

    knot = allocator->allocate(allocator, sizeof(dast_knot_t) + tree->obj_size);
    knot->left = 0;
    knot->right = 0;
    knot->parent = parent;
    tree->cpy_f(obj, (char*)(knot) + sizeof(dast_knot_t));

    (*root) = knot;
}