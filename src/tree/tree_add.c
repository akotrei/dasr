#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/cmp.h"
#include "utils/mem.h"

void dast_tree_add(dast_tree_t* tree, void* obj)
{
    dast_cmp_t         cmp = tree->cmp;
    dast_cpy_t         cpy = tree->cpy;
    dast_del_t         del = tree->del;
    dast_knot_t*       knot_new;
    dast_knot_t**      curr = &(tree->root);
    dast_knot_t*       parent = 0;
    dast_iallocator_t* allocator = tree->allocator;
    int                compare_result;

    while (*curr)
    {
        parent = *curr;
        compare_result = cmp((char*)(parent) + sizeof(dast_knot_t), obj);
        if (compare_result > 0)
        {
            curr = &(parent->left);
        }
        else if (compare_result < 0)
        {
            curr = &(parent->right);
        }
        else
        {
            del((char*)(parent) + sizeof(dast_knot_t));
            cpy(obj, (char*)(parent) + sizeof(dast_knot_t), tree->obj_size);
            return;
        }
    }

    knot_new = allocator->allocate(allocator, sizeof(dast_knot_t) + tree->obj_size);
    knot_new->is_black = 0;
    knot_new->left = 0;
    knot_new->right = 0;
    knot_new->parent = parent;
    cpy(obj, (char*)(knot_new) + sizeof(dast_knot_t), tree->obj_size);

    *curr = knot_new;
    tree->size++;

    dast_tree_add_fix_up(tree, knot_new);
}