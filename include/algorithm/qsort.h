#ifndef __DAST_QSORT_H__
#define __DAST_QSORT_H__

void dast_qsort(void* array, long elems, int elem_size, int (*cmp)(void* l, void* r));

#endif // __DAST_QSORT_H__