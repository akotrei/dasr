#ifndef __DAST_MEM_H__
#define __DAST_MEM_H__

#include "ntype.h"

typedef void (*dast_cpy_t)(void* obj, void* dst, dast_u64_t size);
typedef void (*dast_del_t)(void* obj);
typedef void (*dast_mset_t)(void* obj, dast_u8_t val, dast_u64_t size);

void dast_cpy_generic(void* obj, void* dst, dast_u64_t size);
void dast_move_generic(void* obj, void* dst, dast_u64_t size);
void dast_cpy_ptr(void* obj, void* dst, dast_u64_t size);
void dast_cpy_str(void* obj, void* dst, dast_u64_t size);
void dast_del_dummy(void* obj);
void dast_mset(void* obj, dast_u8_t val, dast_u64_t size);

#endif /* __DAST_MEM_H__ */