#ifndef __DAST_IALLOCATOR_H__
#define __DAST_IALLOCATOR_H__

#include "ntype.h"

typedef struct _dast_allocator_t
{
    void* (*allocate)(void* self, dast_u64_t size);
    void* (*reallocate)(void* self, void* ptr, dast_u64_t size);
    void (*deallocate)(void* self, void* ptr);
} dast_allocator_t;

#endif /* __DAST_IALLOCATOR_H__ */