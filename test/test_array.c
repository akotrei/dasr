#include "unity/unity.h"

#include "utils/allocator_std.h"
#include "array/array_private.h"
#include "utils/mem.h"

#include <stdio.h>

char              alloc_mem[sizeof(dast_allocator_t)];
dast_allocator_t* allocator;

void setUp(void) {}

void tearDown(void) {}

void test_dast_array_sizeof() { TEST_ASSERT(sizeof(dast_array_t) == dast_array_sizeof()); }

void test_dast_array_init_on_and_destroy_from()
{
    dast_u64_t   bytes = dast_array_sizeof();
    char         memory[bytes];
    dast_array_t* array = dast_array_init_on(memory, allocator, 10, (dast_cpy_t)111, (dast_del_t)222);
    TEST_ASSERT(array->allocator == allocator);
    TEST_ASSERT(array->cpy == (dast_cpy_t)111);
    TEST_ASSERT(array->del == (dast_del_t)222);
    TEST_ASSERT(array->elem_size == 10);
    TEST_ASSERT(array->factor == DAST_ARRAY_GROW_FACTOR);
    TEST_ASSERT(array->capacity == DAST_ARRAY_INIT_SIZE);
    TEST_ASSERT(array->elems == 0);
    dast_array_destroy_from(array);
}

void test_dast_array_init_and_destroy()
{
    dast_array_t* array = dast_array_init(allocator, 10, (dast_cpy_t)111, (dast_del_t)222);
    TEST_ASSERT(array->allocator == allocator);
    TEST_ASSERT(array->cpy == (dast_cpy_t)111);
    TEST_ASSERT(array->del == (dast_del_t)222);
    TEST_ASSERT(array->elem_size == 10);
    TEST_ASSERT(array->factor == DAST_ARRAY_GROW_FACTOR);
    TEST_ASSERT(array->capacity == DAST_ARRAY_INIT_SIZE);
    TEST_ASSERT(array->elems == 0);
    dast_array_destroy(array);
}

void test_dast_array_shrink_and_append()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    int obj = 42;
    dast_array_shrink(array);
    TEST_ASSERT(array->elems == 0);
    TEST_ASSERT(array->capacity == DAST_ARRAY_INIT_SIZE);

    dast_array_append(array, &obj);
    dast_array_shrink(array);
    TEST_ASSERT(array->elems == 1);
    TEST_ASSERT(array->capacity == DAST_ARRAY_INIT_SIZE);

    for (int i = 0; i < DAST_ARRAY_INIT_SIZE; i++)
    {
        dast_array_append(array, &obj);
    }
    TEST_ASSERT(array->elems == 9);
    TEST_ASSERT(array->capacity == 2*DAST_ARRAY_INIT_SIZE);
    dast_array_shrink(array);
    TEST_ASSERT(array->elems == 9);
    TEST_ASSERT(array->capacity == 9);

    dast_array_destroy(array);
}

void test_dast_array_reserve()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    
    dast_array_reserve(array, 2);
    TEST_ASSERT(array->elems == 0);
    TEST_ASSERT(array->capacity == DAST_ARRAY_INIT_SIZE);

    dast_array_reserve(array, 10);
    TEST_ASSERT(array->elems == 0);
    TEST_ASSERT(array->capacity == 10);

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(array, &i);
    }
    TEST_ASSERT(array->elems == 10);
    TEST_ASSERT(array->capacity == 10);

    for (int i = 0; i < 1; i++)
    {
        dast_array_append(array, &i);
    }
    TEST_ASSERT(array->elems == 11);
    TEST_ASSERT(array->capacity == 10*DAST_ARRAY_GROW_FACTOR);

    dast_array_reserve(array, 5);
    TEST_ASSERT(array->elems == 11);
    TEST_ASSERT(array->capacity == 10*DAST_ARRAY_GROW_FACTOR);

    dast_array_destroy(array);
}

void test_dast_array_size()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    TEST_ASSERT(array->elems == dast_array_size(array));
    for (int i = 0; i < 10; i++)
    {
        dast_array_append(array, &i);
        TEST_ASSERT(array->elems == dast_array_size(array));
    }
    dast_array_destroy(array);
}

void test_dast_array_obj_size()
{
    dast_array_t* array = dast_array_init(allocator, 12, dast_cpy_generic, dast_del_dummy);
    TEST_ASSERT(12 == dast_array_obj_size(array));
    dast_array_destroy(array);
}

void test_dast_array_clear()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    
    for (int i = 0; i < 10; i++)
    {
        dast_array_append(array, &i);
    }
    TEST_ASSERT(array->elems == 10);
    dast_array_clear(array);
    TEST_ASSERT(array->elems == 0);

    dast_array_destroy(array);
}

