#include "map.h"


void dast_map_init(dast_map_t* map, int key_size, int val_size, unsigned long (*hash)(void* key), int (*cmp)(void* key1, void* key2))
{
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
    map->hash = hash;
    map->cmp = cmp;
    map->key_size = key_size;
    map->val_size = val_size;
    int slot_size = sizeof(dast_slote_t) + key_size + val_size;
    dast_slote_t* slots = DAST_MALLOC(slot_size*DAST_MAP_INITIAL_CAPACITY);
    for (unsigned long i = 0; i < DAST_MAP_INITIAL_CAPACITY; i++)
    {
        dast_slote_t* slot = (void*)((char*)slots + slot_size*i);
        slot->kind = SLOTE_EMPTY;
    }
    map->slots = slots;
}

void dast_map_destroy(dast_map_t* map) { DAST_FREE(map->slots); }

void dast_map_copy(dast_map_t* map, dast_map_t* dst)
{
    int slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;
    dast_slote_t* dst_slots = DAST_MALLOC(slot_size*(map->capacity));
    DAST_MEMCPY(dst_slots, map->slots, slot_size*(map->capacity));
    dst->slots = dst_slots;
    dst->elems = map->elems;
    dst->capacity = map->capacity;
    dst->hash = map->hash;
    dst->cmp = map->cmp;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_deepcopy(
    dast_map_t* map, dast_map_t* dst, void (*cpy_key)(void* src, void* dst), void (*cpy_val)(void* src, void* dst)
)
{
    int slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;
    int key_size = map->key_size;
    unsigned long capacity = map->capacity;
    dast_slote_t* src_slots = map->slots;
    dast_slote_t* dst_slots = DAST_MALLOC(slot_size*capacity);
    for (unsigned long i = 0; i < capacity; i++)
    {
        dast_slote_t* src_slote = (void*)((char*)src_slots + slot_size*i);
        dast_slote_t* dst_slote = (void*)((char*)dst_slots + slot_size*i);
        dst_slote->kind = src_slote->kind;
        dst_slote->hash = src_slote->hash;
        if (dst_slote->kind == SLOTE_BISY)
        {
            cpy_key((char*)src_slote + sizeof(dast_slote_t), (char*)dst_slote + sizeof(dast_slote_t));
            cpy_val((char*)src_slote + sizeof(dast_slote_t) + key_size,
                    (char*)dst_slots + sizeof(dast_slote_t) + key_size);
        }
    }
    dst->slots = dst_slots;
    dst->elems = map->elems;
    dst->capacity = map->capacity;
    dst->hash = map->hash;
    dst->cmp = map->cmp;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_clear(dast_map_t* map)
{
    int slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;
    dast_array_t* slots = DAST_REALLOC(map->slots, slot_size*DAST_MAP_INITIAL_CAPACITY);
    for (unsigned long i = 0; i < DAST_MAP_INITIAL_CAPACITY; i++)
    {
        dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*i);
        slote->kind = SLOTE_EMPTY;
    }
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
}

void dast_map_deepclear(dast_map_t* map, void (*del_key)(void* obj), void (*del_val)(void* obj))
{
    int slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;
    int key_size = map->key_size;
    unsigned long capacity = map->capacity;
    dast_slote_t* slots = map->slots;
    for (unsigned long i = 0; i < capacity; i++)
    {
        dast_slote_t* slote = (void*)((char*)slots + slot_size*i);
        if (slote->kind == SLOTE_BISY)
        {
            del_key((char*)slote + sizeof(dast_slote_t));
            del_val((char*)slote + sizeof(dast_slote_t) + key_size);
        }
    }
    dast_map_clear(map);
}

dast_slote_t* dast_map_get(dast_map_t* map, void* key)
{
    int slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;
    int key_size = map->key_size;
    unsigned long capacity = map->capacity;
    unsigned long h = map->hash(key);
    unsigned long i = h % capacity;
    int (*cmp)(void* key1, void* key2) = map->cmp;
    dast_slote_t* slots = map->slots;
    dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*i);

    while (slote->kind != SLOTE_EMPTY)
    {
        if ((slote->kind != SLOTE_DELETED) && (slote->hash == h) && (cmp((char*)slote + key_size, key) == 0))
        {
            return slote;
        }
        i = (i + 1) % capacity;
        slote = (dast_slote_t*)((char*)slots + slot_size*i);
    }
    return 0;
}

