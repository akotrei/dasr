#include "set.h"
#include <stdio.h>

void dast_set_init(dast_set_t* set, int key_size, unsigned long (*hash)(void* k), int (*cmp)(void* l, void* r))
{
    dast_slote_t**      buckets = DAST_MALLOC(sizeof(dast_slote_t*)*DAST_SET_INITIAL_CAPACITY);
    dast_bucket_info_t* buckets_info = DAST_MALLOC(sizeof(dast_bucket_info_t)*DAST_SET_INITIAL_CAPACITY);
    DAST_MEMSET(buckets, 0, sizeof(dast_slote_t*)*DAST_SET_INITIAL_CAPACITY);
    DAST_MEMSET(buckets_info, 0, sizeof(dast_bucket_info_t)*DAST_SET_INITIAL_CAPACITY);
    set->buckets = buckets;
    set->buckets_info = buckets_info;
    set->elems = 0;
    set->capacity = DAST_SET_INITIAL_CAPACITY;
    set->hash = hash;
    set->cmp = cmp;
    set->key_size = key_size;
}

void dast_set_destroy(dast_set_t* set)
{
    DAST_FREE(set->buckets);
    DAST_FREE(set->buckets_info);
}

void dast_set_copy(dast_set_t* set, dast_set_t* dst)
{
    int           slot_size = sizeof(dast_slote_t) + set->key_size;
    unsigned long capacity = set->capacity;

    dast_slote_t**      src_buckets = set->buckets;
    dast_bucket_info_t* src_buckets_info = set->buckets_info;

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
    dst->elems = set->elems;
    dst->capacity = capacity;
    dst->hash = set->hash;
    dst->cmp = set->cmp;
    dst->key_size = set->key_size;
}

void dast_set_deepcopy(dast_set_t* set, dast_set_t* dst, void (*cpy)(void* src, void* dst))
{
    int           slot_size = sizeof(dast_slote_t) + set->key_size;
    unsigned long capacity = set->capacity;

    dast_slote_t**      src_buckets = set->buckets;
    dast_bucket_info_t* src_buckets_info = set->buckets_info;

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
                cpy((char*)dst_slote + sizeof(dast_slote_t), (char*)src_slote + sizeof(dast_slote_t));
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
    dst->elems = set->elems;
    dst->capacity = capacity;
    dst->hash = set->hash;
    dst->cmp = set->cmp;
    dst->key_size = set->key_size;
}

void dast_set_clear(dast_set_t* set)
{
    unsigned long       capacity = set->capacity;
    dast_bucket_info_t* buckets_info = set->buckets_info;
    for (unsigned long i = 0; i < capacity; i++)
    {
        buckets_info[i].elems = 0;
    }
    set->elems = 0;
}

void dast_set_deepclear(dast_set_t* set, void (*del)(void* k))
{
    int slot_size = sizeof(dast_slote_t) + set->key_size;

    unsigned long       capacity = set->capacity;
    dast_slote_t**      buckets = set->buckets;
    dast_bucket_info_t* buckets_info = set->buckets_info;
    for (unsigned long i = 0; i < capacity; i++)
    {
        unsigned long elems_in_bucket = buckets_info[i].elems;
        dast_slote_t* slots = buckets[i];
        for (unsigned long k = 0; i < elems_in_bucket; k++)
        {
            dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
            del((char*)slote + sizeof(dast_slote_t));
        }
        buckets_info[i].elems = 0;
    }
    set->elems = 0;
}

dast_slote_t* dast_set_get(dast_set_t* set, void* key)
{
    int           slot_size = sizeof(dast_slote_t) + set->key_size;
    unsigned long h = set->hash(key);
    unsigned long slots_count = (set->buckets_info)[h % set->capacity].elems;
    dast_slote_t* slots = (set->buckets)[h % set->capacity];
    int           (*cmp)(void* key1, void* key2) = set->cmp;

    for (unsigned long k = 0; k < slots_count; k++)
    {
        dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
        if ((slote->hash == h) && (cmp((char*)slote + sizeof(dast_slote_t), key) == 0))
        {
            return slote;
        }
    }
    return 0;
}

