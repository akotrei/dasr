#include "ntype.h"
#include "tree/tree.h"
#include "tree/tree_private.h"

dast_u64_t dast_knot_height(dast_knot_t* knot)
{
    if (!knot)
    {
        return 0;
    }

    dast_u64_t lheight = dast_knot_height(knot->left);
    dast_u64_t rheight = dast_knot_height(knot->right);
    return lheight >= rheight ? lheight + 1 : rheight + 1;
}

dast_u64_t dast_tree_height(dast_tree_t* tree) { return dast_knot_height(tree->root); }