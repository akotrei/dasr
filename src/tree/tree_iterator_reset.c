#include "tree/tree_private.h"

void dast_tree_iterator_reset(void* self)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)self;
    if (tree_iterator->reversed)
    {
        tree_iterator->curr = dast_knot_max(tree_iterator->tree->root);
    }
    else
    {
        tree_iterator->curr = dast_knot_min(tree_iterator->tree->root);
    }
}
