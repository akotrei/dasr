#ifndef __DAST_BSEARCH_H__
#define __DAST_BSEARCH_H__

long dast_bsearch(void* key, void* array, long elems, int elem_size, int (*cmp)(void* l, void* r));

#endif // __DAST_BSEARCH_H__