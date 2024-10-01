#include "algorithm/qsort.h"
#include "memory.h"

#define SWAP(a, b, size)\
    do\
    {\
        if(size == sizeof(int))\
        {\
            int itmp = *(int*)(a);\
            *(int*)(a) = *(int*)(b);\
            *(int*)(b) = itmp;\
        }\
        else if(size == sizeof(long))\
        {\
            long itmp = *(long*)(a);\
            *(long*)(a) = *(long*)(b);\
            *(long*)(b) = itmp;\
        }\
        else\
        {\
            size_t __size = (size);\
            char *__a = (a), *__b = (b);\
            do\
            {\
                char __tmp = *__a;\
                *__a++ = *__b;\
                *__b++ = __tmp;\
            } while (--__size > 0);\
        }\
    } while (0)

static void dast_qsort_internal(void* array, long start, long end, int elem_size, int (*cmp)(void* l, void* r));
static long dast_partition(void* array, long start, long end, int elem_size, int (*cmp)(void* l, void* r));

void dast_qsort(void* array, long elems, int elem_size, int (*cmp)(void* l, void* r))
{
    dast_qsort_internal(array, 0, elems-1, elem_size, cmp);
}

void dast_qsort_internal(void* array, long start, long end, int elem_size, int (*cmp)(void* l, void* r))
{
    if (start < end)
    {
        long middle = dast_partition(array, start, end, elem_size, cmp);
        dast_qsort_internal(array, start, middle-1, elem_size, cmp);
        dast_qsort_internal(array, middle+1, end, elem_size, cmp);
    }
}

static long dast_partition(void* array, long start, long end, int elem_size, int (*cmp)(void* l, void* r))
{
    void* pivot = (char*)array + start*elem_size;
    long k = end;
    for (long i = end; i > start; i--)
    {
        if (cmp((char*)array + i * elem_size, pivot) > 0)
        {
            SWAP((char*)array + i * elem_size, (char*)array + k * elem_size, elem_size);
            k--;
        }
    }
    SWAP((char*)array + k * elem_size, (char*)array + start * elem_size, elem_size);
    return k;
}