#include "map.h"


void dast_map_init(
    dast_map_t*       map,
    dast_allocator_t* allocator,
    int               key_size,
    int               val_size,
    unsigned long     (*hash)(void* key),
    int               (*cmp)(void* key1, void* key2)
)
{
    map->allocator = allocator;
    dast_array_init(&(map->buckets), allocator, sizeof(dast_array_t*));
    dast_array_reserve(&(map->buckets), DAST_MAP_INITIAL_CAPACITY);
    dast_mset(&(map->buckets.data), 0, sizeof(dast_array_t*)*DAST_MAP_INITIAL_CAPACITY);

    dast_array_init(&(map->non_empty_buckets_index), allocator, sizeof(unsigned long));
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
    map->hash_f = hash;
    map->cmp_f = cmp;
    map->key_size = key_size;
    map->val_size = val_size;
}

void dast_map_destroy(dast_map_t* map)
{
    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        map->allocator->deallocate(map->allocator, *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k));
    }
    dast_array_destroy(&(map->buckets));
    dast_array_destroy(&(map->non_empty_buckets_index));
}

void dast_map_copy(dast_map_t* map, dast_map_t* dst)
{
    dst->allocator = map->allocator;

    dast_array_init(&(dst->buckets), dst->allocator, sizeof(dast_array_t*));
    dast_array_reserve(&(dst->buckets), map->capacity);
    dast_mset(&(dst->buckets.data), 0, sizeof(dast_array_t*)*(map->capacity));
    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        dast_array_t* map_bucket = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k);
        dast_array_t* dst_bucket = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
        dast_array_copy(map_bucket, dst_bucket);
        DAST_ARRAY_REPLACE(&(dst->buckets), dst_bucket, k);
    }

    dast_array_copy(&(map->non_empty_buckets_index), &(dst->non_empty_buckets_index));

    dst->elems = map->elems;
    dst->capacity = map->capacity;
    dst->hash_f = map->hash_f;
    dst->cmp_f = map->cmp_f;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_deepcopy(
    dast_map_t* map, dast_map_t* dst, void (*cpy_key)(void* src, void* dst), void (*cpy_val)(void* src, void* dst)
)
{
    dst->allocator = map->allocator;

    dast_array_init(&(dst->buckets), dst->allocator, sizeof(dast_array_t*));
    dast_array_reserve(&(dst->buckets), map->capacity);
    dast_mset(&(dst->buckets.data), 0, sizeof(dast_array_t*)*(map->capacity));
    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        dast_array_t* map_bucket = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k);
        dast_array_t* dst_bucket = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
        dast_array_copy(map_bucket, dst_bucket);

        unsigned long slot_count = map_bucket->elems;
        dast_slote_t* src_slots = DAST_ARRAY_FIRST(map_bucket);
        dast_slote_t* dst_slots = DAST_ARRAY_FIRST(dst_bucket);
        for (unsigned long slot_idx = 0; slot_idx < slot_count; slot_idx++)
        {
            cpy_key(DAST_SLOTE_KEY(&(src_slots[slot_idx])), DAST_SLOTE_KEY(&(dst_slots[slot_idx])));
            cpy_val(DAST_SLOTE_VAL(map, &(src_slots[slot_idx])), DAST_SLOTE_VAL(map, &(dst_slots[slot_idx])));
        }

        DAST_ARRAY_REPLACE(&(dst->buckets), dst_bucket, k);
    }

    dast_array_copy(&(map->non_empty_buckets_index), &(dst->non_empty_buckets_index));
    
    dst->elems = map->elems;
    dst->capacity = map->capacity;
    dst->hash_f = map->hash_f;
    dst->cmp_f = map->cmp_f;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_clear(dast_map_t* map)
{
    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    dast_array_t* zero = 0;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        dast_array_t* bucket = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k);
        DAST_ARRAY_CLEAR(bucket);
        map->allocator->deallocate(map->allocator, bucket);
        DAST_ARRAY_REPLACE(&(map->buckets), &zero, k);
    }
    DAST_ARRAY_CLEAR(&(map->buckets));
    DAST_ARRAY_CLEAR(&(map->non_empty_buckets_index));
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
}

void dast_map_deepclear(dast_map_t* map, void (*del_key)(void* obj), void (*del_val)(void* obj))
{
    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    dast_array_t* zero = 0;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        dast_array_t* bucket = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k);
        dast_slote_t* slots = DAST_ARRAY_FIRST(bucket);
        unsigned long slot_count = bucket->elems;
        for (unsigned long slot_idx = 0; slot_idx < slot_count; slot_idx++)
        {
            del_key(DAST_SLOTE_KEY(&(slots[slot_idx])));
            del_val(DAST_SLOTE_VAL(map, &(slots[slot_idx])));
        }
        DAST_ARRAY_CLEAR(bucket);
        map->allocator->deallocate(map->allocator, bucket);
        DAST_ARRAY_REPLACE(&(map->buckets), &zero, k);
    }
    DAST_ARRAY_CLEAR(&(map->buckets));
    DAST_ARRAY_CLEAR(&(map->non_empty_buckets_index));
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
}

