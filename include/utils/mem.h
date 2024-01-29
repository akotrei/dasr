#ifndef __DAST_MEM_H__
#define __DAST_MEM_H__

#include "ntype.h"

typedef void (*dast_cpy_t)(void* obj, void* dst, dast_u64_t size);
typedef void (*dast_del_t)(void* obj);

void dast_cpy_generic(void* obj, void* dst, dast_u64_t size);
void dast_cpy_ptr(void* obj, void* dst, dast_u64_t size);
void dast_cpy_str(void* obj, void* dst, dast_u64_t size);
void dast_del_dummy(void* obj);

#endif /* __DAST_MEM_H__ */