#ifndef __DAST_HASH_H__
#define __DAST_HASH_H__

#include "ntype.h"

typedef dast_u64_t (*dast_hash_t)(void* key);

dast_u64_t dast_hash_i8(void* key);
dast_u64_t dast_hash_i16(void* key);
dast_u64_t dast_hash_i32(void* key);
dast_u64_t dast_hash_i64(void* key);
dast_u64_t dast_hash_u8(void* key);
dast_u64_t dast_hash_u16(void* key);
dast_u64_t dast_hash_u32(void* key);
dast_u64_t dast_hash_u64(void* key);
dast_u64_t dast_hash_f32(void* key);
dast_u64_t dast_hash_f64(void* key);
dast_u64_t dast_hash_ptr(void* key);
dast_u64_t dast_hash_str(void* key);

#endif /* __DAST_HASH_H__ */