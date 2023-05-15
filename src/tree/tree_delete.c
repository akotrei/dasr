#include "interface/iallocator.h"
#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/mem.h"

void dast_tree_free(dast_tree_t* tree)
{
    dast_iallocator_t* allocator = tree->allocator;
    dast_del_f         del = tree->del;
    dast_knot_t**      glob = &(tree->root);
    dast_knot_t*       curr;

    while (*glob)
    {
        curr = *glob;

        if ((*glob = curr->left))
        {
        }
        else if ((*glob = curr->right))
        {
        }
        else if (curr->parent)
        {
            *glob = curr->parent;
            del((char*)(curr) + sizeof(dast_knot_t));
            allocator->deallocate(allocator, curr);
            if (curr->parent->left == curr)
            {
                (*glob)->left = 0;
            }
            else
            {
                (*glob)->right = 0;
            }
        }
        else
        {
            del((char*)(curr) + sizeof(dast_knot_t));
            allocator->deallocate(allocator, curr);
            curr = 0;
        }
    }
}

void dast_tree_delete(dast_tree_t* tree)
{
    dast_iallocator_t* allocator = tree->allocator;
    dast_tree_free(tree);
    allocator->deallocate(allocator, tree);
}