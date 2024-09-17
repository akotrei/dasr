#include "array.h"
#include "utils/mem.h"


void dast_array_init(dast_array_t* array, dast_allocator_t* allocator, int obj_size)
{
    array->data = allocator->allocate(allocator, obj_size * DAST_ARRAY_INIT_SIZE);
    array->elems = 0;
    array->elem_size = obj_size;
    array->capacity = DAST_ARRAY_INIT_SIZE;
    array->allocator = allocator;
}

void dast_array_destroy(dast_array_t* array) { array->allocator->deallocate(array->allocator, array->data); }

void dast_array_shrink(dast_array_t* array)
{
    dast_allocator_t* allocator = array->allocator;
    unsigned long     capacity = array->capacity;
    unsigned long     elems = array->elems;
    int               elem_size = array->elem_size;
    if (capacity == elems)
    {
        return;
    }
    array->capacity = elems < DAST_ARRAY_INIT_SIZE ? DAST_ARRAY_INIT_SIZE : elems;
    array->data = allocator->reallocate(allocator, array->data, elem_size * array->capacity);
}

void dast_array_reserve(dast_array_t* array, unsigned long size)
{
    dast_allocator_t* allocator = array->allocator;
    unsigned long     elems = array->elems;
    unsigned long     elem_size = array->elem_size;
    unsigned long     free_slots = array->capacity - elems;

    if (free_slots < size)
    {
        array->capacity = elems + size;
        array->data = allocator->reallocate(allocator, array->data, elem_size * array->capacity);
    }
}


void dast_array_copy(dast_array_t* array, dast_array_t* dst)
{
    dst->data = array->allocator->allocate(array->allocator, array->capacity * array->elem_size);
    dast_cpy_generic(array->data, dst->data, array->elems * array->elem_size);
    dst->elems = array->elems;
    dst->elem_size = array->elem_size;
    dst->capacity = array->capacity;
    dst->allocator = array->allocator;
}

void dast_array_deepcopy(dast_array_t* array, dast_array_t* dst, void (*cpy)(void* src, void* dst))
{
    unsigned long elem_size = array->elem_size;
    unsigned long i = 0;
    unsigned long end = array->elems;

    char* src_data = array->data;
    char* dst_data = array->allocator->allocate(array->allocator, array->capacity * array->elem_size);
    while (i < end)
    {
        cpy(src_data + i * elem_size, dst_data + i * elem_size);
        i++;
    }
    dst->data = dst_data;

    dst->elems = array->elems;
    dst->elem_size = array->elem_size;
    dst->capacity = array->capacity;
    dst->allocator = array->allocator;
}

void dast_array_clear(dast_array_t* array) { array->elems = 0; }

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

void* dast_array_first(dast_array_t* array) { return array->data; }

void* dast_array_last(dast_array_t* array) { return array->data + (array->elems - 1) * array->elem_size; }

void* dast_array_ith(dast_array_t* array, unsigned long index) { return array->data + index * array->elem_size; }

void dast_array_append(dast_array_t* array, void* obj)
{
    dast_allocator_t* allocator = array->allocator;
    if (array->elems == array->capacity)
    {
        array->capacity *= DAST_ARRAY_GROW_FACTOR;
        array->data = allocator->reallocate(allocator, array->data, array->capacity * array->elem_size);
    }
    dast_cpy_generic(obj, array->data + array->elems * array->elem_size, array->elem_size);
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
    dast_allocator_t* allocator = array->allocator;
    unsigned long     elems = array->elems;
    unsigned long     elem_size = array->elem_size;
    if (array->elems == array->capacity)
    {
        array->capacity *= DAST_ARRAY_GROW_FACTOR;
        char* new_data = allocator->allocate(allocator, array->capacity * elem_size);
        dast_cpy_generic(data, new_data, index * elem_size);
        dast_cpy_generic(obj, new_data + index * elem_size, elem_size);
        dast_cpy_generic(data + index * elem_size, new_data + (index + 1) * elem_size, (elems - index) * elem_size);
        allocator->deallocate(allocator, data);
        array->data = new_data;
    }
    else
    {
        dast_move_generic(data + index * elem_size, data + (index + 1) * elem_size, (elems - index) * elem_size);
        dast_cpy_generic(obj, data + index * elem_size, elem_size);
    }
    array->elems++;
}

void dast_array_extend(dast_array_t* array, void* objs, unsigned long n)
{
    dast_allocator_t* allocator = array->allocator;
    if (array->elems + n > array->capacity)
    {
        array->capacity = (array->elems + n) * DAST_ARRAY_GROW_FACTOR;
        array->data = allocator->reallocate(allocator, array->data, array->capacity * array->elem_size);
    }
    dast_cpy_generic(objs, array->data + array->elems * array->elem_size, array->elem_size * n);
    array->elems += n;
}

void dast_array_pop(dast_array_t* array, void* dst)
{
    if (dst)
    {
        dast_cpy_generic(array->data + (array->elems - 1) * array->elem_size, dst, array->elem_size);
    }
    array->elems--;
}

void dast_array_remove(dast_array_t* array, unsigned long index, void* dst)
{
    unsigned long elems = array->elems;
    unsigned long elem_size = array->elem_size;
    unsigned long bytes_to_copy = (elems - 1 - index) * elem_size;
    if (dst)
    {
        dast_cpy_generic(array->data + index * elem_size, dst, elem_size);
    }
    dast_move_generic(array->data + (index + 1) * elem_size, array->data + index * elem_size, bytes_to_copy);
    array->elems--;
}

void dast_array_replace(dast_array_t* array, void* obj, unsigned long index)
{
    dast_cpy_generic(obj, array->data + index * array->elem_size, array->elem_size);
}

void dast_array_reverse(dast_array_t* array)
{
    unsigned long start = 0;
    unsigned long end = array->elems - 1;
    unsigned long elem_size = array->elem_size;
    char*         data = array->data;
    char          tmp[elem_size];
    while (start < end)
    {
        dast_cpy_generic(data + start * elem_size, tmp, elem_size);
        dast_cpy_generic(data + end * elem_size, data + start * elem_size, elem_size);
        dast_cpy_generic(tmp, data + end * elem_size, elem_size);
        start++;
        end--;
    }
}