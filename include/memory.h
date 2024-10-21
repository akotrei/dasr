#ifndef __DAST_MEMORY_H__
#define __DAST_MEMORY_H__

#include <stdlib.h>
#include <string.h>

#define DAST_MALLOC  malloc
#define DAST_CALLOC  calloc
#define DAST_REALLOC realloc
#define DAST_FREE    free

#define DAST_MEMCPY  memcpy
#define DAST_MEMMOVE memmove
#define DAST_MEMSET  memset

#endif // __DAST_MEMORY_H__