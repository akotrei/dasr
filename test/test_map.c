#include <stdio.h>
#include <unity/unity.h>

#include "map.h"


void setUp(void) {}

void tearDown(void) {}

unsigned long hash_int(void* x)
{
    unsigned long ux = *(int*)x;
    ux = ((ux >> 16) ^ ux) * 0x45d9f3b;
    ux = ((ux >> 16) ^ ux) * 0x45d9f3b;
    ux = (ux >> 16) ^ ux;
    return ux;
}

int cmp_int(void* l, void* r) { return *(int*)l - *(int*)r; }

void test_dast_map_init_and_destroy()
{
    dast_map_t map;
    dast_map_init(&map, sizeof(int), sizeof(int), hash_int, cmp_int);
    TEST_ASSERT(map.elems == 0);
    TEST_ASSERT(map.capacity == DAST_ARRAY_INIT_SIZE);
    TEST_ASSERT(map.hash == hash_int);
    TEST_ASSERT(map.cmp == cmp_int);
    TEST_ASSERT(map.key_size == sizeof(int));
    TEST_ASSERT(map.val_size == sizeof(int));
    dast_map_destroy(&map);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_map_init_and_destroy);
    return UNITY_END();
}