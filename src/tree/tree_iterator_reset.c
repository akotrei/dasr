#include "tree/tree_private.h"

void dast_tree_iterator_reset(void* self)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)self;
    tree_iterator->curr = tree_iterator->tree->root;
}
