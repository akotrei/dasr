#ifndef __DAST_QSORT_H__
#define __DAST_QSORT_H__

unsigned long dast_qsort(void*         array,
                                    unsigned long elems,
                                    unsigned long elem_size,
                                    int (*cmp_f)(void* o1, void* o2));

#endif /* __DAST_QSORT_H__ */