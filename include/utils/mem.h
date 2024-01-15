#ifndef __DAST_MEM_H__
#define __DAST_MEM_H__

typedef void (*dast_cpy_t)(void* obj, void* dst);
typedef void (*dast_del_t)(void* obj);

void dast_cpy_i8(void* obj, void* dst);
void dast_cpy_i16(void* obj, void* dst);
void dast_cpy_i32(void* obj, void* dst);
void dast_cpy_i64(void* obj, void* dst);
void dast_cpy_f32(void* obj, void* dst);
void dast_cpy_f64(void* obj, void* dst);
void dast_cpy_ptr(void* obj, void* dst);
void dast_cpy_str(void* obj, void* dst);

#endif /* __DAST_MEM_H__ */