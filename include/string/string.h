#ifndef __DAST_STRING_H__
#define __DAST_STRING_H__

#include "interface/iallocator.h"

typedef struct _dast_string_t dast_string_t;

/* Returns number of bytes that a @dast_string_t instance needs*/
unsigned long dast_string_sizeof();

dast_string_t* dast_string_init(void*             memory,
                            dast_allocator_t* allocator,
                            char* c_string,
                            void (*del_f)(void* obj));

dast_string_t* dast_string_new(dast_allocator_t* allocator,
                           char* c_string,
                           void (*del_f)(void* obj));

dast_string_t dast_array_copy(dast_string_t* list, void* memory);

dast_string_t* dast_array_clone(dast_string_t* list, dast_allocator_t* allocator);

dast_string_t dast_array_deepcopy(dast_string_t* list, void* memory);

dast_string_t* dast_array_deepclone(dast_string_t*      list,
                                  dast_allocator_t* allocator);

void dast_list_release(dast_string_t* list);

void dast_list_delete(dast_string_t* list);

unsigned long dast_list_length(dast_string_t* list);

void dast_list_append(dast_string_t* list, void* obj);

void dast_list_prepend(dast_string_t* list, void* obj);

void dast_list_clear(dast_string_t* list);

void dast_list_insert(dast_string_t* list, void* obj, unsigned long index);

void* dast_list_pop(dast_string_t* list, void* memory);

void* dast_list_popleft(dast_string_t* list, void* memory);

int dast_list_remove(dast_string_t* list,
                     void*        obj,
                     int (*cmp_f)(void* l, void* r));

void dast_list_reverse(dast_string_t* list);

#endif /* __DAST_STRING_H__ */
