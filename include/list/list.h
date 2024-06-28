#ifndef __DAST_LIST_H__
#define __DAST_LIST_H__

#include "interface/allocator.h"
#include "interface/iterator.h"
#include "ntype.h"
#include "utils/mem.h"

typedef struct _dast_list_t dast_list_t;

/* Returns number of bytes that a @dast_list_t instance needs*/
dast_u64_t       dast_list_sizeof();
dast_list_t*     dast_list_init_on(void*             memory,
                                   dast_allocator_t* allocator,
                                   dast_u64_t        obj_size,
                                   dast_cpy_t        cpy,
                                   dast_del_t        del);
dast_list_t*     dast_list_init(dast_allocator_t* allocator, dast_u64_t obj_size, dast_cpy_t cpy, dast_del_t del);
void             dast_list_destroy_from(void* list);
void             dast_list_destroy(dast_list_t* list);
void             dast_list_copy_to(void* list, void* memory, dast_u64_t size);
void             dast_list_deepcopy_to(void* list, void* memory, dast_u64_t size);
dast_list_t*     dast_list_copy(dast_list_t* list, dast_allocator_t* allocator);
dast_list_t*     dast_list_deepcopy(dast_list_t* list, dast_allocator_t* allocator);
dast_u64_t       dast_list_size(dast_list_t* list);
dast_u64_t       dast_list_obj_size(dast_list_t* list);
void             dast_list_reverse(dast_list_t* list);
void             dast_list_clear(dast_list_t* list);
void*            dast_list_front(dast_list_t* list);
void*            dast_list_back(dast_list_t* list);
void             dast_list_append(dast_list_t* list, void* obj);
void             dast_list_prepend(dast_list_t* list, void* obj);
dast_u8_t        dast_list_replace_front(dast_list_t* list, void* obj);
dast_u8_t        dast_list_replace_back(dast_list_t* list, void* obj);
dast_u8_t        dast_list_remove_front(dast_list_t* list, void* dst);
dast_u8_t        dast_list_remove_back(dast_list_t* list, void* dst);
dast_iterator_t* dast_list_iterator_new(dast_list_t* list, dast_u8_t reversed);
void             dast_list_iterator_delete(dast_iterator_t* iterator);
dast_u8_t        dast_list_insert_before(dast_iterator_t* iterator, void* obj);
dast_u8_t        dast_list_insert_after(dast_iterator_t* iterator, void* obj);
dast_u8_t        dast_list_replace(dast_iterator_t* iterator, void* obj);
dast_u8_t        dast_list_remove(dast_iterator_t* iterator, void* dst, dast_u8_t move_backward);


#endif /* __DAST_LIST_H__ */
