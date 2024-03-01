#ifndef __DAST_SET_H__
#define __DAST_SET_H__

#include "interface/allocator.h"
#include "list/list.h"

typedef struct _dast_set_t dast_set_t;

/* Returns number of bytes that a @dast_set_t instance needs*/
unsigned long dast_set_sizeof();
dast_set_t*   dast_set_init(void*              memory,
                            dast_allocator_t* allocator,
                            unsigned long      obj_size,
                            unsigned long(hash_f)(void* key),
                            int (*cmp_f)(void* l, void* r),
                            void (*cpy_f)(void* obj, void* memory),
                            void (*del_f)(void* obj));
dast_set_t*   dast_set_new(dast_allocator_t* allocator,
                           unsigned long      obj_size,
                           unsigned long(hash_f)(void* key),
                           int (*cmp_f)(void* l, void* r),
                           void (*cpy_f)(void* obj, void* memory),
                           void (*del_f)(void* obj));
void          dast_set_release(dast_set_t* set);
void          dast_set_delete(dast_set_t* set);
dast_set_t    dast_set_copy(dast_set_t* set, void* memory);
dast_set_t*   dast_set_clone(dast_set_t* set, dast_allocator_t* allocator);
dast_set_t    dast_set_deepcopy(dast_set_t* set, void* memory);
dast_set_t*   dast_set_deepclone(dast_set_t* set, dast_allocator_t* allocator);
void          dast_set_clear(dast_set_t* set);
unsigned long dast_set_length(dast_set_t* set);

dast_list_t* dast_set_elements(dast_set_t* set);
void*        dast_set_pop(dast_set_t* set, void* key, void* memory);
int          dast_set_remove(dast_set_t* set, void* key);
void         dast_set_add(dast_set_t* set, void* key);
void         dast_set_join(dast_set_t* set, dast_set_t* another);
void         dast_set_intersect(dast_set_t* set, dast_set_t* another);
void         dast_set_diff(dast_set_t* set, dast_set_t* another);
int          dast_set_is_disjoint(dast_set_t* set_left, dast_set_t* set_right);
int          dast_set_is_subset(dast_set_t* set, dast_set_t* another);
int          dast_set_is_superset(dast_set_t* set, dast_set_t* another);

#endif /* __DAST_SET_H__ */