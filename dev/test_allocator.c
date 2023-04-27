#include "utils/allocator.h"

#include <stdio.h>

int main(int argc, char** arcv)
{
    printf("---\n");
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem);
    printf("alloc_size: %lu\n", alloc_size);
    printf("mem: %p\n", mem);
    printf("allocator: %p\n", allocator);

    void* m = allocator->allocate(NULL, 1000);
    allocator->deallocate(NULL, m);

    return 0;
}