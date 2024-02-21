#include "array/array_private.h"
#include "utils/mem.h"

void dast_array_copy_to(dast_array_t* array, void* memory, dast_u64_t size)
{
    dast_array_t* dst_array = (dast_array_t*)memory;

    dst_array->data = array->allocator->allocate(array->allocator, array->capacity * array->elem_size);
    dast_cpy_generic(array->data, dst_array->data, array->elems * array->elem_size);

    dst_array->elems = array->elems;
    dst_array->elem_size = array->elem_size;
    dst_array->capacity = array->capacity;
    dst_array->factor = array->factor;
    dst_array->allocator = array->allocator;
    dst_array->cpy = array->cpy;
    dst_array->del = array->del;
}

void dast_array_deepcopy_to(dast_array_t* array, void* memory, dast_u64_t size)
{
    dast_array_t* dst_array = (dast_array_t*)memory;
    dast_u64_t    elem_size = array->elem_size;
    dast_u64_t    i = 0;
    dast_u64_t    end = array->elems;
    dast_cpy_t    cpy = array->cpy;

    char* src_data = array->data;
    char* dst_data = array->allocator->allocate(array->allocator, array->capacity * array->elem_size);
    while (i < end)
    {
        cpy(src_data + i * elem_size, dst_data + i * elem_size, elem_size);
    }
    dst_array->data = dst_data;

    dst_array->elems = array->elems;
    dst_array->elem_size = array->elem_size;
    dst_array->capacity = array->capacity;
    dst_array->factor = array->factor;
    dst_array->allocator = array->allocator;
    dst_array->cpy = array->cpy;
    dst_array->del = array->del;
}

dast_array_t* dast_array_copy(dast_array_t* array)
{
    dast_array_t* new_array = array->allocator->allocate(array->allocator, sizeof(dast_array_t));
    dast_array_copy_to(array, new_array, sizeof(dast_array_t));
    return new_array;
}

dast_array_t* dast_array_deepcopy(dast_array_t* array)
{
    dast_array_t* new_array = array->allocator->allocate(array->allocator, sizeof(dast_array_t));
    dast_array_deepcopy_to(array, new_array, sizeof(dast_array_t));
    return new_array;
}