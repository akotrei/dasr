#include "tree/tree.h"
#include "tree/tree_private.h"

#include "../dev/debug.h"
void dast_tree_knots_free(dast_iallocator_t* allocator, dast_knot_t* knot)
{
    if (knot)
    {
        DEBUG_PRINT("dast_tree_knots_free\n");
        if (knot->left)
        {
            dast_tree_knots_free(allocator, knot->left);
        }
        else if (knot->right)
        {
            dast_tree_knots_free(allocator, knot->right);
        }
        else
        {
            if (knot->parent->left == knot)
            {   
                knot = knot->parent;
                allocator->deallocate(allocator, knot->left);
            }
            else
            {
                knot = knot->parent;
                allocator->deallocate(allocator, knot->right);
            }
        }
    }
}

void dast_tree_free(dast_tree_t* tree)
{
    dast_tree_knots_free(tree->allocator, tree->root);
}

void dast_tree_delete(dast_tree_t* tree)
{
    dast_iallocator_t* allocator = tree->allocator;
    dast_tree_free(tree);
    allocator->deallocate(allocator, tree);
}