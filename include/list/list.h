#ifndef __DAST_LIST_H__
#define __DAST_LIST_H__

#include "interface/allocator.h"
#include "ntype.h"
#include "cmp.h"

typedef struct _dast_list_t dast_list_t;

/* Returns number of bytes that a @dast_list_t instance needs*/
unsigned long dast_list_sizeof();
dast_list_t*  dast_list_init(void*             memory,
                             dast_allocator_t* allocator,
                             unsigned long     obj_size,
                             void (*cpy_f)(void* obj, void* memory),
                             void (*del_f)(void* obj));
dast_list_t*  dast_list_new(dast_allocator_t* allocator,
                            unsigned long     obj_size,
                            void (*cpy_f)(void* obj, void* memory),
                            void (*del_f)(void* obj));
dast_list_t   dast_list_copy(dast_list_t* list, void* memory);
dast_list_t*  dast_list_clone(dast_list_t* list, dast_allocator_t* allocator);
dast_list_t   dast_list_deepcopy(dast_list_t* list, void* memory);
dast_list_t*  dast_list_deepclone(dast_list_t* list, dast_allocator_t* allocator);
void          dast_list_release(dast_list_t* list);
void          dast_list_delete(dast_list_t* list);

dast_u64_t dast_list_size(dast_list_t* list);
dast_u64_t dast_list_obj_size(dast_list_t* list);
void dast_list_clear(dast_list_t* list);

void dast_list_append(dast_list_t* list, void* obj);
void dast_list_prepend(dast_list_t* list, void* obj);
void dast_list_insert(dast_list_t* list, void* obj, dast_u64_t index);

void* dast_list_front(dast_list_t* list);
void* dast_list_back(dast_list_t* list);
void* dast_list_index(dast_list_t* list, dast_u64_t index);

void dast_list_remove_front(dast_list_t* list);
void dast_list_remove_back(dast_list_t* list);
void dast_list_remove_index(dast_list_t* list, dast_u64_t index);
void dast_list_lremove(dast_list_t* list, void* obj, dast_cmp_t cmp);
void dast_list_rremove(dast_list_t* list, void* obj, dast_cmp_t cmp);

void* dast_list_lfind(dast_list_t* list, dast_cmp_t cmp);
void* dast_list_rfind(dast_list_t* list, dast_cmp_t cmp);

void dast_list_reverse(dast_list_t* list);

#endif /* __DAST_LIST_H__ */
