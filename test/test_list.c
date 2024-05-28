#include "unity/unity.h"

#include "list/list_private.h"
#include "utils/allocator_std.h"
#include "utils/mem.h"

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
    dast_list_destroy_from(list);
}

void test_dast_list_init()
{
    dast_list_t* list = dast_list_init(allocator, 10, (dast_cpy_t)111, (dast_del_t)222);
    TEST_ASSERT(list->allocator == allocator);
    TEST_ASSERT(list->cpy == (dast_cpy_t)111);
    TEST_ASSERT(list->del == (dast_del_t)222);
    TEST_ASSERT(list->elem_size == 10);
    TEST_ASSERT(list->elems == 0);
    dast_list_destroy(list);
}

void test_dast_list_append()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98;

    TEST_ASSERT(list->head == 0);
    TEST_ASSERT(list->tail == 0);

    dast_list_append(list, &v1);
    TEST_ASSERT(list->elems == 1);
    TEST_ASSERT(list->head == list->tail);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->head + sizeof(dast_node_t)) == v1);

    dast_list_append(list, &v2);
    TEST_ASSERT(list->elems == 2);
    TEST_ASSERT(list->head->next == list->tail);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->tail + sizeof(dast_node_t)) == v2);
    dast_list_destroy(list);
}

void test_dast_list_size()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42;
    dast_list_append(list, &v1);
    dast_list_append(list, &v1);
    dast_list_append(list, &v1);
    TEST_ASSERT(list->elems == 3);
    dast_list_destroy(list);
}

void test_dast_list_obj_size()
{
    dast_list_t* list = dast_list_init(allocator, 777, dast_cpy_generic, dast_del_dummy);
    TEST_ASSERT(list->elem_size == 777);
    dast_list_destroy(list);
}

void test_dast_list_clear()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42;
    dast_list_append(list, &v1);
    dast_list_clear(list);
    TEST_ASSERT(list->elems == 0);
    dast_list_destroy(list);
}

void test_dast_list_front()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42;
    dast_list_append(list, &v1);
    TEST_ASSERT(*(dast_u32_t*)dast_list_front(list) == v1);
    dast_list_destroy(list);
}

void test_dast_list_back()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 97;
    dast_list_append(list, &v1);
    dast_list_append(list, &v2);
    TEST_ASSERT(*(dast_u32_t*)dast_list_back(list) == v2);
    dast_list_destroy(list);
}

void test_dast_list_iterator()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t values[3] = {1, 2, 3};
    dast_list_append(list, &values[0]);
    dast_list_append(list, &values[1]);
    dast_list_append(list, &values[2]);
    dast_iterator_t* iter = dast_list_iterator_new(list, 0);
    dast_u32_t i = 0;
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->next(iter);
        i++;
    }
    TEST_ASSERT(i == 3);
    dast_list_iterator_delete(iter);
    dast_list_destroy(list);
}

void test_dast_list_iterator_back()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t values[3] = {1, 2, 3};
    dast_list_append(list, &values[0]);
    dast_list_append(list, &values[1]);
    dast_list_append(list, &values[2]);
    dast_iterator_t* iter = dast_list_iterator_new(list, 0);
    dast_u32_t i = 0;
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->prev(iter);
        i++;
    }
    TEST_ASSERT(i == 1);
    dast_list_iterator_delete(iter);
    dast_list_destroy(list);
}

void test_dast_list_iterator_reversed()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t values[3] = {1, 2, 3};
    dast_list_append(list, &values[0]);
    dast_list_append(list, &values[1]);
    dast_list_append(list, &values[2]);
    dast_iterator_t* iter = dast_list_iterator_new(list, 1);
    dast_u32_t i = 2;
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->prev(iter);
        i--;
    }
    TEST_ASSERT(i == -1);
    dast_list_iterator_delete(iter);
    dast_list_destroy(list);
}

void test_dast_list_iterator_reversed_next()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t values[3] = {1, 2, 3};
    dast_list_append(list, &values[0]);
    dast_list_append(list, &values[1]);
    dast_list_append(list, &values[2]);
    dast_iterator_t* iter = dast_list_iterator_new(list, 1);
    dast_u32_t i = 2;
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->next(iter);
        i--;
    }
    TEST_ASSERT(i == 1);
    dast_list_iterator_delete(iter);
    dast_list_destroy(list);
}

