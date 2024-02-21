#include "array/array_private.h"

dast_array_t* dast_array_init_on(void*             memory,
                                 dast_u64_t        obj_size,
                                 dast_u64_t        prealloc_size,
                                 dast_f32_t        factor,
                                 dast_allocator_t* allocator,
                                 dast_cpy_t        cpy,
                                 dast_del_t        del)
{
    dast_array_t* array = (dast_array_t*)memory;
    array->data = 0;
    array->elems = 0;
    array->elem_size = obj_size;
    array->capacity = prealloc_size;
    array->factor = factor;
    array->allocator = allocator;
    array->cpy = cpy;
    array->del = del;
    return array;
}

dast_array_t* dast_array_init(dast_u64_t        obj_size,
                              dast_u64_t        prealloc_size,
                              dast_f32_t        factor,
                              dast_allocator_t* allocator,
                              dast_cpy_t        cpy,
                              dast_del_t        del)
{
    dast_array_t* array = allocator->allocate(allocator, sizeof(dast_array_t));
    array = dast_array_init_on(array, obj_size, prealloc_size, factor, allocator, cpy, del);
    return array;
}