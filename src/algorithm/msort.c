#include "algorithm/qsort.h"
#include "memory.h"

#define COPY(dst, src, size)\
do\
{\
    int __size = (size);\
    char *__dst = (dst), *__src = (src);\
    do\
    {\
        *__dst++ = *__src++;\
    } while (--__size > 0);\
} while (0)

static void dast_msort_internal(void* array, void* dst, long start, long end, int elem_size, int (*cmp)(void* l, void* r));

void dast_msort(void* array, void* dst, long elems, int elem_size, int (*cmp)(void* l, void* r))
{
    DAST_MEMCPY(dst, array, elems*elem_size);
    dast_msort_internal(array, dst, 0, elems, elem_size, cmp);
}

void dast_msort_internal(void* array, void* dst, long start, long end, int elem_size, int (*cmp)(void* l, void* r))
{
    if (end - start <= 1)
    {
        return;
    }
    long middle = (start + end) / 2;
    long i = start;
    long j = middle;
    dast_msort_internal(dst, array, start, middle, elem_size, cmp);
    dast_msort_internal(dst, array, middle, end, elem_size, cmp);

    for(long k = start; k < end; k++)
    {
        if ((j < end) && (i >= middle || (cmp((char*)array + elem_size*i, (char*)array + elem_size*j) > 0)))
        {
            COPY((char*)dst + elem_size*k, (char*)array + elem_size*j, elem_size);
            j++;
        }
        else
        {
            COPY((char*)dst + elem_size*k, (char*)array + elem_size*i, elem_size);
            i++;
        }
    }
}

// dast_msort_int
void dast_msort_int_internal(int* array, int* dst, long start, long end)
{
    if (end - start <= 1)
    {
        return;
    }
    long middle = (start + end) / 2;
    long i = start;
    long j = middle;
    dast_msort_int_internal(dst, array, start, middle);
    dast_msort_int_internal(dst, array, middle, end);

    for(long k = start; k < end; k++)
    {
        if ((j < end) && (i >= middle || (array[i] > array[j])))
        {
            dst[k] = array[j++];
        }
        else
        {
            dst[k] = array[i++];
        }
    }
}

void dast_msort_int(int* array, int* dst, long elems)
{
    DAST_MEMCPY(dst, array, elems*sizeof(int));
    dast_msort_int_internal(dst, array, 0, elems);
}
// dast_msort_int end