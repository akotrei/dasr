#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_clear(dast_tree_t* tree)
{
    dast_tree_deinit(tree);
    tree->size = 0;
}