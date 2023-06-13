#include "tree/tree.h"
#include "tree/tree_private.h"

unsigned long dast_knot_height(dast_knot_t* knot)
{
    if (!knot)
    {
        return 0;
    }

    unsigned long lheight = dast_knot_height(knot->left);
    unsigned long rheight = dast_knot_height(knot->right);
    return lheight >= rheight ? lheight + 1 : rheight + 1;
}

unsigned long dast_tree_height(dast_tree_t* tree) { return dast_knot_height(tree->root); }