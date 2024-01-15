#ifndef __DAST_ARRAY_PRIVATE_H__
#define __DAST_ARRAY_PRIVATE_H__

#include "interface/iallocator.h"

typedef struct _dast_array_t
{
    void*             data;
    unsigned long     elems;
    unsigned long     capacity;
    float             factor;
    dast_iallocator_t* allocator;

    void* (*cpy_f)(void* obj, void* memory);
    void (*del_f)(void* obj);
} _dast_array_t;

#endif /* __DAST_ARRAY_PRIVATE_H__ */