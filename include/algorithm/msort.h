#ifndef __DAST_MSORT_H__
#define __DAST_MSORT_H__

void dast_msort(void* array, void* dst, long elems, int elem_size, int (*cmp)(void* l, void* r));
void dast_msort_int(int* array, int* dst, long elems);

#endif // __DAST_MSORT_H__