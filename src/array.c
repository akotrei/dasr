#include "array.h"

void dast_array_init(dast_array_t* array, int obj_size)
{
    array->data = DAST_MALLOC(obj_size * DAST_ARRAY_INIT_SIZE);
    array->elems = 0;
    array->elem_size = obj_size;
    array->capacity = DAST_ARRAY_INIT_SIZE;
}

void dast_array_destroy(dast_array_t* array) { DAST_FREE(array->data); }

void dast_array_shrink(dast_array_t* array)
{
    unsigned long     capacity = array->capacity;
    unsigned long     elems = array->elems;
    int               elem_size = array->elem_size;
    if (capacity == elems)
    {
        return;
    }
    array->capacity = elems < DAST_ARRAY_INIT_SIZE ? DAST_ARRAY_INIT_SIZE : elems;
    array->data = DAST_REALLOC(array->data, elem_size * array->capacity);
}

void dast_array_reserve(dast_array_t* array, unsigned long size)
{
    unsigned long     elems = array->elems;
    unsigned long     elem_size = array->elem_size;
    unsigned long     free_slots = array->capacity - elems;

    if (free_slots < size)
    {
        array->capacity = elems + size;
        array->data = DAST_REALLOC(array->data, elem_size * array->capacity);
    }
}


void dast_array_copy(dast_array_t* array, dast_array_t* dst)
{
    dst->data = DAST_MALLOC(array->capacity * array->elem_size);
    DAST_MEMCPY(dst->data, array->data, array->elems * array->elem_size);
    dst->elems = array->elems;
    dst->elem_size = array->elem_size;
    dst->capacity = array->capacity;
}

void dast_array_deepcopy(dast_array_t* array, dast_array_t* dst, void (*cpy)(void* src, void* dst))
{
    unsigned long elem_size = array->elem_size;
    unsigned long i = 0;
    unsigned long end = array->elems;

    char* src_data = array->data;
    char* dst_data = DAST_MALLOC(array->capacity * array->elem_size);
    while (i < end)
    {
        cpy(src_data + i * elem_size, dst_data + i * elem_size);
        i++;
    }
    dst->data = dst_data;

    dst->elems = array->elems;
    dst->elem_size = array->elem_size;
    dst->capacity = array->capacity;
}

void dast_array_deepclear(dast_array_t* array, void (*del)(void* obj))
{
    unsigned long end = array->elems;
    unsigned long elem_size = array->elem_size;
    char*         data = array->data;
    unsigned long i = 0;
    while (i < end)
    {
        del(data + i * elem_size);
        i++;
    }
    array->elems = 0;
}

void dast_array_append(dast_array_t* array, void* obj)
{
    if (array->elems == array->capacity)
    {
        array->capacity *= DAST_ARRAY_GROW_FACTOR;
        array->data = DAST_REALLOC(array->data, array->capacity * array->elem_size);
    }
    DAST_MEMCPY(array->data + array->elems * array->elem_size, obj, array->elem_size);
    array->elems++;
}

void dast_array_insert(dast_array_t* array, void* obj, unsigned long index)
{
    if (index >= array->elems)
    {
        dast_array_append(array, obj);
        return;
    }

    char*             data = array->data;
    unsigned long     elems = array->elems;
    unsigned long     elem_size = array->elem_size;
    if (array->elems == array->capacity)
    {
        array->capacity *= DAST_ARRAY_GROW_FACTOR;
        char* new_data = DAST_MALLOC(array->capacity * elem_size);
        DAST_MEMCPY(new_data, data, index * elem_size);
        DAST_MEMCPY(new_data + index * elem_size, obj, elem_size);
        DAST_MEMCPY(new_data + (index + 1) * elem_size, data + index * elem_size, (elems - index) * elem_size);
        DAST_FREE(data);
        array->data = new_data;
    }
    else
    {
        DAST_MEMMOVE(data + (index + 1) * elem_size, data + index * elem_size, (elems - index) * elem_size);
        DAST_MEMCPY(data + index * elem_size, obj, elem_size);
    }
    array->elems++;
}

void dast_array_extend(dast_array_t* array, void* objs, unsigned long n)
{
    if (array->elems + n > array->capacity)
    {
        array->capacity = (array->elems + n) * DAST_ARRAY_GROW_FACTOR;
        array->data = DAST_REALLOC(array->data, array->capacity * array->elem_size);
    }
    DAST_MEMCPY(array->data + array->elems * array->elem_size, objs, array->elem_size * n);
    array->elems += n;
}

void dast_array_remove(dast_array_t* array, unsigned long index, void* dst)
{
    unsigned long elems = array->elems;
    unsigned long elem_size = array->elem_size;
    unsigned long bytes_to_copy = (elems - 1 - index) * elem_size;
    if (dst)
    {
        DAST_MEMCPY(dst, array->data + index * elem_size, elem_size);
    }
    DAST_MEMMOVE(array->data + index * elem_size, array->data + (index + 1) * elem_size, bytes_to_copy);
    array->elems--;
}

void dast_array_reverse(dast_array_t* array)
{
    unsigned long start = 0;
    unsigned long end = array->elems - 1;
    unsigned long elem_size = array->elem_size;
    char*         data = array->data;
    char*         tmp = DAST_MALLOC(elem_size);
    while (start < end)
    {
        DAST_MEMCPY(tmp, data + start * elem_size, elem_size);
        DAST_MEMCPY(data + start * elem_size, data + end * elem_size, elem_size);
        DAST_MEMCPY(data + end * elem_size, tmp, elem_size);
        start++;
        end--;
    }
}