void test_dast_list_iterator_reset()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t values[3] = {1, 2, 3};
    dast_list_append(list, &values[0]);
    dast_list_append(list, &values[1]);
    dast_list_append(list, &values[2]);
    dast_iterator_t* iter = dast_list_iterator_new(list, 0);
    dast_u32_t i = 0;
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->next(iter);
        i++;
    }
    TEST_ASSERT(i == 3);
    dast_list_iterator_reset(iter);
    while (iter->elem)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i-3]);
        iter->next(iter);
        i++;
    }
    TEST_ASSERT(i == 6);
    dast_list_iterator_delete(iter);
    dast_list_destroy(list);
}

void test_dast_list_prepend()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98;

    TEST_ASSERT(list->head == 0);
    TEST_ASSERT(list->tail == 0);

    dast_list_prepend(list, &v1);
    TEST_ASSERT(list->elems == 1);
    TEST_ASSERT(list->head == list->tail);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->head + sizeof(dast_node_t)) == v1);

    dast_list_prepend(list, &v2);
    TEST_ASSERT(list->elems == 2);
    TEST_ASSERT(list->head->next == list->tail);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->head + sizeof(dast_node_t)) == v2);
    dast_list_destroy(list);
}

void test_dast_list_replace_front()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98, v3 = 102;
    dast_u8_t status;

    status = dast_list_replace_front(list, &v3);
    TEST_ASSERT(status == 0);

    dast_list_append(list, &v1);
    dast_list_append(list, &v2);
    status = dast_list_replace_front(list, &v3);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->head + sizeof(dast_node_t)) == v3);
    TEST_ASSERT(status == 1);

    dast_list_destroy(list);
}

void test_dast_list_replace_back()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98, v3 = 102;
    dast_u8_t status;

    status = dast_list_replace_back(list, &v3);
    TEST_ASSERT(status == 0);

    dast_list_append(list, &v1);
    dast_list_append(list, &v2);
    status = dast_list_replace_back(list, &v3);
    TEST_ASSERT(*(dast_u32_t*)((char*)list->tail + sizeof(dast_node_t)) == v3);
    TEST_ASSERT(status == 1);

    dast_list_destroy(list);
}

void test_dast_list_remove_front()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98;
    dast_u32_t dst = 0;
    dast_u8_t status;

    status = dast_list_remove_front(list, &dst);
    TEST_ASSERT(status == 0);
    TEST_ASSERT(dst == 0);

    dast_list_append(list, &v1);
    status = dast_list_remove_front(list, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 42);
    TEST_ASSERT(list->elems == 0);
    TEST_ASSERT(list->head == 0);
    TEST_ASSERT(list->tail == 0);

    dast_list_append(list, &v1);
    dast_list_append(list, &v2);
    status = dast_list_remove_front(list, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 42);
    TEST_ASSERT(list->elems == 1);
    TEST_ASSERT(list->head == list->tail);
    TEST_ASSERT(list->tail->next == 0);
    TEST_ASSERT(list->head->prev == 0);

    dast_list_destroy(list);
}

void test_dast_list_remove_back()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_u32_t v1 = 42, v2 = 98;
    dast_u32_t dst = 0;
    dast_u8_t status;

    status = dast_list_remove_back(list, &dst);
    TEST_ASSERT(status == 0);
    TEST_ASSERT(dst == 0);

    dast_list_append(list, &v1);
    status = dast_list_remove_back(list, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 42);
    TEST_ASSERT(list->elems == 0);
    TEST_ASSERT(list->head == 0);
    TEST_ASSERT(list->tail == 0);

    dast_list_append(list, &v1);
    dast_list_append(list, &v2);
    status = dast_list_remove_back(list, &dst);
    TEST_ASSERT(status == 1);
    TEST_ASSERT(dst == 98);
    TEST_ASSERT(list->elems == 1);
    TEST_ASSERT(list->head == list->tail);
    TEST_ASSERT(list->tail->next == 0);
    TEST_ASSERT(list->head->prev == 0);

    dast_list_destroy(list);
}

void test_dast_list_insert_before()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_iterator_t* iter;
    dast_u32_t v1 = 42, v2 = 98, v3 = 102;
    dast_u32_t values[3] = {102, 98, 42};
    dast_u32_t values_repeat[4] = {102, 42, 98, 42};
    dast_u8_t status;

    iter = dast_list_iterator_new(list, 0);
    status = dast_list_insert_before(iter, &v1);
    TEST_ASSERT(status == 1);
    status = dast_list_insert_before(iter, &v2);
    TEST_ASSERT(status == 1);
    status = dast_list_insert_before(iter, &v3);
    TEST_ASSERT(status == 1);
    dast_u32_t i;
    for (i = 0; iter->elem; i++)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->next(iter);
    }
    TEST_ASSERT(i == 3);
    status = dast_list_insert_before(iter, &v3);
    TEST_ASSERT(status == 0);
    TEST_ASSERT(list->elems == 3);
    dast_list_iterator_delete(iter);

    iter = dast_list_iterator_new(list, 0);
    TEST_ASSERT(list->elems == 3);
    iter->next(iter);
    status = dast_list_insert_before(iter, &v1);
    TEST_ASSERT(status == 1);
    iter->prev(iter);
    for (i = 0; iter->elem; i++)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values_repeat[i]);
        iter->next(iter);
    }
    TEST_ASSERT(i == 4);
    TEST_ASSERT(list->elems == 4);
    dast_list_iterator_delete(iter);

    dast_list_destroy(list);
}

