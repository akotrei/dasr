/* @dast_array_t is a dynamic data structure for storing any elements in a
 * contiguous memory space and is able to expand it's capacity by strategy
 * capacity = capacity * factor   */

#ifndef __DAST_ARRAY_H__
#define __DAST_ARRAY_H__

#include "interface/iallocator.h"

typedef struct _dast_array_t dast_array_t;

/* Returns number of bytes that a @dast_array_t instance needs*/
unsigned long dast_array_sizeof();

/* Intialises @dast_array_t on @memory (undefined behaviour if @memory points to
 * the memory block occupy number of bytes less than @dast_array_sizeof
 * returns).
 * @obj_size - size of objects to store in bytes, @initial_capacity -
 * number of objects that can be added without reallocation.
 * @factor - parameter
 * in the equation of capacity increasing strategy (capacity = capacity *
 * factor).
 * @allocator - allocator that is being used for internal allocations.
 * @cpy_f - function that is used to copy each object to the array.
 * @del_f - function that is invoked when array is being deleted (when invoking
 * @dast_array_release, @dast_array_delete, @dast_array_pop, and
 * @dast_array_remove).
 */
dast_array_t* dast_array_init(void*             memory,
                              unsigned long     obj_size,
                              unsigned long     initial_capacity,
                              float             factor,
                              dast_allocator_t* allocator,
                              void* (*cpy_f)(void* obj, void* memory),
                              void (*del_f)(void* obj));

/* Does the same as @dast_array_init except using @allocator to allocate memory
 * for self-initialization.
 */
dast_array_t* dast_array_new(unsigned long     obj_size,
                             unsigned long     prealloc_size,
                             float             factor,
                             dast_allocator_t* allocator,
                             void* (*cpy_f)(void* obj, void* memory),
                             void (*del_f)(void* obj));

void dast_array_release(dast_array_t* array);

void dast_array_delete(dast_array_t* array);

void* dast_array_begin(dast_array_t* array);

void* dast_array_end(dast_array_t* array);

void dast_array_insert(dast_array_t* array, void* obj, unsigned long index);

void dast_array_append(dast_array_t* array, void* obj);

void dast_array_clear(dast_array_t* array);

dast_array_t* dast_array_copy(dast_array_t* array, void* memory, int is_deep);

dast_array_t* dast_array_clone(dast_array_t*     array,
                               dast_allocator_t* allocator,
                               int               is_deep);

unsigned long dast_array_count(dast_array_t* array);

void dast_array_extend(dast_array_t* array, void* objs, unsigned long n);

unsigned long dast_array_index(dast_array_t* array,
                               void*         obj,
                               int (*cmp_f)(void* l, void* r));

void* dast_array_pop(dast_array_t* array, unsigned long index, void* memory);

int dast_array_remove(dast_array_t* array,
                      void*         obj,
                      int (*cmp_f)(void* l, void* r));

void dast_array_reverse(dast_array_t* array);

void dast_array_sort(dast_array_t* array, int (*cmp_f)(void* l, void* r));

#endif /* __DAST_ARRAY_H__ */