dast_slote_t* dast_map_get(dast_map_t* map, void* key)
{
    long          size = map->capacity;
    unsigned long h = map->hash_f(key);
    unsigned long bucket_idx = h % size;

    dast_array_t* slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), bucket_idx);
    if (!slots_array)
    {
        return 0;
    }

    int (*cmp_f)(void* key1, void* key2) = map->cmp_f;
    dast_slote_t* slots = DAST_ARRAY_FIRST(slots_array);
    unsigned n = slots_array->elems;
    for (unsigned long i = 0; i < n; i++)
    {
        if ((slots[i].hash == h) && (cmp_f(DAST_SLOTE_KEY(&(slots[i])), key) == 0))
        {
            return &(slots[i]);
        }
    }
    return 0;
}

void dast_map_set(dast_map_t* map, void* key, void* value)
{
    if (map->elems >= map->capacity * DAST_MAP_MAX_LOAD)
    {
        long new_capacity = map->capacity * DAST_MAP_GROW_FACTOR;

        dast_array_t new_buckets;
        dast_array_t new_non_empty_buckets_index;

        dast_array_init(&new_buckets, map->allocator, sizeof(dast_array_t*));
        dast_array_reserve(&new_buckets, new_capacity);
        dast_mset(&(new_buckets.data), 0, sizeof(dast_array_t*)*new_capacity);

        dast_array_init(&new_non_empty_buckets_index, map->allocator, sizeof(unsigned long));

        unsigned long bucket_count = map->non_empty_buckets_index.elems;
        for (unsigned long i = 0; i < bucket_count; i++)
        {
            unsigned long bucket_idx = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
            dast_array_t* slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), bucket_idx);
            unsigned long slot_count = slots_array->elems;
            dast_slote_t* slots = DAST_ARRAY_FIRST(slots_array);
            for (unsigned long k = 0; k < slot_count; k++)
            {
                unsigned long idx_to_put = slots[k].hash % new_capacity;
                dast_array_t* new_slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), idx_to_put);
                if (!slots_array)
                {
                    new_slots_array = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
                    dast_array_init(new_slots_array, map->allocator, DAST_MAP_SIZEOF_SLOTE(map));
                    DAST_ARRAY_REPLACE(&new_buckets, &new_slots_array, idx_to_put);
                    dast_array_append(&new_non_empty_buckets_index, &idx_to_put);
                }
                if (new_slots_array->elems == new_slots_array->capacity)
                {
                    dast_array_reserve(new_slots_array, new_slots_array->elems);
                }
                slots_array->elems++;
                dast_slote_t* new_slote = DAST_ARRAY_LAST(slots_array);
                dast_cpy_generic(&(slots[k]), DAST_SLOTE_KEY(new_slote), DAST_MAP_SIZEOF_SLOTE(map));
            }
            map->allocator->deallocate(map->allocator, slots_array);
        }
        dast_array_destroy(&(map->buckets));
        dast_array_destroy(&(map->non_empty_buckets_index));
        map->buckets = new_buckets;
        map->non_empty_buckets_index = new_non_empty_buckets_index;
    }

    long          size = map->capacity;
    unsigned long h = map->hash_f(key);
    unsigned long bucket_idx = h % size;
    
    dast_array_t* slots_array = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), bucket_idx);
    if (!slots_array)
    {
        slots_array = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
        dast_array_init(slots_array, map->allocator, DAST_MAP_SIZEOF_SLOTE(map));
        DAST_ARRAY_REPLACE(&(map->buckets), &slots_array, bucket_idx);
        dast_array_append(&(map->non_empty_buckets_index), &bucket_idx);
    }

    if (slots_array->elems == slots_array->capacity)
    {
        dast_array_reserve(slots_array, slots_array->elems);
    }
    slots_array->elems++;

    dast_slote_t* new_slote = DAST_ARRAY_LAST(slots_array);
    new_slote->hash = h;
    dast_cpy_generic(key, DAST_SLOTE_KEY(new_slote), map->key_size);
    dast_cpy_generic(value, DAST_SLOTE_VAL(map, new_slote), map->val_size);
    map->elems++;
}

void dast_map_del(dast_map_t* map, dast_slote_t* slote)
{
    dast_array_t* bucket = DAST_ARRAY_ITH(&(map->buckets), slote->hash % DAST_MAP_SIZEOF_SLOTE(map));
    unsigned long to_delete_idx = ((char*)slote - (char*)DAST_ARRAY_FIRST(bucket)) / DAST_MAP_SIZEOF_SLOTE(map);
    if (to_delete_idx != bucket->elems - 1)
    {
        DAST_ARRAY_REPLACE(bucket, DAST_ARRAY_LAST(bucket), to_delete_idx);
    }
    DAST_ARRAY_POP(bucket);
    map->elems--;
}

dast_array_t* dast_map_slots(dast_map_t* map)
{
    dast_array_t* slots = map->allocator->allocate(map->allocator, sizeof(dast_array_t));
    dast_array_init(slots, map->allocator, DAST_MAP_SIZEOF_SLOTE(map));
    dast_array_reserve(slots, map->elems);

    unsigned long bucket_count = map->non_empty_buckets_index.elems;
    for(unsigned long i = 0; i < bucket_count; i++)
    {
        unsigned long k = *(unsigned long*)DAST_ARRAY_ITH(&(map->non_empty_buckets_index), i);
        dast_array_t* bucket = *(dast_array_t**)DAST_ARRAY_ITH(&(map->buckets), k);
        dast_array_extend(slots, DAST_ARRAY_FIRST(bucket), bucket->elems);
    }

    return slots;
}