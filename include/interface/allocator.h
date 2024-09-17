#ifndef __DAST_ALLOCATOR_H__
#define __DAST_ALLOCATOR_H__


typedef struct _dast_allocator_t dast_allocator_t;
typedef struct _dast_allocator_t
{
    void* (*allocate)(void* dast_allocator_t, unsigned long size);
    void* (*reallocate)(void* dast_allocator_t, void* ptr, unsigned long size);
    void (*deallocate)(void* dast_allocator_t, void* ptr);
} dast_allocator_t;

#endif /* __DAST_ALLOCATOR_H__ */