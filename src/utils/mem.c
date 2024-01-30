#include "utils/mem.h"
#include <string.h>

void dast_cpy_generic(void* obj, void* dst, dast_u64_t size) { memcpy(dst, obj, size); }
void dast_cpy_ptr(void* obj, void* dst, dast_u64_t size) { memcpy(dst, &obj, size); }
void dast_cpy_str(void* obj, void* dst, dast_u64_t size) { strcpy(dst, obj); }
void dast_del_dummy(void* obj) {}