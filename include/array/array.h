/* @dast_array_t is a dynamic data structure for storing any elements in a
 * contiguous memory space and is able to expand it's capacity by strategy
 * capacity = capacity * factor
 */

#ifndef __DAST_ARRAY_H__
#define __DAST_ARRAY_H__

#include "interface/allocator.h"
#include "ntype.h"
#include "utils/mem.h"

typedef struct _dast_array_t dast_array_t;

/* Returns number of bytes needed that an array instance occupies */
dast_u64_t dast_array_sizeof();

/* Creates an array instance on @memory
 * (it should point to a memory block that >= x bytes, where x = @dast_array_sizeof())
 *
 * @obj_size - size (in bytes) of objects that the array should hold
 * @initial_capacity - initial size of the array, should be a positive value
 * @factor - increment factor, by which the capacity of the array will be increased when more memory is needed
 * @allocator - an allocator instance to manage all memory tasks inside the array
 * @cpy - pointer to a function that is responsible for copying objects that are being added to the array
 * or when deep copying of the array occurs
 * @del - pointer to a function that is responsible for deleting objects that are erased from the array
 * or when the array going to be deleted
 */
dast_array_t* dast_array_init_on(void*             memory,
                                 dast_u64_t        obj_size,
                                 dast_allocator_t* allocator,
                                 dast_cpy_t        cpy,
                                 dast_del_t        del);

/* Does the same as @dast_array_init but preallocate memory for itself using @allocator */
dast_array_t* dast_array_init(dast_u64_t obj_size, dast_allocator_t* allocator, dast_cpy_t cpy, dast_del_t del);

/* Deletes the array instance, should be used for such array that was created via @dast_array_init
 *
 * Notes: invokes @del function from @dast_array_init for each objected in the array
 */
void dast_array_destroy_from(dast_array_t* array);

/* Does the same as @dast_array_deinit but shoukd be used if the array was created via @dast_array_new*/
void dast_array_destroy(dast_array_t* array);

// Frees unused memory
void dast_array_shrink(dast_array_t* array);

// Increase capacity to @size, if success returns 1,
// otherwise 0 (e.g. there is already more than @size elements)
dast_u8_t dast_array_reserve(dast_array_t* array, dast_u64_t size);

void          dast_array_copy_to(dast_array_t* array, void* memory, dast_u64_t size);
void          dast_array_deepcopy_to(dast_array_t* array, void* memory, dast_u64_t size);
dast_array_t* dast_array_copy(dast_array_t* array);
dast_array_t* dast_array_deepcopy(dast_array_t* array);

dast_u64_t dast_array_size(dast_array_t* array);
dast_u64_t dast_array_obj_size(dast_array_t* array);

void  dast_array_clear(dast_array_t* array);
void* dast_array_begin(dast_array_t* array);
void* dast_array_end(dast_array_t* array);
void* dast_array_index(dast_array_t* array, dast_u64_t index);

void dast_array_append(dast_array_t* array, void* obj);
void dast_array_insert(dast_array_t* array, void* obj, dast_u64_t index);
void dast_array_extend(dast_array_t* array, void* objs, dast_u64_t n);

dast_u8_t dast_array_pop(dast_array_t* array, void* dst);
dast_u8_t dast_array_remove(dast_array_t* array, dast_u64_t index, void* dst);
dast_u8_t dast_array_replace(dast_array_t* array, void* obj, dast_u64_t index);

void dast_array_reverse(dast_array_t* array);

#endif /* __DAST_ARRAY_H__ */