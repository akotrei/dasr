#include "tree/tree.h"

#include <stdio.h>

int main(int argc, char** arcv)
{
    printf("---\n");
    unsigned long tree_size = dast_tree_sizeof();
    char mem[tree_size];
    printf("alloc_size: %lu\n", tree_size);

    return 0;
}