void test_dast_list_insert_after()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_iterator_t* iter;
    dast_u32_t v1 = 42, v2 = 98, v3 = 102;
    dast_u32_t values[3] = {42, 98, 102};
    dast_u32_t values_repeat[4] = {42, 98, 42, 102};
    dast_u8_t status;

    iter = dast_list_iterator_new(list, 0);
    status = dast_list_insert_after(iter, &v1);
    TEST_ASSERT(status == 1);
    status = dast_list_insert_after(iter, &v2);
    TEST_ASSERT(status == 1);
    status = dast_list_insert_after(iter, &v3);
    TEST_ASSERT(status == 1);
    dast_u32_t i;
    for (i = 2; iter->elem; i--)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->prev(iter);
    }
    TEST_ASSERT(i == -1);
    TEST_ASSERT(list->elems == 3);
    status = dast_list_insert_after(iter, &v3);
    TEST_ASSERT(status == 0);
    TEST_ASSERT(list->elems == 3);
    dast_list_iterator_delete(iter);

    iter = dast_list_iterator_new(list, 0);
    TEST_ASSERT(list->elems == 3);
    iter->next(iter);
    status = dast_list_insert_after(iter, &v1);
    TEST_ASSERT(status == 1);
    dast_list_iterator_reset(iter);
    for (i = 0; iter->elem; i++)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values_repeat[i]);
        iter->next(iter);
    }
    TEST_ASSERT(i == 4);
    TEST_ASSERT(list->elems == 4);
    dast_list_iterator_delete(iter);

    dast_list_destroy(list);
}

void test_dast_list_replace()
{
    dast_list_t* list = dast_list_init(allocator, sizeof(dast_u32_t), dast_cpy_generic, dast_del_dummy);
    dast_iterator_t* iter;
    dast_u32_t v1 = 42, v2 = 98, v3 = 102;
    dast_u32_t values[2] = {42, 102};
    dast_u8_t status;
    dast_u32_t i;

    iter = dast_list_iterator_new(list, 0);
    status = dast_list_replace(iter, &v1);
    TEST_ASSERT(status == 0);
    dast_list_iterator_delete(iter);

    iter = dast_list_iterator_new(list, 0);
    dast_list_insert_after(iter, &v1);
    dast_list_insert_after(iter, &v2);
    status = dast_list_replace(iter, &v3);
    TEST_ASSERT(status == 1);
    dast_list_iterator_reset(iter);
    for (i = 0; iter->elem; i++)
    {
        TEST_ASSERT(*(dast_u32_t*)iter->elem == values[i]);
        iter->next(iter);
    }
    TEST_ASSERT(i == 2);
    TEST_ASSERT(list->elems == 2);
    dast_list_iterator_delete(iter);

    dast_list_destroy(list);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_list_sizeof);
    RUN_TEST(test_dast_list_init_on);
    RUN_TEST(test_dast_list_init);
    RUN_TEST(test_dast_list_append);
    RUN_TEST(test_dast_list_size);
    RUN_TEST(test_dast_list_obj_size);
    RUN_TEST(test_dast_list_clear);
    RUN_TEST(test_dast_list_front);
    RUN_TEST(test_dast_list_back);
    RUN_TEST(test_dast_list_iterator);
    RUN_TEST(test_dast_list_iterator_back);
    RUN_TEST(test_dast_list_iterator_reversed);
    RUN_TEST(test_dast_list_iterator_reversed_next);
    RUN_TEST(test_dast_list_iterator_reset);
    RUN_TEST(test_dast_list_prepend);
    RUN_TEST(test_dast_list_replace_front);
    RUN_TEST(test_dast_list_replace_back);
    RUN_TEST(test_dast_list_remove_front);
    RUN_TEST(test_dast_list_remove_back);
    RUN_TEST(test_dast_list_insert_before);
    RUN_TEST(test_dast_list_insert_after);
    RUN_TEST(test_dast_list_replace);
    return UNITY_END();
}