void test_dast_array_begin_and_end()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    
    for (int i = 0; i < 3; i++)
    {
        dast_array_append(array, &i);
    }
    TEST_ASSERT(*(int*)dast_array_begin(array) == 0);
    TEST_ASSERT(dast_array_end(array) - dast_array_begin(array) == 3*sizeof(int));

    dast_array_destroy(array);
}

void test_dast_array_index()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    
    for (int i = 0; i < 5; i++)
    {
        dast_array_append(array, &i);
    }
    TEST_ASSERT(*(int*)dast_array_index(array, 1) == 1);
    TEST_ASSERT(*(int*)dast_array_index(array, 4) == 4);

    dast_array_destroy(array);
}

void test_dast_array_extend()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);
    
    int objs[] = {1, 2, 3};
    dast_array_extend(array, objs, 3);
    for (int i = 0; i < 3; i++)
    {
        TEST_ASSERT(*(int*)dast_array_index(array, i) == i+1);
    }
    TEST_ASSERT(array->elems == 3);

    dast_array_extend(array, objs, 3);
    dast_array_extend(array, objs, 3);
    for (int i = 3; i < 9; i++)
    {
        TEST_ASSERT(*(int*)dast_array_index(array, i) == i % 3 + 1);
    }
    TEST_ASSERT(array->elems == 9);
    
    dast_array_destroy(array);
}

void test_dast_array_pop()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);

    int dst = -1;
    dast_u8_t status;
    status = dast_array_pop(array, &dst);
    TEST_ASSERT(status == 0);
    
    for (int i = 0; i < 1; i++)
    {
        dast_array_append(array, &i);
    }
    status = dast_array_pop(array, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 0);
    TEST_ASSERT(array->elems == 0);

    for (int i = 0; i < 3; i++)
    {
        dast_array_append(array, &i);
    }
    status = dast_array_pop(array, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 2);
    TEST_ASSERT(array->elems == 2);

    dast_array_destroy(array);
}

void test_dast_array_remove()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);

    int dst = -1;
    dast_u8_t status;
    status = dast_array_remove(array, 0, &dst);
    TEST_ASSERT(status == 0);
    
    for (int i = 0; i < 1; i++)
    {
        dast_array_append(array, &i);
    }
    status = dast_array_remove(array, 0, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 0);
    TEST_ASSERT(array->elems == 0);

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(array, &i);
    }
    status = dast_array_remove(array, 5, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 5);
    TEST_ASSERT(array->elems == 9);

    dast_array_destroy(array);
}

void test_dast_array_replace()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);

    int dst = 42;
    dast_u8_t status;

    status = dast_array_replace(array, &dst, 0);
    TEST_ASSERT(status == 0);

    for (int i = 0; i < 10; i++)
    {
        dast_array_append(array, &i);
    }

    status = dast_array_replace(array, &dst, 0);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(*(int*)dast_array_index(array, 0) == dst);
    TEST_ASSERT(array->elems == 10);

    status = dast_array_replace(array, &dst, 5);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(*(int*)dast_array_index(array, 5) == dst);
    TEST_ASSERT(array->elems == 10);

    status = dast_array_replace(array, &dst, 9);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(*(int*)dast_array_index(array, 9) == dst);
    TEST_ASSERT(array->elems == 10);

    status = dast_array_replace(array, &dst, 10);
    TEST_ASSERT(status == 0);
    TEST_ASSERT(array->elems == 10);

    dast_array_destroy(array);
}

void test_dast_array_reverse()
{
    dast_array_t* array = dast_array_init(allocator, sizeof(int), dast_cpy_generic, dast_del_dummy);

    for (int i = 0; i < 5; i++)
    {
        dast_array_append(array, &i);
    }
    dast_array_reverse(array);
    for (int i = 0; i < 5; i++)
    {
        TEST_ASSERT(*(int*)dast_array_index(array, i) == 4 - i);
    }

    dast_array_destroy(array);
}

int main()
{
    allocator = dast_allocator_std_init(alloc_mem);
    UNITY_BEGIN();
    RUN_TEST(test_dast_array_sizeof);
    RUN_TEST(test_dast_array_init_on_and_destroy_from);
    RUN_TEST(test_dast_array_init_and_destroy);
    RUN_TEST(test_dast_array_shrink_and_append);
    RUN_TEST(test_dast_array_reserve);
    RUN_TEST(test_dast_array_size);
    RUN_TEST(test_dast_array_obj_size);
    RUN_TEST(test_dast_array_clear);
    RUN_TEST(test_dast_array_begin_and_end);
    RUN_TEST(test_dast_array_index);
    RUN_TEST(test_dast_array_extend);
    RUN_TEST(test_dast_array_pop);
    RUN_TEST(test_dast_array_remove);
    RUN_TEST(test_dast_array_replace);
    RUN_TEST(test_dast_array_reverse);
    return UNITY_END();
}