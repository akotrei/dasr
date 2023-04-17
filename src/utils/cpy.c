#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <time.h>


void cpy_std(void* dst, void* src, int size)
{
    memcpy(dst, src, size);

}

void cpy_custom(void* dst, void* src, long size)
{
    char *d = dst;
    char *s = src;
    long step = sizeof(char);
    
    for (long i = 0; i < size; i++)
    {
        d[i] = s[i];
    }
}

void cpy_custom_int(void* dst, void* src, long size)
{
    int *d = dst;
    int *s = src;
    int step = sizeof(int);
    // size &= ~15;
    size /= step;
    
    for (long i = 0; i < size; i++)
    {
        d[i] = s[i];
    }
}

void cpy_custom_long(void* dst, void* src, long size)
{
    long *d = dst;
    long *s = src;
    long step = sizeof(long);
    // size &= ~15;
    size /= step;
    
    for (long i; i < size; i++)
    {
        d[i] = s[i];
    }
}

void cpy_custom_simd(void* dst, void* src, long size)
{
    char *d = dst;
    char *s = src;
    long step = 32;
    int max = 0xffffffff;

    __m256i tmp;
    __m256i mask = _mm256_set_epi32(max, max, max, max, max, max, max, max);
    
    for (long i = 0; i < size; i+= step)
    {
        tmp = _mm256_maskload_epi64((long long const *)(s + i), mask);
        _mm256_store_si256(d + i, tmp);
    }
}

int main()
{
    long size = 1e9;

    // printf("sizeof(long): %lu\n", sizeof(long));

    void* src, *dst;

    src = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    dst = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    for (long i = 0; i < size / 8; i++)
    {
        ((long*)src)[i] = i;
    }
    // printf("sizeof(long): %lu\n", sizeof(long));
    // src = malloc(size);
    // dst = malloc(size);

    clock_t begin = clock();

    // cpy_custom(dst, src, size);
    // cpy_custom_int(dst, src, size);
    // cpy_custom_long(dst, src, size);
    cpy_custom_simd(dst, src, size);
    // cpy_std(dst, src, size);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time_spent: %f\n", time_spent);

    FILE *f_src = fopen("src.bin", "wb");
    FILE *f_dst = fopen("dst.bin", "wb");

    // fwrite(src, 1, size, f_src);
    // fwrite(dst, 1, size, f_dst);
    

    return 0;
}