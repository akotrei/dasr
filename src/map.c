#include "map.h"


void dast_map_init(dast_map_t* map, int key_size, int val_size, unsigned long (*hash)(void* key), int (*cmp)(void* key1, void* key2))
{
    dast_slote_t**      buckets = DAST_MALLOC(sizeof(dast_slote_t*)*DAST_MAP_INITIAL_CAPACITY);
    dast_bucket_info_t* buckets_info = DAST_MALLOC(sizeof(dast_bucket_info_t)*DAST_MAP_INITIAL_CAPACITY);
    DAST_MEMSET(buckets, 0, sizeof(dast_slote_t*)*DAST_MAP_INITIAL_CAPACITY);
    DAST_MEMSET(buckets_info, 0, sizeof(dast_bucket_info_t)*DAST_MAP_INITIAL_CAPACITY);
    map->buckets = buckets;
    map->buckets_info = buckets_info;
    map->elems = 0;
    map->capacity = DAST_MAP_INITIAL_CAPACITY;
    map->hash = hash;
    map->cmp = cmp;
    map->key_size = key_size;
    map->val_size = val_size;
}

void dast_map_destroy(dast_map_t* map)
{
    DAST_FREE(map->buckets);
    DAST_FREE(map->buckets_info);
}

void dast_map_copy(dast_map_t* map, dast_map_t* dst)
{
    int           key_size = map->key_size;
    int           val_size = map->val_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + val_size;
    unsigned long capacity = map->capacity;

    dast_slote_t**      src_buckets = map->buckets;
    dast_bucket_info_t* src_buckets_info = map->buckets_info;

    dast_slote_t**      dst_buckets = DAST_MALLOC(sizeof(dast_slote_t*)*capacity);
    dast_bucket_info_t* dst_buckets_info = DAST_MALLOC(sizeof(dast_bucket_info_t)*capacity);
    for (unsigned long i = 0; i < capacity; i++)
    {
        if (src_buckets[i])
        {
            dst_buckets[i] = DAST_MALLOC(slot_size*src_buckets_info[i].capacity);
            DAST_MEMCPY(dst_buckets[i], src_buckets[i], slot_size*src_buckets_info[i].elems);
        }
        else
        {
            dst_buckets[i] = 0;
        }
        dst_buckets_info[i] = src_buckets_info[i];
    }

    dst->buckets = dst_buckets;
    dst->buckets_info = dst_buckets_info;
    dst->elems = map->elems;
    dst->capacity = capacity;
    dst->hash = map->hash;
    dst->cmp = map->cmp;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_deepcopy(dast_map_t* map, dast_map_t* dst, void (*cpy_key)(void* src, void* dst), void (*cpy_val)(void* src, void* dst))
{
    int           key_size = map->key_size;
    int           val_size = map->val_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + val_size;
    unsigned long capacity = map->capacity;

    dast_slote_t**      src_buckets = map->buckets;
    dast_bucket_info_t* src_buckets_info = map->buckets_info;

    dast_slote_t**      dst_buckets = DAST_MALLOC(sizeof(dast_slote_t*)*capacity);
    dast_bucket_info_t* dst_buckets_info = DAST_MALLOC(sizeof(dast_bucket_info_t)*capacity);
    for (unsigned long i = 0; i < capacity; i++)
    {
        if (src_buckets[i])
        {
            dst_buckets[i] = DAST_MALLOC(slot_size*src_buckets_info[i].capacity);
            unsigned long elems_in_bucket = src_buckets_info[i].elems;
            dast_slote_t* src_slots = src_buckets[i];
            dast_slote_t* dst_slots = dst_buckets[i];
            for (unsigned long k = 0; i < elems_in_bucket; k++)
            {
                dast_slote_t* src_slote = (dast_slote_t*)((char*)src_slots + slot_size*k);
                dast_slote_t* dst_slote = (dast_slote_t*)((char*)dst_slots + slot_size*k);
                dst_slote->hash = src_slote->hash;
                cpy_key((char*)dst_slote + sizeof(dast_slote_t), (char*)src_slote + sizeof(dast_slote_t));
                cpy_val(
                    (char*)dst_slote + sizeof(dast_slote_t) + key_size,
                    (char*)src_slote + sizeof(dast_slote_t) + key_size
                );
            }
        }
        else
        {
            dst_buckets[i] = 0;
        }
        dst_buckets_info[i] = src_buckets_info[i];
    }

    dst->buckets = dst_buckets;
    dst->buckets_info = dst_buckets_info;
    dst->elems = map->elems;
    dst->capacity = capacity;
    dst->hash = map->hash;
    dst->cmp = map->cmp;
    dst->key_size = map->key_size;
    dst->val_size = map->val_size;
}

void dast_map_clear(dast_map_t* map)
{
    unsigned long       capacity = map->capacity;
    dast_bucket_info_t* buckets_info = map->buckets_info;
    for (unsigned long i = 0; i < capacity; i++)
    {
        buckets_info[i].elems = 0;
    }
    map->elems = 0;
}

void dast_map_deepclear(dast_map_t* map, void (*del_key)(void* obj), void (*del_val)(void* obj))
{
    int           key_size = map->key_size;
    int           val_size = map->val_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + val_size;

    unsigned long       capacity = map->capacity;
    dast_slote_t**      buckets = map->buckets;
    dast_bucket_info_t* buckets_info = map->buckets_info;
    for (unsigned long i = 0; i < capacity; i++)
    {
        unsigned long elems_in_bucket = buckets_info[i].elems;
        dast_slote_t* slots = buckets[i];
        for (unsigned long k = 0; i < elems_in_bucket; k++)
        {
            dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
            del_key((char*)slote + sizeof(dast_slote_t));
            del_val((char*)slote + sizeof(dast_slote_t) + key_size);
        }
        buckets_info[i].elems = 0;
    }
    map->elems = 0;
}

dast_slote_t* dast_map_get(dast_map_t* map, void* key)
{
    int           key_size = map->key_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + map->val_size;
    unsigned long h = map->hash(key);
    unsigned long slots_count = (map->buckets_info)[h % map->capacity].elems;
    dast_slote_t* slots = (map->buckets)[h % map->capacity];
    int           (*cmp)(void* key1, void* key2) = map->cmp;

    for (unsigned long k = 0; k < slots_count; k++)
    {
        dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
        if ((slote->hash == h) && (cmp((char*)slote + key_size, key) == 0))
        {
            return slote;
        }
    }
    return 0;
}

void dast_map_set(dast_map_t* map, void* key, void* value)
{
    int           key_size = map->key_size;
    int           val_size = map->val_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + val_size;

    if (map->elems >= map->capacity * DAST_MAP_MAX_LOAD)
    {
        // rehash
        unsigned long src_capacity = map->capacity;
        unsigned long dst_capacity = src_capacity * DAST_MAP_GROW_FACTOR;

        dast_slote_t**      src_buckets = map->buckets;
        dast_bucket_info_t* src_buckets_info = map->buckets_info;

        dast_slote_t**      dst_buckets = DAST_MALLOC(sizeof(dast_slote_t*)*dst_capacity);
        dast_bucket_info_t* dst_buckets_info = DAST_MALLOC(sizeof(dast_bucket_info_t)*dst_capacity);

        DAST_MEMSET(dst_buckets, 0, sizeof(dast_slote_t*)*dst_capacity);
        DAST_MEMSET(dst_buckets_info, 0, sizeof(dast_bucket_info_t)*dst_capacity);

        for (unsigned long i = 0; i < src_capacity; i++)
        {
            dast_slote_t* src_slots = src_buckets[i];
            if (!src_slots)
            {
                continue;
            }
            unsigned long slots_count = src_buckets_info[i].elems;
            for (unsigned long k = 0; k < slots_count; k++)
            {
                dast_slote_t* src_slote = (dast_slote_t*)((char*)src_slots + slot_size*k);

                unsigned long dst_idx = src_slote->hash % dst_capacity;
                unsigned long dst_slots_count = dst_buckets_info[dst_idx].elems;
                unsigned long dst_slots_capacity = dst_buckets_info[dst_idx].capacity;
                if (dst_slots_count == dst_slots_capacity)
                {
                    dst_buckets[dst_idx] = DAST_REALLOC(dst_buckets[dst_idx], slot_size*(dst_slots_count*2 + 2));
                    dst_buckets_info[dst_idx].capacity = dst_slots_count*2 + 2;
                }

                dast_slote_t* dst_slots = dst_buckets[dst_idx];
                dast_slote_t* dst_slote = (dast_slote_t*)((char*)dst_slots + slot_size*dst_slots_count);
                DAST_MEMCPY((char*)dst_slote + sizeof(dast_slote_t), (char*)src_slote + sizeof(dast_slote_t), key_size);
                DAST_MEMCPY(
                    (char*)dst_slote + sizeof(dast_slote_t) + key_size,
                    (char*)src_slote + sizeof(dast_slote_t) + key_size, val_size
                );
                dst_buckets_info[dst_idx].elems += 1;
            }
            DAST_FREE(src_slots);
        }
        map->buckets = dst_buckets;
        map->buckets_info = dst_buckets_info;
        DAST_FREE(src_buckets);
        DAST_FREE(src_buckets_info);
    }

    unsigned long h = map->hash(key);
    unsigned long bucket_index = h % map->capacity;
    unsigned long slots_count = (map->buckets_info)[bucket_index].elems;
    unsigned long slots_capacity = (map->buckets_info)[bucket_index].capacity;
    int           (*cmp)(void* key1, void* key2) = map->cmp;

    if (slots_count == slots_capacity)
    {
        (map->buckets)[bucket_index] = DAST_REALLOC((map->buckets)[bucket_index], slot_size*(slots_count*2 + 2));
        (map->buckets_info)[bucket_index].capacity = slots_count*2 + 2;
    }

    unsigned long k = 0;
    dast_slote_t* slots = (map->buckets)[bucket_index];
    dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
    while (k < slots_count)
    {
        slote = (dast_slote_t*)((char*)slots + slot_size*k);
        if ((slote->hash == h) && (cmp((char*)slote + key_size, key) == 0))
        {
            break;
        }
        k++;
    }
    DAST_MEMCPY((char*)slote + sizeof(dast_slote_t), key, key_size);
    DAST_MEMCPY((char*)slote + sizeof(dast_slote_t) + key_size, value, val_size);
    map->elems += (k == slots_count);
    (map->buckets_info)[bucket_index].elems += (k == slots_count);
}

void dast_map_del(dast_map_t* map, dast_slote_t* slote)
{
    int           key_size = map->key_size;
    int           val_size = map->val_size;
    int           slot_size = sizeof(dast_slote_t) + key_size + val_size;
    unsigned long bucket_index = slote->hash % map->capacity;
    dast_slote_t* slots = (map->buckets)[bucket_index];
    unsigned long k = ((char*)slots - (char*)slote) / slot_size;
    unsigned long l = map->buckets_info[bucket_index].elems - 1;
    if (k < l)
    {
        dast_slote_t* last_slote = (dast_slote_t*)((char*)slots + slot_size*l);
        DAST_MEMCPY((char*)slote + sizeof(dast_slote_t), (char*)last_slote + sizeof(dast_slote_t), key_size);
        DAST_MEMCPY(
            (char*)slote + sizeof(dast_slote_t) + key_size,
            (char*)last_slote + sizeof(dast_slote_t) + key_size,
            val_size
        );
    }
    map->buckets_info[bucket_index].elems--;
    map->elems--;
}

dast_array_t* dast_map_slots(dast_map_t* map)
{
    unsigned long capacity = map->capacity;
    int           slot_size = sizeof(dast_slote_t) + map->key_size + map->val_size;

    dast_array_t* array_slots = DAST_MALLOC(sizeof(dast_array_t));
    dast_array_init(array_slots, sizeof(slot_size));

    dast_slote_t**      buckets = map->buckets;
    dast_bucket_info_t* buckets_info = map->buckets_info;

    for (unsigned long i = 0; i < capacity; i++)
    {
        dast_slote_t* slots = buckets[i];
        if (!slots)
        {
            continue;
        }
        dast_array_extend(array_slots, slots, buckets_info[i].elems);
    }
    return array_slots;
}