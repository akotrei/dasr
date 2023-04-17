#ifndef __DAST_HASH_H__
#define __DAST_HASH_H__

unsigned long dast_hash_ptr(void* key);
unsigned long dast_hash_int(void* key);
unsigned long dast_hash_float(void* key);

#endif /* __DAST_HASH_H__ */