#include "tree/tree.h"
#include "tree/tree_private.h"

dast_iterator_t* dast_tree_iterator_new(dast_tree_t* tree, dast_u8_t reversed)
{
    dast_tree_iterator_t* tree_iterator = tree->allocator->allocate(tree->allocator, sizeof(dast_tree_iterator_t));

    tree_iterator->tree = tree;
    if (reversed)
    {
        tree_iterator->curr = dast_knot_max(tree->root);
    }
    else
    {
        tree_iterator->curr = dast_knot_min(tree->root);
    }
    tree_iterator->iterator.next = dast_tree_iterator_next;
    tree_iterator->iterator.prev = dast_tree_iterator_prev;
    tree_iterator->iterator.reset = dast_tree_iterator_reset;
    tree_iterator->reversed = reversed;

    return (dast_iterator_t*)tree_iterator;
}