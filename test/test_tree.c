#include <unity/unity.h>
#include <string.h>

#include "tree.h"

void print_4digits_from_int(void* key, void* buffer)
{
    int k = *(int*)key;
    int n = 3;
    unsigned char* char_buff = (unsigned char*)buffer + sizeof(unsigned char) * n;
    for (; n > -1; n--, k /= 10)
    {
        *char_buff-- = (unsigned char)(k % 10) + 48;
    }
}

void dast_tree_traverse(
    dast_knot_t* knot,
    void** buffer,
    void (*print)(void* key, void* buffer),
    unsigned int printed_size
)
{
    if (!knot) { return; }

    **(unsigned char**)buffer = 40;
    *((unsigned char**)buffer) += 1;

    print(DAST_KNOT_KEY(knot), *buffer);
    *(unsigned char**)buffer += printed_size;

    dast_tree_traverse(knot->left, buffer, print, printed_size);
    dast_tree_traverse(knot->right, buffer, print, printed_size);

    **(unsigned char**)buffer = 41;
    *((unsigned char**)buffer) += 1;
}

char* dast_tree_serialize(dast_tree_t* tree, unsigned int printed_size, void (*print)(void* key, void* buffer))
{
    unsigned long elems = tree->elems;
    unsigned long buffer_size = (2 + printed_size) * elems + 1;
    char* buffer = DAST_MALLOC(buffer_size*sizeof(char));
    buffer[buffer_size-1] = '\0';

    char* tmp = buffer;
    dast_tree_traverse(tree->root, (void**)&tmp, print, printed_size);

    return buffer;
}

void dast_tree_serialize_free_buffer(char* buffer) { DAST_FREE(buffer); };

void setUp(void) {}

void tearDown(void) {}

void copy_int_and_inc(void* src, void* dst) { *(int*)dst = (*(int*)src) + 1; }
void int_pointer_inc(void* obj) { (**((int**)obj))++; }
int cmp_int(void* l, void* r) { return *(int*)l - *(int*)r; }

void test_dast_tree_init_and_destroy_and_add()
{
    dast_tree_t tree;
    dast_tree_init(&tree, sizeof(int), cmp_int);

    int elems = 1024;
    for (int i = 0; i < elems; i++)
    {
        dast_tree_add(&tree, &i);
    }
    TEST_ASSERT(tree.elems == elems);
    TEST_ASSERT(dast_tree_height(&tree) == 18);

    dast_tree_destroy(&tree);
}

void test_add_fix_up_left_left_nil()
{
    dast_tree_t tree;
    dast_tree_init(&tree, sizeof(int), cmp_int);

    int a5 = 5, a4 = 4, a3 = 3;
    dast_tree_add(&tree, &a5);
    dast_tree_add(&tree, &a4);
    dast_tree_add(&tree, &a3);

    TEST_ASSERT(tree.elems == 3);

    char* tree_repr = dast_tree_serialize(&tree, 4, print_4digits_from_int);
    TEST_ASSERT(strcmp(tree_repr, "(0004(0003)(0005))") == 0);
    dast_tree_serialize_free_buffer(tree_repr);

    dast_tree_destroy(&tree);
}

void test_add_fix_up_left_left_red()
{
    dast_tree_t tree;
    dast_tree_init(&tree, sizeof(int), cmp_int);

    int a5 = 5, a4 = 4, a3 = 3, a2 = 2;
    dast_tree_add(&tree, &a5);
    dast_tree_add(&tree, &a4);
    dast_tree_add(&tree, &a3);
    dast_tree_add(&tree, &a2);

    TEST_ASSERT(tree.elems == 4);

    char* tree_repr = dast_tree_serialize(&tree, 4, print_4digits_from_int);
    TEST_ASSERT(strcmp(tree_repr, "(0004(0003(0002))(0005))") == 0);
    dast_tree_serialize_free_buffer(tree_repr);

    dast_tree_destroy(&tree);
}

void test_add_fix_up_left_left_nill_and_parent()
{
    dast_tree_t tree;
    dast_tree_init(&tree, sizeof(int), cmp_int);

    int a5 = 5, a4 = 4, a3 = 3, a2 = 2, a1 = 1;
    dast_tree_add(&tree, &a5);
    dast_tree_add(&tree, &a4);
    dast_tree_add(&tree, &a3);
    dast_tree_add(&tree, &a2);
    dast_tree_add(&tree, &a1);

    TEST_ASSERT(tree.elems == 5);

    char* tree_repr = dast_tree_serialize(&tree, 4, print_4digits_from_int);
    TEST_ASSERT(strcmp(tree_repr, "(0004(0002(0001)(0003))(0005))") == 0);
    dast_tree_serialize_free_buffer(tree_repr);

    dast_tree_destroy(&tree);
}

void test_add_fix_up_left_left_nil_and_parent_case2()
{
    dast_tree_t tree;
    dast_tree_init(&tree, sizeof(int), cmp_int);

    for (int i = 8; i > 0; i--)
    {
        dast_tree_add(&tree, &i);
    }

    TEST_ASSERT(tree.elems == 8);

    char* tree_repr = dast_tree_serialize(&tree, 4, print_4digits_from_int);
    printf("tree_repr: %s\n", tree_repr);
    // TEST_ASSERT(strcmp(tree_repr, "(0004(0002(0001)(0003))(0005))") == 0);
    dast_tree_serialize_free_buffer(tree_repr);

    dast_tree_destroy(&tree);
}

void test_dast_tree_copy()
{

}

void test_dast_tree_deepcopy()
{
    
}

void test_dast_tree_clear()
{

}

void test_dast_tree_deepclear()
{

}

void test_dast_tree_height()
{

}

void test_dast_tree_add()
{

}

void test_dast_tree_get()
{

}

void test_dast_tree_remove()
{

}

void test_dast_tree_min_max()
{

}

void test_dast_tree_next()
{

}

void test_dast_tree_prev()
{

}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_dast_tree_init_and_destroy_and_add);
    RUN_TEST(test_add_fix_up_left_left_nil);
    RUN_TEST(test_add_fix_up_left_left_red);
    RUN_TEST(test_add_fix_up_left_left_nill_and_parent);
    RUN_TEST(test_add_fix_up_left_left_nil_and_parent_case2);
    return UNITY_END();
}