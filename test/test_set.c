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

unsigned long hash_int_ptr(void* x)
{
    unsigned long ux = **(int**)x;
    return ux;
}

int cmp_int_ptr(void* l, void* r) { return **(int**)l - **(int**)r; }

void copy_int_and_inc(void* src, void* dst) { *(int*)dst = (*(int*)src) + 1; }

void int_pointer_inc(void* obj)
{
    (**((int**)obj))++;
}

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

void test_dast_set_add_and_slots_and_key()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int k = 1;
    dast_set_add(&set, &k);
    TEST_ASSERT(set.elems == 1);
    TEST_ASSERT(set.capacity == 8);

    dast_array_t slots;
    dast_set_slots(&set, &slots);

    TEST_ASSERT(slots.elems == 1);
    for (int i = 0; i < slots.elems; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == 1);
    }
    dast_array_destroy(&slots);

    for (int i = 0; i < 4; i++)
    {
        dast_set_add(&set, &i);
    }

    dast_set_slots(&set, &slots);
    TEST_ASSERT(set.elems == 4);
    TEST_ASSERT(slots.elems == 4);
    for (int i = 0; i < slots.elems; i++)
    {

        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == i);
    }
    dast_array_destroy(&slots);

    int a = 16;
    dast_set_add(&set, &a);

    a = 17;
    dast_set_add(&set, &a);

    int expected[] = {0, 16, 1, 17, 2, 3};
    dast_set_slots(&set, &slots);
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(slots.elems == 6);
    for (int i = 0; i < slots.elems; i++)
    {

        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == expected[i]);
    }
    dast_array_destroy(&slots);

    dast_set_destroy(&set);
}

void test_dast_set_copy()
{
    dast_set_t set, new_set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int elems[] = {0, 1, 8};
    for (int i = 0; i < 3; i++)
    {
        dast_set_add(&set, &elems[i]);
    }

    dast_set_copy(&set, &new_set);

    int expected[] = {0, 8, 1};
    dast_array_t slots;
    dast_set_slots(&new_set, &slots);
    TEST_ASSERT(set.elems == 3);
    TEST_ASSERT(slots.elems == 3);
    for (int i = 0; i < slots.elems; i++)
    {

        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == expected[i]);
    }
    dast_array_destroy(&slots);

    dast_set_destroy(&set);
    dast_set_destroy(&new_set);
}

void test_dast_set_deepcopy()
{
    dast_set_t set, new_set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int elems[] = {0, 1, 8};
    for (int i = 0; i < 3; i++)
    {
        dast_set_add(&set, &elems[i]);
    }

    dast_set_deepcopy(&set, &new_set, copy_int_and_inc);

    int expected[] = {1, 9, 2};
    dast_array_t slots;
    dast_set_slots(&new_set, &slots);
    TEST_ASSERT(set.elems == 3);
    TEST_ASSERT(slots.elems == 3);
    for (int i = 0; i < slots.elems; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == expected[i]);
    }
    dast_array_destroy(&slots);

    dast_set_destroy(&set);
    dast_set_destroy(&new_set);
}

void test_dast_set_clear()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int elems[] = {0, 1, 8, 16, 24, 32};
    for (int i = 0; i < 6; i++)
    {
        dast_set_add(&set, &elems[i]);
    }
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(set.capacity == 16);

    dast_set_clear(&set);
    TEST_ASSERT(set.elems == 0);
    TEST_ASSERT(set.capacity == 16);

    dast_set_destroy(&set);
}

void test_dast_set_deepclear()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int*), hash_int_ptr, cmp_int_ptr);

    int elems[] = {0, 1, 8, 16, 24, 32};
    int elems_incremented[] = {1, 2, 9, 17, 25, 33};
    for (int i = 0; i < 6; i++)
    {
        int* int_ptr = &elems[i];
        dast_set_add(&set, &int_ptr);
    }
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(set.capacity == 16);

    dast_set_deepclear(&set, int_pointer_inc);
    TEST_ASSERT(set.elems == 0);
    TEST_ASSERT(set.capacity == 16);
    for (int i = 0; i < 6; i++)
    {
        TEST_ASSERT(elems[i] == elems_incremented[i]);
    }

    dast_set_destroy(&set);
}

void test_dast_set_get()
{
    dast_set_t set;
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int elems[] = {0, 1, 8, 16, 24, 32};
    for (int i = 0; i < 6; i++)
    {
        dast_set_add(&set, &elems[i]);
    }
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(set.capacity == 16);

    int key = 12;
    TEST_ASSERT(dast_set_get(&set, &key) == 0);
    for (int i = 0; i < 6; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(dast_set_get(&set, &elems[i])) == elems[i]);
    }
    
    dast_set_destroy(&set);
}

void test_dast_set_del()
{
    dast_set_t set;
    dast_array_t slots;
    
    dast_set_init(&set, sizeof(int), hash_int, cmp_int);

    int elems[] = {0, 1, 8, 16, 24, 32};
    for (int i = 0; i < 6; i++)
    {
        dast_set_add(&set, &elems[i]);
    }
    TEST_ASSERT(set.elems == 6);
    TEST_ASSERT(set.capacity == 16);

    int key = 24;
    int expected_without_24[] = {0, 16, 32, 1, 8};
    dast_set_del(&set, dast_set_get(&set, &key));
    dast_set_slots(&set, &slots);
    TEST_ASSERT(set.elems == 5);
    TEST_ASSERT(set.capacity == 16);
    for (int i = 0; i < 5; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == expected_without_24[i]);
    }
    dast_array_destroy(&slots);

    key = 16;
    int expected_without_24_and_16[] = {0, 32, 1, 8};
    dast_set_del(&set, dast_set_get(&set, &key));
    dast_set_slots(&set, &slots);
    TEST_ASSERT(slots.elems == 4);
    TEST_ASSERT(set.elems == 4);
    TEST_ASSERT(set.capacity == 16);
    for (int i = 0; i < 4; i++)
    {
        TEST_ASSERT(*(int*)DAST_SLOTE_KEY(DAST_ARRAY_ITH(&slots, i)) == expected_without_24_and_16[i]);
    }
    dast_array_destroy(&slots);

    dast_set_destroy(&set);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_set_init_and_destroy);
    RUN_TEST(test_dast_set_add_and_slots_and_key);
    RUN_TEST(test_dast_set_copy);
    RUN_TEST(test_dast_set_deepcopy);
    RUN_TEST(test_dast_set_clear);
    RUN_TEST(test_dast_set_deepclear);
    RUN_TEST(test_dast_set_get);
    RUN_TEST(test_dast_set_del);
    return UNITY_END();
}