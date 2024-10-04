#include "algorithm/qsort.h"
#include "memory.h"

#define SWAP(a, b, size)\
    do\
    {\
        int __size = (size);\
        char *__a = (a), *__b = (b);\
        do\
        {\
            char __tmp = *__a;\
            *__a++ = *__b;\
            *__b++ = __tmp;\
        } while (--__size > 0);\
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

/// dast_qsort_int
static long dast_partition_int(int* array, long start, long end)
{
    int pivot = array[start];
    int tmp;
    long k = end;
    for (long i = end; i > start; i--)
    {
        if (array[i] > pivot)
        {
            tmp = array[i];
            array[i] = array[k];
            array[k] = tmp;
            k--;
        }
    }
    tmp = array[start];
    array[start] = array[k];
    array[k] = tmp;
    return k;
}

static void dast_qsort_int_internal(int* array, long start, long end)
{
    if (start < end)
    {
        long middle = dast_partition_int(array, start, end);
        dast_qsort_int_internal(array, start, middle-1);
        dast_qsort_int_internal(array, middle+1, end);
    }
}

void dast_qsort_int(int* array, long elems)
{
    dast_qsort_int_internal(array, 0, elems-1);
}
/// dast_qsort_int end