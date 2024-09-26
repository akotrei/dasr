#include <stdio.h>
#include <unity/unity.h>

#include "set.h"


void setUp(void) {}

void tearDown(void) {}

unsigned long hash_int(void* x)
{
    unsigned long ux = *(int*)x;
    return ux;
}

int cmp_int(void* l, void* r) { return *(int*)l - *(int*)r; }

void test_dast_set_init_and_destroy()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);
    TEST_ASSERT(set.elems == 0);
    TEST_ASSERT(set.capacity == DAST_ARRAY_INIT_SIZE);
    TEST_ASSERT(set.hash == hash_int);
    TEST_ASSERT(set.cmp == cmp_int);
    TEST_ASSERT(set.key_size == sizeof(int));
    dast_set_destroy(&set);
}

void test_dast_set_set()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int k = 1;
    dast_set_add(&set, &k);
    TEST_ASSERT(set.elems == 1);
    TEST_ASSERT(set.capacity == 8);

    for (int i = 2; i < 2000001; i++)
    {
        dast_set_add(&set, &i);
    }
    TEST_ASSERT(set.elems == 2000000);
    TEST_ASSERT(set.capacity == 4194304);

    int max_in_bucket = 0;
    for (int i = 0; i < set.capacity; i++)
    {
        if (set.buckets_info[i].elems > max_in_bucket)
        {
            max_in_bucket = set.buckets_info[i].elems;
        }
        // printf("i: %d; cap: %lu; elems: %lu; h: %lu\n", i, map.buckets_info[i].capacity, map.buckets_info[i].elems, hash_int(&i) % map.capacity);
    }
    printf("max_in_bucket: %d\n", max_in_bucket);

    dast_set_destroy(&set);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_set_init_and_destroy);
    RUN_TEST(test_dast_set_set);
    return UNITY_END();
}