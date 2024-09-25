#ifndef __DAST_MAP_H__
#define __DAST_MAP_H__

#include "array.h"
#include "memory.h"

#define DAST_MAP_GROW_FACTOR 2.0f
#define DAST_MAP_INITIAL_CAPACITY 8
#define DAST_MAP_MAX_LOAD 0.5f
#define DAST_MAP_MIN_LOAD 2

// memory laoyot of @dast_slote_t type:
// dast_slote_t* slote -> |--hash--|--key--|--value--|
typedef struct _dast_slote_t { unsigned long hash; } dast_slote_t;

// used to handle each bucket of slots
typedef struct _dast_bucket_info_t {unsigned long elems; unsigned long capacity; } dast_bucket_info_t;

typedef struct _dast_map_t
{
    dast_slote_t**      buckets;
    dast_bucket_info_t* buckets_info;
    unsigned long       elems;
    unsigned long       capacity;
    unsigned long       (*hash)(void* key);
    int                 (*cmp)(void* key1, void* key2);
    int                 key_size;
    int                 val_size;
} dast_map_t;

// Initialize an map instance on @map
// @key_size - size (in bytes) of keys that @map should hold
// @val_size - size (in bytes) of values that @map should hold
// @hash - hash function to get key's hash
// @cmp - comparing function to compare keys
void dast_map_init(dast_map_t* map, int key_size, int val_size, unsigned long (*hash)(void* key), int (*cmp)(void* key1, void* key2));

// Destroy @map and free memory that @map uses
void dast_map_destroy(dast_map_t* map);

// Copy @map to @dst. Note: it is a shallow copying
// Note: @dst supposed to be not initialized
void dast_map_copy(dast_map_t* map, dast_map_t* dst);

// Copy @map to @dst. Also it copies all objcets too (invokes @cpy_key for each key and @cpy_val for each value)
// Note: @dst supposed to be not initialized
void dast_map_deepcopy(dast_map_t* map, dast_map_t* dst, void (*cpy_key)(void* src, void* dst), void (*cpy_val)(void* src, void* dst));

// remove all objects from @map
void dast_map_clear(dast_map_t* map);

// remove all objects from @map with invoking @del_key and @del_val on each key and value
void dast_map_deepclear(dast_map_t* map, void (*del_key)(void* obj), void (*del_val)(void* obj));

// Get key from a slote that is return by @dast_map_get. Note: @slote should not be 0
#define DAST_SLOTE_KEY(slote) ((char*)(slote) + sizeof(dast_slote_t))

// Get value from a slote that is return by @dast_map_get. Note: @slote should not be 0
#define DAST_SLOTE_VAL(map, slote) ((char*)(slote) + sizeof(dast_slote_t) + (map)->key_size)

// Get a slote by @key. If @key does not exist return 0
dast_slote_t* dast_map_get(dast_map_t* map, void* key);

// Set new slote with @key and @value, if @key is already exists -
// overrides this slote with @key and @value
void dast_map_set(dast_map_t* map, void* key, void* value);

// Delete @slote from @map, @slote should be a result of @dast_map_get, and not be 0
// Also between @dast_map_get and @dast_map_del should not be any @map's modifications -
// it may make @slote to be invalid
void dast_map_del(dast_map_t* map, dast_slote_t* slote);

// Get array of slots from @map
// The Caller is responsible for deleting return slots
dast_array_t* dast_map_slots(dast_map_t* map);

#endif // __DAST_MAP_H__