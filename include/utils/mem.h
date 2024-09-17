#ifndef __DAST_MEM_H__
#define __DAST_MEM_H__

void dast_cpy_generic(void* obj, void* dst, unsigned long size);
void dast_move_generic(void* obj, void* dst, unsigned long size);
void dast_cpy_ptr(void* obj, void* dst, unsigned long size);
void dast_cpy_str(void* obj, void* dst, unsigned long size);
void dast_del_dummy(void* obj);
void dast_mset(void* obj, unsigned char val, unsigned long size);

#endif /* __DAST_MEM_H__ */