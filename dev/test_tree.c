#include "tree/tree.h"
#include "utils/allocator.h"

#include <stdio.h>
#include "debug.h"

int cmp(void* o1, void* o2)
{
    return *(int*)o1 - *(int*)o2;
}

void cpy(void* o, void* dst)
{
    *(int*)dst = *(int*)o;
}

void del(void* o)
{
    printf("o: %d\n", *(int*)o);
}

int main(int argc, char** arcv)
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    printf("---\n");
    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];
    printf("alloc_size: %lu\n", tree_size);

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add

    dast_tree_free(tree);

    return 0;
}