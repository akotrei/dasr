#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_iterator_delete(dast_iterator_t* iterator)
{
    dast_tree_iterator_t* iter = (dast_tree_iterator_t*)iterator;
    iter->tree->allocator->deallocate(iter->tree->allocator, iterator);
}