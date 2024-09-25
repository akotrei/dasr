#include <unity/unity.h>

#include "list.h"

void setUp(void) {}

void tearDown(void) {}

void copy_int_and_inc(void* src, void* dst) { *(int*)dst = (*(int*)src) + 1; }
void int_pointer_inc(void* obj) { (**((int**)obj))++; }

void test_dast_list_init_and_destroy()
{
    dast_list_t list;
    dast_list_init(&list, 10);
    TEST_ASSERT(list.elem_size == 10);
    TEST_ASSERT(list.elems == 0);

    dast_list_destroy(&list);
}

void test_dast_list_copy_append()
{
    dast_list_t list;
    dast_list_t new_list;

    dast_list_init(&list, sizeof(int));

    for (int i = 42; i < 45; i++)
    {
        dast_list_append(&list, DAST_LIST_LAST(&list), &i);
    }

    TEST_ASSERT(list.elems == 3);
    int i = 0;
    dast_node_t* curr = DAST_LIST_FIRST(&list);
    while (curr)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(curr) == i + 42);
        curr = DAST_LIST_NEXT(curr);
        i++;
    }
    TEST_ASSERT(i == 3);

    dast_list_copy(&list, &new_list);
    TEST_ASSERT(new_list.elems == 3);
    i = 0;
    curr = DAST_LIST_FIRST(&new_list);
    while (curr)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(curr) == i + 42);
        curr = DAST_LIST_NEXT(curr);
        i++;
    }
    TEST_ASSERT(i == 3);

    dast_list_destroy(&list);
    dast_list_destroy(&new_list);
}

void test_test_dast_list_deepcopy()
{
    dast_list_t list;
    dast_list_t new_list;

    dast_list_init(&list, sizeof(int));

    for (int i = 0; i < 3; i++)
    {
        dast_list_append(&list, DAST_LIST_LAST(&list), &i);
    }

    dast_list_deepcopy(&list, &new_list, copy_int_and_inc);
    
    int i = 0;
    for (dast_node_t* c = DAST_LIST_FIRST(&new_list); c; c = DAST_LIST_NEXT(c), i++)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(c) == i + 1);
    }
    TEST_ASSERT(i == 3);

    dast_list_destroy(&new_list);
    dast_list_destroy(&list);
}

void test_dast_list_reverse_append()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    dast_list_reverse(&list);
    TEST_ASSERT(list.head == 0);
    TEST_ASSERT(list.tail == 0);
    TEST_ASSERT(list.elems == 0);

    for (int i = 0; i < 3; i++)
    {
        dast_list_append(&list, DAST_LIST_LAST(&list), &i);
    }

    dast_list_reverse(&list);
    int i = 2;
    for (dast_node_t* c = DAST_LIST_FIRST(&list); c; c = DAST_LIST_NEXT(c), i--)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(c) == i);
    }
    TEST_ASSERT(i == -1);
    TEST_ASSERT(list.elems == 3);

    dast_list_destroy(&list);
}

void test_dast_list_clear()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    dast_list_clear(&list);
    TEST_ASSERT(list.elems == 0);

    int i = 1;
    dast_list_append(&list, DAST_LIST_LAST(&list), &i);
    dast_list_clear(&list);
    TEST_ASSERT(list.elems == 0);

    dast_list_destroy(&list);
}

void test_dast_list_deepclear()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int*));

    dast_list_clear(&list);
    TEST_ASSERT(list.elems == 0);

    int i = 1;
    int* pi = &i;
    dast_list_append(&list, DAST_LIST_LAST(&list), &pi);
    dast_list_deepclear(&list, int_pointer_inc);
    TEST_ASSERT(list.elems == 0);
    TEST_ASSERT(i == 2);

    dast_list_destroy(&list);
}

void test_dast_list_first_last()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    TEST_ASSERT(DAST_LIST_FIRST(&list) == 0);
    TEST_ASSERT(DAST_LIST_LAST(&list) == 0);

    int obj = 12;
    dast_list_append(&list, DAST_LIST_LAST(&list), &obj);
    TEST_ASSERT(*(int*)DAST_NODE_ELEM(DAST_LIST_FIRST(&list)) == obj);
    TEST_ASSERT(*(int*)DAST_NODE_ELEM(DAST_LIST_LAST(&list)) == obj);

    int obj2 = 75;
    dast_list_append(&list, DAST_LIST_LAST(&list), &obj2);
    TEST_ASSERT(*(int*)DAST_NODE_ELEM(DAST_LIST_FIRST(&list)) == obj);
    TEST_ASSERT(*(int*)DAST_NODE_ELEM(DAST_LIST_LAST(&list)) == obj2);
    TEST_ASSERT(list.elems == 2);

    dast_list_destroy(&list);
}

