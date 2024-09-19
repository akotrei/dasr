#ifndef __DAST_MAP_H__
#define __DAST_MAP_H__

#include "interface/allocator.h"
#include "list/list.h"
#include "ntype.h"
#include "utils/cmp.h"
#include "utils/hash.h"
#include "utils/mem.h"


#define DAST_MAP_CAPACITY_GROW_FACTOR 1 // power of 2 value; e.g. 2 equalent to grow in 4 times
#define DAST_MAP_INITIAL_CAPACITY 8     // should be power of 2
#define DAST_MAP_MAX_LOAD_FACTOR 1      // should be not greater than @DAST_MAP_CAPACITY_GROW_FACTOR

// memory laoyot of @dast_slot_t type:
// dast_slot_t* slot -> |--hash--|--index--|--key--|--value--|
typedef struct _dast_slot_t { unsigned long hash; long index; } dast_slot_t;

typedef struct _dast_map_t
{
    dast_allocator_t* allocator;
    long*             indices;
    long              indices_capacity;
    dast_slot_t*      slots;
    long              slots_capacity;
    long              elems;
    long              key_size;
    long              val_size;
    unsigned long     (*hash_f)(void* key);
    int               (*cmp_f)(void* key1, void* key2);

} dast_map_t;


dast_map_t* dast_map_init(
    dast_map_t*       map,
    dast_allocator_t* allocator,
    int               key_size,
    int               val_size,
    unsigned long     (*hash)(void* key),
    int               (*cmp)(void* key1, void* key2)
);
void dast_map_destroy(dast_map_t* map);
void dast_map_copy(dast_map_t* map, dast_map_t* dst);
void dast_map_deepcopy(dast_map_t* map, dast_map_t* dst, (*cpy_key)(void* src, void* dst), (*cpy_val)(void* src, void* dst));
void dast_map_clear(dast_map_t* map);
void dast_map_deepclear(dast_map_t* map, (*del_key)(void* obj), (*del_val)(void* obj));
#define DAST_SLOT_KEY(slot) ((char*)(slot) + sizeof(dast_slot_t))
#define DAST_SLOT_VALUE(map, slot) ((char*)(slot) + sizeof(dast_slot_t) + (map)->key_size)
dast_slot_t* dast_map_get(dast_map_t* map, void* key);
void dast_map_set(dast_map_t* map, void* key, void* value);
void dast_map_del(dast_map_t* map, dast_slot_t* slot);
void* dast_map_cursor(dast_map_t* map);
void* dast_map_next(dast_map_t* map, dast_slot_t *cursor);
void* dast_map_prev(dast_map_t* map, dast_slot_t *cursor);

#endif /* __DAST_MAP_H__ */