#ifndef __DAST_DISPLAY_H__
#define __DAST_DISPLAY_H__

#include "interface/allocator.h"

typedef char* (*dast_display_t)(void* obj, dast_allocator_t* allocator);

char* dast_display_int32_t(void* obj, dast_allocator_t* allocator);

#endif /* __DAST_DISPLAY_H__ */