#ifndef __DAST_ARRAY_PRIVATE_H__
#define __DAST_ARRAY_PRIVATE_H__

#include "array/array.h"
#include "interface/allocator.h"

typedef struct _dast_array_t
{
    char*             data;
    dast_u64_t        elems;
    dast_u64_t        elem_size;
    dast_u64_t        capacity;
    dast_f64_t        factor;
    dast_allocator_t* allocator;

    dast_cpy_t cpy;
    dast_del_t del;
} dast_array_t;

#endif /* __DAST_ARRAY_PRIVATE_H__ */