#include "utils/mem.h"
#include <stdlib.h>
#include <string.h>


void dast_cpy_generic(void* obj, void* dst, dast_u64_t size) { memcpy(dst, obj, size); }
void dast_move_generic(void* obj, void* dst, dast_u64_t size) { memmove(dst, obj, size); }
void dast_cpy_ptr(void* obj, void* dst, dast_u64_t size) { memcpy(dst, &obj, size); }
void dast_cpy_str(void* obj, void* dst, dast_u64_t size) { strcpy(dst, obj); }
void dast_del_dummy(void* obj) {}
void dast_mset(void* obj, dast_u8_t val, dast_u64_t size) { memset(obj, val, size); }