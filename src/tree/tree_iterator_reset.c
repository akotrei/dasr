#include "tree/tree_private.h"

void dast_tree_iterator_reset(dast_iterator_t* iterator)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)iterator;
    if (tree_iterator->reversed)
    {
        tree_iterator->curr = tree_iterator->end;
    }
    else
    {
        tree_iterator->curr = tree_iterator->start;
    }
}