void dast_map_set(dast_map_t* map, void* key, void* value)
{
    // if (map->elems >= map->capacity * DAST_MAP_MAX_LOAD)
    // {
    //     long new_capacity = map->capacity * DAST_MAP_GROW_FACTOR;

    //     dast_array_t new_buckets;
    //     dast_array_t new_non_empty_buckets_index;

    //     dast_array_init(&new_buckets, map->allocator, sizeof(dast_array_t*));
    //     dast_array_reserve(&new_buckets, new_capacity);
    //     dast_mset(&(new_buckets.data), 0, sizeof(dast_array_t*)*new_capacity);

    //     dast_array_init(&new_non_empty_buckets_index, map->allocator, sizeof(unsigned long));

    //     unsigned long bucket_count = map->non_empty_buckets_index.elems;
    //     for (unsigned long i = 0; i < bucket_count; i++)
    //     {
    //         unsigned long bucket_idx = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
    //         dast_array_t* slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), bucket_idx);
    //         unsigned long slot_count = slots_array->elems;
    //         dast_slote_t* slots = DAST_ARRAY_FIRST(slots_array);
    //         for (unsigned long k = 0; k < slot_count; k++)
    //         {
    //             unsigned long idx_to_put = slots[k].hash % new_capacity;
    //             dast_array_t* new_slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), idx_to_put);
    //             if (!new_slots_array)
    //             {
    //                 new_slots_array = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
    //                 dast_array_init(new_slots_array, map->allocator, DAST_SLOTE_SIZEOF(map));
    //                 DAST_ARRAY_REPLACE(&new_buckets, &new_slots_array, idx_to_put);
    //                 dast_array_append(&new_non_empty_buckets_index, &idx_to_put);
    //             }
    //             dast_array_append(new_slots_array, DAST_ARRAY_LAST(slots_array));
    //         }
    //         map->allocator->deallocate(map->allocator, slots_array);
    //     }
    //     dast_array_destroy(&(map->buckets));
    //     dast_array_destroy(&(map->non_empty_buckets_index));
    //     map->buckets = new_buckets;
    //     map->non_empty_buckets_index = new_non_empty_buckets_index;
    // }

    // long          size = map->capacity;
    // unsigned long h = map->hash(key);
    // unsigned long bucket_idx = h % size;
    
    // dast_array_t* slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), bucket_idx);
    // if (!slots_array)
    // {
    //     slots_array = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
    //     dast_array_init(slots_array, map->allocator, DAST_SLOTE_SIZEOF(map));
    //     DAST_ARRAY_REPLACE(&(map->buckets), &slots_array, bucket_idx);
    //     dast_array_append(&(map->non_empty_buckets_index), &bucket_idx);
    // }

    // if (slots_array->elems == slots_array->capacity)
    // {
    //     dast_array_reserve(slots_array, slots_array->elems);
    // }
    // slots_array->elems++;

    // dast_slote_t* new_slote = DAST_ARRAY_LAST(slots_array);
    // new_slote->hash = h;
    // dast_cpy_generic(key, DAST_SLOTE_KEY(new_slote), map->key_size);
    // dast_cpy_generic(value, DAST_SLOTE_VAL(map, new_slote), map->val_size);
    // map->elems++;
}

void dast_map_del(dast_map_t* map, dast_slote_t* slote)
{
    slote->kind = SLOTE_DELETED;
    map->elems--;
}