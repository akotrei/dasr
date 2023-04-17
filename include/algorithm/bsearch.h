#ifndef __DAST_BSEARCH_H__
#define __DAST_BSEARCH_H__

unsigned long dast_bsearch(void*         key,
                           void*         array,
                           unsigned long elems,
                           unsigned long elem_size,
                           int (*cmp_f)(void* o1, void* o2));

#endif /* __DAST_BSEARCH_H__ */