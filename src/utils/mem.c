#include "utils/mem.h"
#include <stdlib.h>
#include <string.h>

void dast_cpy_generic(void* obj, void* dst, unsigned long size) { memcpy(dst, obj, size); }
void dast_move_generic(void* obj, void* dst, unsigned long size) { memmove(dst, obj, size); }
void dast_cpy_ptr(void* obj, void* dst, unsigned long size) { memcpy(dst, &obj, size); }
void dast_cpy_str(void* obj, void* dst, unsigned long size) { strcpy(dst, obj); }
void dast_del_dummy(void* obj) {}
void dast_mset(void* obj, unsigned char val, unsigned long size) { memset(obj, val, size); }