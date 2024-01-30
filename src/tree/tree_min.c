#include "tree/tree.h"
#include "tree/tree_private.h"

void* dast_knot_min(dast_knot_t* knot)
{
    if (knot)
    {
        while (1)
        {
            if (knot->left)
            {
                knot = knot->left;
            }
            else
            {
                return knot;
            }
        }
    }
    return 0;
}
void* dast_tree_min(dast_tree_t* tree) { return (char*)dast_knot_min(tree->root) + sizeof(dast_knot_t); }