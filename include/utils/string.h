#ifndef __DAST_STRING_H__
#define __DAST_STRING_H__

#include "interface/iallocator.h"

int dast_string_cmp(void* o1, void* o2);
unsigned long dast_string_hash(void* key);
unsigned long dast_string_len(char* str);
void dast_string_cpy_mem(char* mem, char* str);
char* dast_string_cpy_alloc(dast_allocator_t* allocator, char* str);

void dast_string_capitalize(char* str);
void dast_string_casefold(char* str);
long dast_string_find(char* str, char* pattern);
unsigned long dast_string_count(char* str, char* pattern);


#endif /* __DAST_STRING_H__ */