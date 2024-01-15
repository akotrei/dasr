#ifndef __DAST_ALLOCATOR_H__
#define __DAST_ALLOCATOR_H__

#include "interface/iallocator.h"

unsigned long     dast_allocator_sizeof();
dast_iallocator_t* dast_allocator_init(void* memory);

#endif /* __DAST_ALLOCATOR_H__ */