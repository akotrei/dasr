#include "tree/tree.h"
#include "tree/tree_private.h"

dast_iterator_t* dast_tree_iterator_new(dast_tree_t* tree)
{
    dast_knot_t**         knot_pivot = &(tree->root);
    dast_knot_t*          knot = 0;
    dast_tree_iterator_t* tree_iterator = tree->allocator->allocate(tree->allocator, sizeof(dast_tree_iterator_t));
    
    tree_iterator->iterator.next = dast_tree_iterator_next;
    tree_iterator->iterator.reset = dast_tree_iterator_reset;

    while (*knot_pivot)
    {
        knot = *knot_pivot;
        *knot_pivot = knot->left;
    }

    tree_iterator->knot = knot;
    tree_iterator->last_visited = LEFT;
    return (dast_iterator_t*)tree_iterator;
}