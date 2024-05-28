#include "utils/allocator_std.h"
#include "ntype.h"

#include <stdlib.h>

static void* allocate_std(void* self, dast_u64_t size) { return malloc(size); }
static void* reallocate_std(void* self, void* ptr, dast_u64_t size) { return realloc(ptr, size); }
static void  deallocate_std(void* self, void* ptr) { free(ptr); }

dast_u64_t dast_allocator_std_sizeof() { return sizeof(dast_allocator_t); }

dast_allocator_t* dast_allocator_std_init(void* memory)
{
    dast_allocator_t* allocator;
    allocator = (dast_allocator_t*)memory;

    allocator->allocate = allocate_std;
    allocator->reallocate = reallocate_std;
    allocator->deallocate = deallocate_std;

    return allocator;
}