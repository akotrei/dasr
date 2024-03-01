#ifndef __DAST_CMP_H__
#define __DAST_CMP_H__

#include "ntype.h"

typedef dast_i32_t (*dast_cmp_t)(void* o1, void* o2);

dast_i32_t dast_cmp_i8(void* o1, void* o2);
dast_i32_t dast_cmp_i16(void* o1, void* o2);
dast_i32_t dast_cmp_i32(void* o1, void* o2);
dast_i32_t dast_cmp_i64(void* o1, void* o2);
dast_i32_t dast_cmp_u8(void* o1, void* o2);
dast_i32_t dast_cmp_u16(void* o1, void* o2);
dast_i32_t dast_cmp_u32(void* o1, void* o2);
dast_i32_t dast_cmp_u64(void* o1, void* o2);
dast_i32_t dast_cmp_f32(void* o1, void* o2);
dast_i32_t dast_cmp_f64(void* o1, void* o2);
dast_i32_t dast_cmp_ptr(void* o1, void* o2);
dast_i32_t dast_cmp_str(void* o1, void* o2);

#endif /* __DAST_CMP_H__ */