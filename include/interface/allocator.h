#ifndef __DAST_ALLOCATOR_H__
#define __DAST_ALLOCATOR_H__

#include "ntype.h"

typedef struct _dast_allocator_t dast_allocator_t;

typedef struct _dast_allocator_t
{
    void* (*allocate)(void* dast_allocator_t, dast_u64_t size);
    void* (*reallocate)(void* dast_allocator_t, void* ptr, dast_u64_t size);
    void (*deallocate)(void* dast_allocator_t, void* ptr);
} dast_allocator_t;

#endif /* __DAST_ALLOCATOR_H__ */