#include "tree/tree.h"
#include "tree/tree_private.h"

void* dast_knot_max(dast_knot_t* knot)
{
    if (knot)
    {
        while (1)
        {
            if (knot->right)
            {
                knot = knot->right;
            }
            else
            {
                return knot;
            }
        }
    }
    return 0;
}

void* dast_tree_max(dast_tree_t* tree) { return (char*)dast_knot_max(tree->root) + sizeof(dast_knot_t); }