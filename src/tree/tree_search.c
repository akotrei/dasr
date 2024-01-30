#include "tree/tree.h"
#include "tree/tree_private.h"

void* dast_tree_search(dast_tree_t* tree, void* obj)
{
    dast_cmp_t    cmp = tree->cmp;
    dast_knot_t** knot = &(tree->root);
    int           compare_result;

    while (*knot)
    {
        compare_result = cmp((char*)(*knot) + sizeof(dast_knot_t), obj);
        if (compare_result > 0)
        {
            knot = &((*knot)->left);
        }
        else if (compare_result < 0)
        {
            knot = &((*knot)->right);
        }
        else
        {
            return (char*)(*knot) + sizeof(dast_knot_t);
        }
    }

    return 0;
}