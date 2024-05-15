#ifndef __DAST_MAP_PRIVATE_H__
#define __DAST_MAP_PRIVATE_H__

#include "map/map.h"

#define DAST_MAP_CAPACITY_GROW_FACTOR 1 /* power of 2 value; e.g. 2 equalent to grow in 4 times */
#define DAST_MAP_INITIAL_CAPACITY 8     /* should be power of 2 */
#define DAST_MAP_MAX_LOAD_FACTOR 1      /* should be not greater than @DAST_MAP_CAPACITY_GROW_FACTOR */
#define DAST_MAP_PERTURB_SHIFT 5
#define DAST_MAP_BAD_SLOT_INDEX UINT64_MAX

typedef struct _dast_slot_t dast_slot_t;

/* memory laoyot of @dast_slot_t type:
 * dast_slot_t* slot -> |--hash--|--key--|--value--|
 */
typedef struct _dast_slot_t
{
    dast_u64_t hash;
} dast_slot_t;

typedef struct _dast_map_t
{
    dast_allocator_t* allocator;
    dast_u64_t*       indices;
    dast_slot_t*      slots;
    dast_u64_t        indices_capacity;
    dast_u64_t        slots_capacity;

    dast_u64_t  elems;
    dast_u64_t  key_size;
    dast_u64_t  val_size;
    dast_cpy_t  cpy_key;
    dast_cpy_t  cpy_val;
    dast_del_t  del_key;
    dast_del_t  del_val;
    dast_hash_t hash_key;
    dast_cmp_t  cmp_key;

} dast_map_t;

#endif /* __DAST_MAP_PRIVATE_H__ */