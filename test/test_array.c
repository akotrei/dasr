#include "unity/unity.h"

#include "utils/allocator_std.h"
#include "array.h"
#include "utils/mem.h"

#include <stdio.h>


dast_allocator_t* allocator;

void setUp(void) {}

void tearDown(void) {}

void copy_int_and_inc(void* src, void* dst) { *(int*)dst = (*(int*)src) + 1; }
void int_pointer_inc(void* obj) { (**((int**)obj))++; }

void test_dast_array_init_and_destroy()
{
    dast_array_t array;
    dast_array_init(&array, allocator, 10);
    TEST_ASSERT(array.allocator == allocator);
    TEST_ASSERT(array.elem_size == 10);
    TEST_ASSERT(array.capacity == DAST_ARRAY_INIT_SIZE);
    TEST_ASSERT(array.elems == 0);
    dast_array_destroy(&array);
}

void test_dast_array_shrink_and_append()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));
    int obj = 42;
    dast_array_shrink(&array);
    TEST_ASSERT(array.elems == 0);
    TEST_ASSERT(array.capacity == DAST_ARRAY_INIT_SIZE);

    dast_array_append(&array, &obj);
    dast_array_shrink(&array);
    TEST_ASSERT(array.elems == 1);
    TEST_ASSERT(*((int*)(array.data)) == obj);
    TEST_ASSERT(array.capacity == DAST_ARRAY_INIT_SIZE);

    for (int i = 0; i < DAST_ARRAY_INIT_SIZE; i++)
    {
        dast_array_append(&array, &obj);
    }
    TEST_ASSERT(array.elems == 9);
    TEST_ASSERT(array.capacity == 2*DAST_ARRAY_INIT_SIZE);
    dast_array_shrink(&array);
    TEST_ASSERT(array.elems == 9);
    TEST_ASSERT(array.capacity == 9);

    dast_array_destroy(&array);
}

void test_dast_array_reserve()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));
    
    dast_array_reserve(&array, 2);
    TEST_ASSERT(array.elems == 0);
    TEST_ASSERT(array.capacity == DAST_ARRAY_INIT_SIZE);

    dast_array_reserve(&array, 10);
    TEST_ASSERT(array.elems == 0);
    TEST_ASSERT(array.capacity == 10);

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }
    TEST_ASSERT(array.elems == 10);
    TEST_ASSERT(array.capacity == 10);

    for (int i = 0; i < 1; i++)
    {
        dast_array_append(&array, &i);
    }
    TEST_ASSERT(array.elems == 11);
    TEST_ASSERT(array.capacity == 10*DAST_ARRAY_GROW_FACTOR);

    dast_array_reserve(&array, 5);
    TEST_ASSERT(array.elems == 11);
    TEST_ASSERT(array.capacity == 10*DAST_ARRAY_GROW_FACTOR);

    dast_array_reserve(&array, 11*DAST_ARRAY_GROW_FACTOR);
    TEST_ASSERT(array.elems == 11);
    TEST_ASSERT(array.capacity == 11*DAST_ARRAY_GROW_FACTOR + 11);

    dast_array_destroy(&array);
}

void test_dast_array_copy()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }

    dast_array_t new_array;
    dast_array_copy(&array, &new_array);

    TEST_ASSERT(new_array.elems == array.elems);
    TEST_ASSERT(new_array.elem_size == array.elem_size);
    TEST_ASSERT(new_array.capacity == array.capacity);
    TEST_ASSERT(new_array.allocator == array.allocator);
    for (int i = 0; i < 10; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&new_array, i) == i);
    }

    dast_array_destroy(&array);
}

void test_dast_array_deepcopy()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }

    dast_array_t new_array;
    dast_array_deepcopy(&array, &new_array, copy_int_and_inc);

    TEST_ASSERT(new_array.elems == array.elems);
    TEST_ASSERT(new_array.elem_size == array.elem_size);
    TEST_ASSERT(new_array.capacity == array.capacity);
    TEST_ASSERT(new_array.allocator == array.allocator);
    for (int i = 0; i < 10; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&new_array, i) == i + 1);
    }

    dast_array_destroy(&array);
}

void test_dast_array_clear()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));
    
    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }

    TEST_ASSERT(array.elems == 10);
    dast_array_clear(&array);
    TEST_ASSERT(array.elems == 0);

    dast_array_destroy(&array);
}

void test_dast_array_deepclear()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int*));
    
    int v = 42;
    int* pv = &v;
    dast_array_append(&array, &pv);

    TEST_ASSERT(array.elems == 1);
    dast_array_deepclear(&array, int_pointer_inc);
    TEST_ASSERT(array.elems == 0);
    TEST_ASSERT(v == 43);

    dast_array_destroy(&array);
}

