#ifndef __DAST_MAP_H__
#define __DAST_MAP_H__

#include "interface/allocator.h"
#include "list/list.h"
#include "ntype.h"
#include "utils/cmp.h"
#include "utils/hash.h"
#include "utils/mem.h"

typedef struct _dast_map_t dast_map_t;

/* Returns number of bytes that a @dast_map_t instance needs*/
dast_u64_t dast_map_sizeof();

dast_map_t* dast_map_init_on(void*             memory,
                             dast_allocator_t* allocator,
                             dast_u64_t        key_size,
                             dast_u64_t        val_size,
                             dast_cpy_t        cpy_key,
                             dast_cpy_t        cpy_val,
                             dast_del_t        del_key,
                             dast_del_t        del_val,
                             dast_hash_t       hash_key,
                             dast_cmp_t        cmp_key);
dast_map_t* dast_map_init(dast_allocator_t* allocator,
                          dast_u64_t        key_size,
                          dast_u64_t        val_size,
                          dast_cpy_t        cpy_key,
                          dast_cpy_t        cpy_val,
                          dast_del_t        del_key,
                          dast_del_t        del_val,
                          dast_hash_t       hash_key,
                          dast_cmp_t        cmp_key);

void dast_map_destroy_from(dast_map_t* map);
void dast_map_destroy(dast_map_t* map);

void dast_map_copy(dast_map_t* map, void* memory);
void dast_map_deepcopy_to(dast_map_t* map, void* memory);

dast_map_t* dast_map_clone(dast_map_t* map, dast_allocator_t* allocator);
dast_map_t* dast_map_deepclone(dast_map_t* map, dast_allocator_t* allocator);

dast_u64_t dast_map_size(dast_map_t* map);
dast_u64_t dast_map_keysize(dast_map_t* map);
dast_u64_t dast_map_valsize(dast_map_t* map);
void       dast_map_clear(dast_map_t* map);

void*     dast_map_get(dast_map_t* map, void* key, dast_u8_t get_value);
void      dast_map_set(dast_map_t* map, void* key, void* value);
dast_u8_t dast_map_remove(dast_map_t* map, void* key, void* key_dst, void* val_dst);

dast_iterator_t* dast_map_iterator_new(dast_map_t* map, dast_u8_t reversed, dast_u8_t by_values);
void             dast_map_iterator_delete(dast_iterator_t* iterator);

#endif /* __DAST_MAP_H__ */