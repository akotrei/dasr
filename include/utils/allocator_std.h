#ifndef __DAST_ALLOCATOR_STD_H__
#define __DAST_ALLOCATOR_STD_H__

#include "interface/allocator.h"

unsigned long     dast_allocator_std_sizeof();
dast_allocator_t* dast_allocator_std_init(void* memory);

#endif /* __DAST_ALLOCATOR_STD_H__ */