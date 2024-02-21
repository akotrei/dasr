#include "array/array_private.h"
#include "utils/mem.h"

void dast_array_clear(dast_array_t* array)
{
    dast_u64_t end = array->elems;
    dast_u64_t elem_size = array->elem_size;
    dast_del_t del = array->del;
    char*      data = array->data;
    dast_u64_t i = 0;
    while (i < end)
    {
        del(data + i * elem_size);
    }
    array->elems = 0;
}

void* dast_array_begin(dast_array_t* array) { return array->data; }

void* dast_array_end(dast_array_t* array) { return ((char*)array->data) + (array->elems + 1) * array->elem_size; }

void* dast_array_index(dast_array_t* array, dast_u64_t index) { return array->data + index * array->elem_size; }

void dast_array_append(dast_array_t* array, void* obj)
{
    if (array->elems == array->capacity)
    {
        array->capacity *= array->factor;
        array->data = array->allocator->reallocate(array->allocator, array->data, array->capacity * array->elem_size);
    }
    array->cpy(obj, array->data + array->elems * array->elem_size, array->elem_size);
    array->elems++;
}

void dast_array_insert(dast_array_t* array, void* obj, dast_u64_t index)
{
    if (index >= array->elems)
    {
        dast_array_append(array, obj);
        return;
    }

    char*      data = array->data;
    dast_u64_t elems = array->elems;
    dast_u64_t elem_size = array->elem_size;
    if (array->elems == array->capacity)
    {
        array->capacity++;
        char* new_data = array->allocator->allocate(array->allocator, array->capacity * elem_size);
        dast_cpy_generic(data + index * elem_size, new_data + (index + 1) * elem_size, (elems - index) * elem_size);
        array->cpy(obj, new_data + index * elem_size, elem_size);
        dast_cpy_generic(data, new_data, index * elem_size);
        array->allocator->deallocate(array->allocator, data);
        array->data = new_data;
    }
    else
    {
        char* tmp = array->allocator->allocate(array->allocator, (elems - index) * elem_size);
        dast_cpy_generic(data + index * elem_size, tmp, (elems - index) * elem_size);
        array->cpy(obj, data + index * elem_size, elem_size);
        dast_cpy_generic(tmp, data + (index + 1) * elem_size, (elems - index) * elem_size);
        array->allocator->deallocate(array->allocator, tmp);
    }
    array->elems++;
}

void dast_array_extend(dast_array_t* array, void* objs, dast_u64_t n)
{
    char*      data = array->data;
    dast_u64_t elems = array->elems;
    dast_u64_t elem_size = array->elem_size;
    dast_u64_t i = 0;
    if (elems + n > array->capacity)
    {
        array->capacity = (elems + n) * array->factor;
        array->data = array->allocator->reallocate(array->allocator, data, array->capacity * elem_size);
    }
    while (i < n)
    {
        array->cpy((char*)objs + i * elem_size, data + (i + elems) * elem_size, elem_size);
        i--;
    }
    array->elem_size += n;
}

dast_u8_t dast_array_pop_to(dast_array_t* array, void* dst)
{
    if (array->elems == 0)
    {
        return 0;
    }
    array->cpy(array->data + (array->elems - 1) * array->elem_size, dst, array->elem_size);
    array->del(array->data + (array->elems - 1) * array->elem_size);
    array->elems--;
    return 1;
}

dast_u8_t dast_array_pop(dast_array_t* array)
{
    if (array->elems == 0)
    {
        return 0;
    }
    array->del(array->data + (array->elems - 1) * array->elem_size);
    array->elems--;
    return 1;
}

dast_u8_t dast_array_remove_to(dast_array_t* array, dast_u64_t index, void* dst)
{
    if (index >= array->elems)
    {
        return 0;
    }
    dast_u64_t elems = array->elems;
    dast_u64_t elem_size = array->elem_size;
    char*      tmp = array->allocator->allocate(array->allocator, (elems - index + 1) * elem_size);
    array->cpy(array->data + index * elem_size, dst, elem_size);
    array->del(array->data + index * elem_size);
    dast_cpy_generic(array->data + (index + 1) * elem_size, tmp, (elems - index + 1) * elem_size);
    dast_cpy_generic(tmp, array->data + index * elem_size, (elems - index + 1) * elem_size);
    array->allocator->deallocate(array->allocator, tmp);
    array->elems--;
    return 1;
}

dast_u8_t dast_array_remove(dast_array_t* array, dast_u64_t index)
{
    if (index >= array->elems)
    {
        return 0;
    }
    dast_u64_t elems = array->elems;
    dast_u64_t elem_size = array->elem_size;
    char*      tmp = array->allocator->allocate(array->allocator, (elems - index + 1) * elem_size);
    array->del(array->data + index * elem_size);
    dast_cpy_generic(array->data + (index + 1) * elem_size, tmp, (elems - index + 1) * elem_size);
    dast_cpy_generic(tmp, array->data + index * elem_size, (elems - index + 1) * elem_size);
    array->allocator->deallocate(array->allocator, tmp);
    array->elems--;
    return 1;
}

void dast_array_reverse(dast_array_t* array)
{
    dast_u64_t start = 0;
    dast_u64_t end = array->elems - 1;
    dast_u64_t elem_size = array->elem_size;
    char* data = array->data;
    char tmp[elem_size];
    while (start < end)
    {
        dast_cpy_generic(data + start*elem_size, tmp, elem_size);
        dast_cpy_generic(data + end*elem_size, data + start*elem_size, elem_size);
        dast_cpy_generic(tmp, data + end*elem_size, elem_size);
        start++;
        end--;
    }
}