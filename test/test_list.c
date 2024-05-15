#include "unity/unity.h"

#include "list/list_private.h"
#include "utils/allocator_std.h"

char              alloc_mem[sizeof(dast_allocator_t)];
dast_allocator_t* allocator;

void setUp(void) { allocator = dast_allocator_std_init(alloc_mem); }

void tearDown(void) {}

void test_dast_list_sizeof() { TEST_ASSERT(sizeof(dast_list_t) == dast_list_sizeof()); }

void test_dast_list_init_on()
{
    dast_u64_t   bytes = dast_list_sizeof();
    char         memory[bytes];
    dast_list_t* list = dast_list_init_on(memory, allocator, 10, (dast_cpy_t)111, (dast_del_t)222);
    TEST_ASSERT(list->allocator == allocator);
    TEST_ASSERT(list->cpy == (dast_cpy_t)111);
    TEST_ASSERT(list->del == (dast_del_t)222);
    TEST_ASSERT(list->elem_size == 10);
    TEST_ASSERT(list->elems == 0);
}

void test_dast_list_init()
{
    dast_list_t* list = dast_list_init(allocator, 10, (dast_cpy_t)111, (dast_del_t)222);
    TEST_ASSERT(list->allocator == allocator);
    TEST_ASSERT(list->cpy == (dast_cpy_t)111);
    TEST_ASSERT(list->del == (dast_del_t)222);
    TEST_ASSERT(list->elem_size == 10);
    TEST_ASSERT(list->elems == 0);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_list_sizeof);
    RUN_TEST(test_dast_list_init_on);
    RUN_TEST(test_dast_list_init);
    return UNITY_END();
}