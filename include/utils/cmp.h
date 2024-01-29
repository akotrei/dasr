#ifndef __DAST_CMP_H__
#define __DAST_CMP_H__

#include "ntype.h"

typedef dast_i32_t (*dast_cmp_t)(void* o1, void* o2);

dast_i32_t dast_cmp_i8(void* o1, void* o2);
dast_i32_t dast_hash_i16(void* o1, void* o2);
dast_i32_t dast_hash_i32(void* o1, void* o2);
dast_i32_t dast_hash_i64(void* o1, void* o2);
dast_i32_t dast_hash_u8(void* o1, void* o2);
dast_i32_t dast_hash_u16(void* o1, void* o2);
dast_i32_t dast_hash_u32(void* o1, void* o2);
dast_i32_t dast_hash_u64(void* o1, void* o2);
dast_i32_t dast_hash_f32(void* o1, void* o2);
dast_i32_t dast_hash_f64(void* o1, void* o2);
dast_i32_t dast_hash_ptr(void* o1, void* o2);
dast_i32_t dast_hash_str(void* o1, void* o2);

#endif /* __DAST_CMP_H__ */