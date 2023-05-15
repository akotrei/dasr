#ifndef __DAST_MEM_H__
#define __DAST_MEM_H__

typedef void (*dast_cpy_f)(void* obj, void* mem);
typedef void (*dast_del_f)(void* obj);

void dast_cpy_shallow(void* dst, void* mem);


#endif /* __DAST_MEM_H__ */