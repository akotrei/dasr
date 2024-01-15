#ifndef __DAST_IALLOCATOR_H__
#define __DAST_IALLOCATOR_H__

typedef struct _dast_iallocator_t
{
    void* (*allocate)(void* self, unsigned long size);
    void* (*reallocate)(void* self, void* ptr, unsigned long size);
    void (*deallocate)(void* self, void* ptr);
} dast_iallocator_t;

#endif /* __DAST_IALLOCATOR_H__ */