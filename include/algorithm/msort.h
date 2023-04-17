#ifndef __DAST_MSORT_H__
#define __DAST_MSORT_H__

unsigned long dast_msort(void*         array,
                         unsigned long elems,
                         unsigned long elem_size,
                         int (*cmp_f)(void* o1, void* o2),
                         void* memory);

#endif /* __DAST_MSORT_H__ */