#include "unity/unity.h"

#include "utils/allocator_std.h"

#include <stdio.h>

void setUp(void) {}

void tearDown(void) {}

void test_dast_allocator_std_sizeof() { TEST_ASSERT(sizeof(dast_allocator_t) == dast_allocator_std_sizeof()); }

void test_dast_allocator_std_init()
{
    dast_u64_t        alloc_size = dast_allocator_std_sizeof();
    dast_u8_t         mem[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem);
    TEST_ASSERT((dast_u8_t*)allocator == (dast_u8_t*)mem);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_allocator_std_sizeof);
    RUN_TEST(test_dast_allocator_std_init);
    return UNITY_END();
}