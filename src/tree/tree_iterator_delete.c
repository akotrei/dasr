#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_iterator_delete(dast_iterator_t* iterator)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)iterator;
    dast_allocator_t*     allocator = tree_iterator->tree->allocator;
    allocator->deallocate(allocator, iterator);
}