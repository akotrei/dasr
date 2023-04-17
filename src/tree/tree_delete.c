#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_free(dast_tree_t* tree)
{

}

void dast_tree_delete(dast_tree_t* tree)
{
    dast_iallocator_t* allocator = tree->allocator;
    dast_tree_free(tree);
    allocator->deallocate(allocator, tree);
}