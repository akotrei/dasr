#include "array/array_private.h"

void dast_array_destroy_from(dast_array_t* array)
{
    char*      data = array->data;
    dast_u64_t obj_size = array->elem_size;
    dast_del_t del = array->del;
    dast_u64_t i = 0;
    dast_u64_t end = array->elems;
    while (i < end)
    {
        del(data + i * obj_size);
        i++;
    }
    array->allocator->deallocate(array->allocator, data);
}

void dast_array_destroy(dast_array_t* array)
{
    dast_allocator_t* allocator = array->allocator;
    dast_array_destroy_from(array);
    allocator->deallocate(allocator, array);
}