void test_dast_list_next_prev()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    int i = 0;
    for (dast_node_t* node = DAST_LIST_FIRST(&list); node; node = DAST_LIST_NEXT(node))
    {
        i++;
    }
    TEST_ASSERT(i == 0);

    for (dast_node_t* node = DAST_LIST_FIRST(&list); node; node = DAST_LIST_PREV(node))
    {
        i++;
    }
    TEST_ASSERT(i == 0);

    int elems[] = {1, 2, 3};
    for (int i = 0; i < 3; i++)
    {
        dast_list_append(&list, DAST_LIST_LAST(&list), &elems[i]);
    }

    TEST_ASSERT(list.elems == 3);
    i = 0;
    for (dast_node_t* node = DAST_LIST_FIRST(&list); node; node = DAST_LIST_NEXT(node))
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(node) == elems[i]);
        i++;
    }
    i = 2;
    for (dast_node_t* node = DAST_LIST_LAST(&list); node; node = DAST_LIST_PREV(node))
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(node) == elems[i]);
        i--;
    }

    dast_list_destroy(&list);
}

void test_dast_list_prepend()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    for (int i = 0; i < 3; i++)
    {
        dast_list_prepend(&list, DAST_LIST_FIRST(&list), &i);
    }

    TEST_ASSERT(list.elems == 3);
    int i = 2;
    for (dast_node_t* c = DAST_LIST_FIRST(&list); c; c = DAST_LIST_NEXT(c), i--)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(c) == i);
    }
    TEST_ASSERT(i == -1);

    dast_list_destroy(&list);
}

void test_dast_list_replace()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    for (int i = 0; i < 3; i++)
    {
        dast_list_prepend(&list, DAST_LIST_FIRST(&list), &i);
    }
    TEST_ASSERT(list.elems == 3);

    int r = 10;
    for (dast_node_t* c = DAST_LIST_FIRST(&list); c; c = DAST_LIST_NEXT(c), r++)
    {
        DAST_LIST_REPLACE(&list, c, &r);
    }
    TEST_ASSERT(list.elems == 3);
    
    r = 10;
    for (dast_node_t* c = DAST_LIST_FIRST(&list); c; c = DAST_LIST_NEXT(c), r++)
    {
        TEST_ASSERT(*(int*)DAST_NODE_ELEM(c) == r);
    }

    dast_list_destroy(&list);
}

void test_dast_list_remove()
{
    dast_list_t list;
    dast_list_init(&list, sizeof(int));

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    dast_list_append(&list, DAST_LIST_LAST(&list), &a);
    TEST_ASSERT(list.elems == 1);

    dast_list_remove(&list, DAST_LIST_LAST(&list));
    TEST_ASSERT(list.elems == 0);

    dast_list_append(&list, DAST_LIST_LAST(&list), &a);
    dast_list_append(&list, DAST_LIST_LAST(&list), &b);
    TEST_ASSERT(list.elems == 2);

    dast_list_remove(&list, DAST_LIST_FIRST(&list));
    TEST_ASSERT(list.elems == 1);
    TEST_ASSERT(*(int*)DAST_NODE_ELEM(DAST_LIST_FIRST(&list)) == b);

    dast_list_append(&list, DAST_LIST_LAST(&list), &c);
    dast_list_append(&list, DAST_LIST_LAST(&list), &d);

    for (dast_node_t* c = DAST_LIST_FIRST(&list); c;)
    {
        dast_node_t* to_delete = c;
        c = DAST_LIST_NEXT(c);
        dast_list_remove(&list, to_delete);
    }
    TEST_ASSERT(list.elems == 0);

    dast_list_destroy(&list);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_list_init_and_destroy);
    RUN_TEST(test_dast_list_copy_append);
    RUN_TEST(test_test_dast_list_deepcopy);
    RUN_TEST(test_dast_list_reverse_append);
    RUN_TEST(test_dast_list_clear);
    RUN_TEST(test_dast_list_deepclear);
    RUN_TEST(test_dast_list_first_last);
    RUN_TEST(test_dast_list_next_prev);
    RUN_TEST(test_dast_list_prepend);
    RUN_TEST(test_dast_list_replace);
    RUN_TEST(test_dast_list_remove);
    return UNITY_END();
}