void test_dast_array_first_last_ith()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));
    
    for (int i = 0; i < 3; i++)
    {
        dast_array_append(&array, &i);
    }
    TEST_ASSERT(*(int*)dast_array_first(&array) == 0);
    TEST_ASSERT(*(int*)dast_array_last(&array) == 2);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 1) == 1);

    dast_array_destroy(&array);
}

void test_dast_array_extend()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));
    
    int objs[] = {1, 2, 3};
    dast_array_extend(&array, objs, 3);
    for (int i = 0; i < 3; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&array, i) == objs[i]);
    }
    TEST_ASSERT(array.elems == 3);

    dast_array_extend(&array, objs, 3);
    dast_array_extend(&array, objs, 3);
    for (int i = 3; i < 9; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&array, i) == i % 3 + 1);
    }
    TEST_ASSERT(array.elems == 9);
    
    dast_array_destroy(&array);
}

void test_dast_array_insert()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    int val = 12;
    dast_array_insert(&array, &val, 100);
    TEST_ASSERT(array.elems == 1);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 0) == val);

    int val2 = 42;
    int val3 = 65;
    dast_array_insert(&array, &val2, 0);
    dast_array_insert(&array, &val3, 1);
    TEST_ASSERT(array.elems == 3);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 0) == val2);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 1) == val3);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 2) == val);

    dast_array_destroy(&array);
}

void test_dast_array_pop()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    int dst = -1;    
    for (int i = 0; i < 1; i++)
    {
        dast_array_append(&array, &i);
    }
    dast_array_pop(&array, &dst);
    TEST_ASSERT(dst == 0);
    TEST_ASSERT(array.elems == 0);

    for (int i = 0; i < 3; i++)
    {
        dast_array_append(&array, &i);
    }
    dast_array_pop(&array, &dst);
    TEST_ASSERT(dst == 2);
    TEST_ASSERT(array.elems == 2);

    dast_array_destroy(&array);
}

void test_dast_array_remove()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    int dst = -1;    
    for (int i = 0; i < 1; i++)
    {
        dast_array_append(&array, &i);
    }
    dast_array_remove(&array, 0, &dst);
    TEST_ASSERT(dst == 0);
    TEST_ASSERT(array.elems == 0);

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }
    dast_array_remove(&array, 5, &dst);
    TEST_ASSERT(dst == 5);
    TEST_ASSERT(array.elems == 9);

    for (int i = 0; i < 5; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&array, i) == i);
    }
    for (int i = 5; i < 9; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&array, i) == i + 1);
    }

    dast_array_destroy(&array);
}

void test_dast_array_replace()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(&array, &i);
    }

    int dst = -5;
    dast_array_replace(&array, &dst, 0);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 0) == dst);
    TEST_ASSERT(array.elems == 10);

    dast_array_replace(&array, &dst, 5);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 5) == dst);
    TEST_ASSERT(array.elems == 10);

    dast_array_replace(&array, &dst, 9);
    TEST_ASSERT(*(int*)dast_array_ith(&array, 9) == dst);
    TEST_ASSERT(array.elems == 10);

    dast_array_destroy(&array);
}

void test_dast_array_reverse()
{
    dast_array_t array;
    dast_array_init(&array, allocator, sizeof(int));

    for (int i = 0; i < 5; i++)
    {
        dast_array_append(&array, &i);
    }
    dast_array_reverse(&array);
    for (int i = 0; i < 5; i++)
    {
        TEST_ASSERT(*(int*)dast_array_ith(&array, i) == 4 - i);
    }

    dast_array_destroy(&array);
}

// void test_dast_array_copy()
// {
//     dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
//     dast_array_t copied_array;
//     dast_array_copy_to(array, &copied_array, sizeof(dast_array_t));

//     -------------
    

//     dast_array_destroy(array);
// }

int main()
{
    char alloc_mem[dast_allocator_std_sizeof()];
    allocator = dast_allocator_std_init(alloc_mem);

    UNITY_BEGIN();
    RUN_TEST(test_dast_array_init_and_destroy);
    RUN_TEST(test_dast_array_shrink_and_append);
    RUN_TEST(test_dast_array_reserve);
    RUN_TEST(test_dast_array_copy);
    RUN_TEST(test_dast_array_deepcopy);
    RUN_TEST(test_dast_array_clear);
    RUN_TEST(test_dast_array_deepclear);
    RUN_TEST(test_dast_array_first_last_ith);
    RUN_TEST(test_dast_array_extend);
    RUN_TEST(test_dast_array_insert);
    RUN_TEST(test_dast_array_pop);
    RUN_TEST(test_dast_array_remove);
    RUN_TEST(test_dast_array_replace);
    RUN_TEST(test_dast_array_reverse);
    return UNITY_END();
}