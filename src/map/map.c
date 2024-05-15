#include "map/map_private.h"

static dast_u64_t get_index(dast_map_t* map, void* key)
{
    dast_cmp_t  cmp = map->cmp_key;
    dast_u64_t  hash = map->hash_key(key);
    dast_u64_t  capacity = map->indices_capacity;
    dast_u64_t  key_size = map->key_size;
    dast_u64_t  slot_size = sizeof(dast_slot_t) + key_size + map->val_size;
    dast_u64_t  perturb = hash;
    dast_u64_t  idx = hash % capacity;
    dast_u64_t* indices = map->indices;
    dast_u64_t  slot_idx = indices[idx];
    char*       slots = (char*)(map->slots);

    while (slot_idx != DAST_MAP_BAD_SLOT_INDEX)
    {
        if ((hash == (dast_u64_t)(slots + slot_idx * slot_size)) &&
            cmp(key, slots + slot_idx * slot_size + sizeof(dast_slot_t)))
        {
            return idx;
        }
        perturb >>= DAST_MAP_PERTURB_SHIFT;
        idx = indices[(idx * 5 + perturb + 1) % capacity];
        slot_idx = indices[idx];
    }

    return DAST_MAP_BAD_SLOT_INDEX;
}

static void rebuild_indices(char*       slots,
                            dast_u64_t  slot_idx,
                            dast_u64_t  slot_size,
                            dast_u64_t* new_indices,
                            dast_u64_t  new_capacity)
{
    dast_u64_t hash = *((dast_u64_t*)slots + slot_size * slot_idx);
    dast_u64_t perturb = hash;
    dast_u64_t idx = hash % new_capacity;
    while (new_indices[idx] != DAST_MAP_BAD_SLOT_INDEX)
    {
        perturb >>= DAST_MAP_PERTURB_SHIFT;
        idx = (idx * 5 + perturb + 1) % new_capacity;
    }
    new_indices[idx] = slot_idx;
}

dast_u64_t dast_map_sizeof() { return sizeof(dast_map_t); }

dast_map_t* dast_map_init_on(void*             memory,
                             dast_allocator_t* allocator,
                             dast_u64_t        key_size,
                             dast_u64_t        val_size,
                             dast_cpy_t        cpy_key,
                             dast_cpy_t        cpy_val,
                             dast_del_t        del_key,
                             dast_del_t        del_val,
                             dast_hash_t       hash_key,
                             dast_cmp_t        cmp_key)
{
    dast_u64_t  slot_size = sizeof(dast_slot_t) + key_size + val_size;
    dast_map_t* map = memory;
    map->allocator = allocator;
    map->indices = allocator->allocate(allocator, sizeof(map->indices) * DAST_MAP_INITIAL_CAPACITY);
    dast_mset(map->indices, UINT8_MAX, sizeof(map->indices) * DAST_MAP_INITIAL_CAPACITY);
    map->slots = allocator->allocate(allocator, slot_size * DAST_MAP_INITIAL_CAPACITY);
    map->indices_capacity = DAST_MAP_INITIAL_CAPACITY;
    map->slots_capacity = DAST_MAP_INITIAL_CAPACITY;
    map->elems = 0;
    map->key_size = key_size;
    map->val_size = val_size;
    map->cpy_key = cpy_key;
    map->cpy_val = cpy_val;
    map->del_key = del_key;
    map->del_val = del_val;
    map->hash_key = hash_key;
    map->cmp_key = cmp_key;

    return map;
}

dast_map_t* dast_map_init(dast_allocator_t* allocator,
                          dast_u64_t        key_size,
                          dast_u64_t        val_size,
                          dast_cpy_t        cpy_key,
                          dast_cpy_t        cpy_val,
                          dast_del_t        del_key,
                          dast_del_t        del_val,
                          dast_hash_t       hash_key,
                          dast_cmp_t        cmp_key)
{
    dast_map_t* map = allocator->allocate(allocator, sizeof(dast_map_t));
    map = dast_map_init_on(map, allocator, key_size, val_size, cpy_key, cpy_val, del_key, del_val, hash_key, cmp_key);
    return map;
}

void dast_map_destroy_from(dast_map_t* map)
{
    dast_u64_t        i = 0;
    dast_u64_t        elems = map->elems;
    dast_del_t        del_key = map->del_key;
    dast_del_t        del_val = map->del_val;
    dast_u64_t        key_size = map->key_size;
    dast_u64_t        val_size = map->val_size;
    dast_u64_t        slot_size = sizeof(dast_slot_t) + key_size + val_size;
    dast_allocator_t* allocator = map->allocator;
    char*             slots = (char*)(map->slots);

    while (i < elems)
    {
        del_key(slots + i * slot_size + sizeof(dast_slot_t));
        del_val(slots + i * slot_size + sizeof(dast_slot_t) + key_size);
        i++;
    }

    allocator->deallocate(allocator, slots);
    allocator->deallocate(allocator, map->indices);
}

void dast_map_destroy(dast_map_t* map)
{
    dast_allocator_t* allocator = map->allocator;
    dast_map_destroy_from(map);
    allocator->deallocate(allocator, map);
}

dast_u64_t dast_map_size(dast_map_t* map) { return map->elems; }

dast_u64_t dast_map_keysize(dast_map_t* map) { return map->key_size; }

dast_u64_t dast_map_valsize(dast_map_t* map) { return map->val_size; }

