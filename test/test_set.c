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

void test_dast_set_add_and_slots()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int k = 1;
    dast_set_add(&set, &k);
    TEST_ASSERT(set.elems == 1);
    TEST_ASSERT(set.capacity == 8);

    dast_array_t* slots = dast_set_slots(&set);

    TEST_ASSERT(slots->elems == 1);
    for (int i = 0; i < slots->elems; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(slots, i)) == 1);
    }
    dast_array_destroy(slots);

    for (int i = 0; i < 4; i++)
    {
        dast_set_add(&set, &i);
    }

    slots = dast_set_slots(&set);
    TEST_ASSERT(set.elems == 4);
    TEST_ASSERT(slots->elems == 4);
    for (int i = 0; i < slots->elems; i++)
    {

        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(slots, i)) == i);
    }
    dast_array_destroy(slots);

    int a = 16;
    dast_set_add(&set, &a);

    a = 17;
    dast_set_add(&set, &a);

    int expected[] = {0, 16, 1, 17, 2, 3};
    slots = dast_set_slots(&set);
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(slots->elems == 6);
    for (int i = 0; i < slots->elems; i++)
    {

        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(slots, i)) == expected[i]);
    }
    dast_array_destroy(slots);
    DAST_FREE(slots);

    dast_set_destroy(&set);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_set_init_and_destroy);
    RUN_TEST(test_dast_set_add_and_slots_and_key);
    return UNITY_END();
}