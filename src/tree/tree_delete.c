#include "tree/tree.h"
#include "tree/tree_private.h"

#include "../dev/debug.h"
void dast_tree_knots_free(dast_iallocator_t* allocator, dast_knot_t* knot)
{
    if (knot->left)
    {
        dast_tree_knots_free(allocator, knot->left);
    }
    else if (knot->right)
    {
        dast_tree_knots_free(allocator, knot->right);
    }
    else if (knot->parent)
    {
        if (knot->parent->left == knot)
        {
            knot = knot->parent;
            DEBUG_PRINT("left: %d\n", *(int*)((char*)(knot->left) + sizeof(dast_knot_t)));
            allocator->deallocate(allocator, knot->left);
            knot->left = 0;
        }
        else
        {
            knot = knot->parent;
            DEBUG_PRINT("right: %d\n", *(int*)((char*)(knot->right) + sizeof(dast_knot_t)));
            allocator->deallocate(allocator, knot->right);
            knot->right = 0;
        }
        dast_tree_knots_free(allocator, knot);
    }
    else
    {
        DEBUG_PRINT("root: %d\n", *(int*)((char*)(knot) + sizeof(dast_knot_t)));
        allocator->deallocate(allocator, knot);
    }
}

void dast_tree_free(dast_tree_t* tree)
{
    DEBUG_PRINT("tree->root: %p\n", tree->root);
    if (tree->root)
    {
        dast_tree_knots_free(tree->allocator, tree->root);
    }
}

void dast_tree_delete(dast_tree_t* tree)
{
    dast_iallocator_t* allocator = tree->allocator;
    dast_tree_free(tree);
    allocator->deallocate(allocator, tree);
}