void dast_map_clear(dast_map_t* map)
{
    dast_u64_t  i = 0;
    dast_u64_t  capacity = map->indices_capacity;
    dast_del_t  del_key = map->del_key;
    dast_del_t  del_val = map->del_val;
    dast_u64_t  key_size = map->key_size;
    dast_u64_t  val_size = map->val_size;
    dast_u64_t  slot_size = sizeof(dast_slot_t) + key_size + val_size;
    dast_u64_t* indices = map->indices;
    char*       slots = (char*)(map->slots);

    while (i < capacity)
    {
        if (indices[i] == DAST_MAP_BAD_SLOT_INDEX)
        {
            i++;
            continue;
        }
        del_key(slots + indices[i] * slot_size + sizeof(dast_slot_t));
        del_val(slots + indices[i] * slot_size + sizeof(dast_slot_t) + key_size);
        indices[i] = DAST_MAP_BAD_SLOT_INDEX;
        i++;
    }
    map->elems = 0;
}

void* dast_map_get(dast_map_t* map, void* key, dast_u8_t get_value)
{
    if (map->elems == 0)
    {
        return 0;
    }

    get_value = !!get_value;
    dast_u64_t idx = get_index(map, key);
    dast_u64_t slot_size = sizeof(dast_slot_t) + map->key_size + map->val_size;
    if (idx == DAST_MAP_BAD_SLOT_INDEX)
    {
        return 0;
    }
    return map->slots + map->indices[idx] * slot_size + sizeof(dast_slot_t) + map->key_size * get_value;
}

void dast_map_set(dast_map_t* map, void* key, void* value)
{
    dast_u64_t slot_size = sizeof(dast_slot_t) + map->key_size + map->val_size;

    if (map->elems << DAST_MAP_MAX_LOAD_FACTOR == map->indices_capacity)
    {
        char*       slots = (char*)map->slots;
        dast_u64_t  elems = map->elems;
        dast_u64_t  new_capacity = map->indices_capacity << DAST_MAP_MAX_LOAD_FACTOR;
        dast_u64_t* new_indices = map->allocator->allocate(map->allocator, sizeof(map->indices) * new_capacity);
        dast_mset(new_indices, UINT8_MAX, sizeof(map->indices) * new_capacity);
        dast_u64_t i;
        for (i = 0; i < elems; i++)
        {
            rebuild_indices(slots, i, slot_size, new_indices, new_capacity);
        }
        map->indices = new_indices;
        map->indices_capacity = new_capacity;
    }

    if (map->slots_capacity == map->elems)
    {
        map->slots_capacity <<= DAST_MAP_MAX_LOAD_FACTOR;
        map->slots = map->allocator->reallocate(map->allocator, map->slots, slot_size * map->slots_capacity);
    }

    dast_cmp_t  cmp = map->cmp_key;
    dast_u64_t  hash = map->hash_key(key);
    dast_u64_t  capacity = map->indices_capacity;
    dast_u64_t  perturb = hash;
    dast_u64_t  idx = hash % capacity;
    dast_u64_t* indices = map->indices;
    dast_u64_t  slot_idx = indices[idx];
    char*       slots = (char*)(map->slots);

    while (slot_idx != DAST_MAP_BAD_SLOT_INDEX)
    {
        if ((hash == (dast_u64_t)(slots + slot_idx * slot_size)) &&
            cmp(key, slots + slot_idx * slot_size + sizeof(dast_slot_t)))
        {
            map->del_key(slots + slot_idx * slot_size + sizeof(dast_slot_t));
            map->del_val(slots + slot_idx * slot_size + sizeof(dast_slot_t) + map->key_size);
            map->cpy_key(key, slots + slot_idx * slot_size + sizeof(dast_slot_t), map->key_size);
            map->cpy_key(value, slots + slot_idx * slot_size + sizeof(dast_slot_t) + map->key_size, map->val_size);
            return;
        }
        perturb >>= DAST_MAP_PERTURB_SHIFT;
        idx = (idx * 5 + perturb + 1) % capacity;
        slot_idx = indices[idx];
    }

    indices[idx] = map->elems;
    map->cpy_key(key, slots + map->elems * slot_size + sizeof(dast_slot_t), map->key_size);
    map->cpy_key(value, slots + map->elems * slot_size + sizeof(dast_slot_t) + map->key_size, map->val_size);
    map->elems++;
}

dast_u8_t dast_map_remove(dast_map_t* map, void* key, void* key_dst, void* val_dst)
{
    if (map->elems == 0)
    {
        return 0;
    }

    dast_u64_t idx_to_remove = get_index(map, key);
    if (idx_to_remove == DAST_MAP_BAD_SLOT_INDEX)
    {
        return 0;
    }

    dast_u64_t slot_size = sizeof(dast_slot_t) + map->key_size + map->val_size;
    dast_u64_t idx_last = get_index(map, map->slots + (map->elems - 1) * slot_size + sizeof(dast_slot_t));

    char* to_revome = ((char*)map->slots) + map->indices[idx_to_remove] * slot_size;
    char* last = ((char*)map->slots) + (map->elems - 1) * slot_size;
    map->del_key(to_revome + sizeof(dast_slot_t));
    map->del_val(to_revome + sizeof(dast_slot_t) + map->key_size);

    if (to_revome != last)
    {
        dast_cpy_generic(to_revome, last, slot_size);
    }
    map->indices[idx_last] = idx_to_remove;
    map->indices[idx_to_remove] = DAST_MAP_BAD_SLOT_INDEX;
    map->elems--;
    return 1;
}