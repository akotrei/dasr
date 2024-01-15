#ifndef __DAST_TABLE_H__
#define __DAST_TABLE_H__

#include "interface/iallocator.h"
#include "list/list.h"

typedef struct _dast_table_t dast_table_t;

/* Returns number of bytes that a @dast_table_t instance needs*/
unsigned long dast_table_sizeof();

dast_table_t* dast_table_init(void*             memory,
                              dast_iallocator_t* allocator,
                              unsigned long     key_size,
                              unsigned long     val_size,
                              void (*cpy_key_f)(void* obj, void* memory),
                              void (*cpy_val_f)(void* obj, void* memory),
                              void (*del_key_f)(void* obj),
                              void (*del_val_f)(void* obj),
                              unsigned long(hash_key_f)(void* key),
                              int (*cmp_key_f)(void* l, void* r));

dast_table_t* dast_table_new(dast_iallocator_t* allocator,
                             unsigned long     key_size,
                             unsigned long     val_size,
                             void (*cpy_f)(void* obj, void* memory),
                             void (*cpy_val_f)(void* obj, void* memory),
                             void (*del_key_f)(void* obj),
                             void (*del_val_f)(void* obj),
                             unsigned long(hash_key_f)(void* key),
                             int (*cmp_key_f)(void* l, void* r));

void dast_table_release(dast_table_t* table);

void dast_table_delete(dast_table_t* table);

dast_table_t dast_table_copy(dast_table_t* table, void* memory);

dast_table_t* dast_table_clone(dast_table_t*     table,
                               dast_iallocator_t* allocator);

dast_table_t dast_table_deepcopy(dast_table_t* table, void* memory);

dast_table_t* dast_table_deepclone(dast_table_t*     table,
                                   dast_iallocator_t* allocator);

unsigned long dast_table_length(dast_table_t* table);

void dast_table_clear(dast_table_t* table);

void* dast_table_get(dast_table_t* table, void* key);

void* dast_table_set(dast_table_t* table, void* key, void* value);

dast_list_t* dast_table_items(dast_table_t* table);

dast_list_t* dast_table_keys(dast_table_t* table);

dast_list_t* dast_table_values(dast_table_t* table);

void dast_table_update(dast_table_t* table, dast_table_t* another);

void* dast_table_pop(dast_table_t* table, void* key, void* memory);

int dast_table_remove(dast_table_t* table, void* key);

#endif /* __DAST_TABLE_H__ */