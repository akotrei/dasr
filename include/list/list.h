#ifndef __DAST_LIST_H__
#define __DAST_LIST_H__

#include "interface/iallocator.h"

typedef struct _dast_list_t dast_list_t;

/* Returns number of bytes that a @dast_list_t instance needs*/
unsigned long dast_list_sizeof();

dast_list_t* dast_list_init(void*             memory,
                            dast_allocator_t* allocator,
                            unsigned long     obj_size,
                            void (*cpy_f)(void* obj, void* memory),
                            void (*del_f)(void* obj));

dast_list_t* dast_list_new(dast_allocator_t* allocator,
                           unsigned long     obj_size,
                           void (*cpy_f)(void* obj, void* memory),
                           void (*del_f)(void* obj));

dast_list_t dast_array_copy(dast_list_t* list, void* memory);

dast_list_t* dast_array_clone(dast_list_t* list, dast_allocator_t* allocator);

dast_list_t dast_array_deepcopy(dast_list_t* list, void* memory);

dast_list_t* dast_array_deepclone(dast_list_t*      list,
                                  dast_allocator_t* allocator);

void dast_list_release(dast_list_t* list);

void dast_list_delete(dast_list_t* list);

unsigned long dast_list_length(dast_list_t* list);

void dast_list_append(dast_list_t* list, void* obj);

void dast_list_prepend(dast_list_t* list, void* obj);

void dast_list_clear(dast_list_t* list);

void dast_list_insert(dast_list_t* list, void* obj, unsigned long index);

void* dast_list_pop(dast_list_t* list, void* memory);

void* dast_list_popleft(dast_list_t* list, void* memory);

int dast_list_remove(dast_list_t* list,
                     void*        obj,
                     int (*cmp_f)(void* l, void* r));

void dast_list_reverse(dast_list_t* list);

#endif /* __DAST_LIST_H__ */