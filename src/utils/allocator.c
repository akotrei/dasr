#include "utils/allocator.h"

#include <stdlib.h>

static void* _allocate(void* self, unsigned long size)
{
    return malloc(size);
}

static void* _reallocate(void* self, void* ptr, unsigned long size)
{
    return realloc(ptr, size);

}

static void _deallocate(void* self, void* ptr)
{
    free(ptr);
}

unsigned long dast_allocator_sizeof()
{
    return sizeof(dast_allocator_t);
}

dast_allocator_t* dast_allocator_init(void* memory)
{
    dast_allocator_t *allocator;
    allocator = (dast_allocator_t*)memory;

    allocator->allocate = _allocate;
    allocator->reallocate = _reallocate;
    allocator->deallocate = _deallocate;

    return allocator;
}