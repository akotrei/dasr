#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_clear(dast_tree_t* tree)
{
    dast_tree_destroy_from(tree);
    tree->size = 0;
}