/// @dast_array_t is a dynamic data structure for storing any elements in a
/// contiguous memory space and is able to expand it's capacity by strategy
/// capacity = capacity * factor that allows to append elemnts for O(1)

#ifndef __DAST_ARRAY_H__
#define __DAST_ARRAY_H__

#include "memory.h"

#define DAST_ARRAY_GROW_FACTOR 2.0f
#define DAST_ARRAY_INIT_SIZE 8

typedef struct _dast_array_t
{
    char*         data;
    unsigned long elems;
    unsigned long capacity;
    int           elem_size;
} dast_array_t;

// Initialize an array instance on @array
// @obj_size - size (in bytes) of objects that the array should hold
// @allocator - an allocator instance to manage all memory tasks inside @array
void dast_array_init(dast_array_t* array, int obj_size);

// Destroy @array and free memory that @array uses
void dast_array_destroy(dast_array_t* array);

// Frees unused memory: make array's capacity to be equal of it's size
void dast_array_shrink(dast_array_t* array);

// Reserve @size elements. If @size <= capacity - elements in array does nothing
void dast_array_reserve(dast_array_t* array, unsigned long size);

// Copy @array to @dst. Note: it is a shallow copying
// Note: @dst supposed to be not initialized
void dast_array_copy(dast_array_t* array, dast_array_t* dst);

// Copy @array to @dst. Also it copies all objcets too (invokes @cpy for each object)
// Note: @dst supposed to be not initialized
void dast_array_deepcopy(dast_array_t* array, dast_array_t* dst, void (*cpy)(void* src, void* dst));

// remove all objects from @array. Note: capacity remains the same
#define DAST_ARRAY_CLEAR(array) ((array)->elems = 0)

// remove all objects from @array with invoking @del on each object. Note: capacity remains the same
void dast_array_deepclear(dast_array_t* array, void (*del)(void* obj));

// get first element
#define DAST_ARRAY_FIRST(array) ((void*)((array)->data))

// get last element
#define DAST_ARRAY_LAST(array) ((void*)((array)->data + ((array)->elems - 1) * (array)->elem_size))

// get i'th element
#define DAST_ARRAY_ITH(array, index) ((void*)((array)->data + (index) * (array)->elem_size))

// add @obj to the end of @array
void dast_array_append(dast_array_t* array, void* obj);

// insert @obj at @index position in @array
void dast_array_insert(dast_array_t* array, void* obj, unsigned long index);

// append @n @objs to the end of @array
void dast_array_extend(dast_array_t* array, void* objs, unsigned long n);

// remove last object to @dst from @array. @dst can be NULL then nothing will copy to @dst
#define DAST_ARRAY_POP(array) ((array)->elems--)

// remove object at a position @index to @dst from @array. @dst can be NULL then nothing will copy to @dst
void dast_array_remove(dast_array_t* array, unsigned long index, void* dst);

// replace an object in @array at the position @index by @obj
#define DAST_ARRAY_REPLACE(array, obj, index) (DAST_MEMMOVE((array)->data + (index) * (array)->elem_size, (obj), (array)->elem_size))

// reverse elements in @array (a[0] swapped with a[n-1], a[1] swapped with a[n-2], ...)
// where n - number of elements in @array
void dast_array_reverse(dast_array_t* array);

#endif // __DAST_ARRAY_H__