void dast_set_add(dast_set_t* set, void* key)
{
    int key_size = set->key_size;
    int slot_size = sizeof(dast_slote_t) + key_size;

    if (set->elems >= set->capacity * DAST_SET_MAX_LOAD)
    {
        // rehash
        unsigned long src_capacity = set->capacity;
        unsigned long dst_capacity = src_capacity * DAST_SET_GROW_FACTOR;

        dast_slote_t**      src_buckets = set->buckets;
        dast_bucket_info_t* src_buckets_info = set->buckets_info;

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
                dst_slote->hash = src_slote->hash;
                DAST_MEMCPY((char*)dst_slote + sizeof(dast_slote_t), (char*)src_slote + sizeof(dast_slote_t), key_size);
                dst_buckets_info[dst_idx].elems += 1;
            }
            DAST_FREE(src_slots);
        }
        set->buckets = dst_buckets;
        set->buckets_info = dst_buckets_info;
        set->capacity = dst_capacity;
        DAST_FREE(src_buckets);
        DAST_FREE(src_buckets_info);
    }

    unsigned long h = set->hash(key);
    unsigned long bucket_index = h % set->capacity;
    unsigned long slots_count = (set->buckets_info)[bucket_index].elems;
    unsigned long slots_capacity = (set->buckets_info)[bucket_index].capacity;
    int           (*cmp)(void* key1, void* key2) = set->cmp;

    if (slots_count == slots_capacity)
    {
        (set->buckets)[bucket_index] = DAST_REALLOC((set->buckets)[bucket_index], slot_size*(slots_count*2 + 2));
        (set->buckets_info)[bucket_index].capacity = slots_count*2 + 2;
    }

    unsigned long k = 0;
    dast_slote_t* slots = (set->buckets)[bucket_index];
    dast_slote_t* slote = (dast_slote_t*)((char*)slots + slot_size*k);
    while (k < slots_count)
    {
        if ((slote->hash == h) && (cmp((char*)slote + sizeof(dast_slote_t), key) == 0))
        {
            break;
        }
        k++;
        slote = (dast_slote_t*)((char*)slots + slot_size*k);
    }
    slote->hash = h;
    DAST_MEMCPY((char*)slote + sizeof(dast_slote_t), key, key_size);
    set->elems += (k == slots_count);
    (set->buckets_info)[bucket_index].elems += (k == slots_count);
}

void dast_set_del(dast_set_t* set, dast_slote_t* slote)
{
    int           key_size = set->key_size;
    int           slot_size = sizeof(dast_slote_t) + key_size;
    unsigned long bucket_index = slote->hash % set->capacity;
    dast_slote_t* slots = (set->buckets)[bucket_index];
    unsigned long k = ((char*)slots - (char*)slote) / slot_size;
    unsigned long l = set->buckets_info[bucket_index].elems - 1;
    if (k < l)
    {
        dast_slote_t* last_slote = (dast_slote_t*)((char*)slots + slot_size*l);
        DAST_MEMCPY((char*)slote + sizeof(dast_slote_t), (char*)last_slote + sizeof(dast_slote_t), key_size);
    }
    set->buckets_info[bucket_index].elems--;
    set->elems--;
}

dast_array_t* dast_set_slots(dast_set_t* set)
{
    unsigned long capacity = set->capacity;
    int           slot_size = sizeof(dast_slote_t) + set->key_size;

    dast_array_t* array_slots = DAST_MALLOC(sizeof(dast_array_t));
    dast_array_init(array_slots, sizeof(slot_size));

    dast_slote_t**      buckets = set->buckets;
    dast_bucket_info_t* buckets_info = set->buckets_info;

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
