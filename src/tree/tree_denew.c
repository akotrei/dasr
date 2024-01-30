#include "interface/allocator.h"
#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/mem.h"

void dast_tree_denew(dast_tree_t* tree)
{
    dast_allocator_t* allocator = tree->allocator;
    dast_tree_deinit(tree);
    allocator->deallocate(allocator, tree);
}