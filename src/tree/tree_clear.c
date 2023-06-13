#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_clear(dast_tree_t* tree)
{
    dast_tree_free(tree);
    tree->count = 0;
    tree->root = 0;
}