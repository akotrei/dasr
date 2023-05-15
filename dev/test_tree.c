#include <stdio.h>

#include "tree/tree.h"
#include "utils/allocator.h"
#include "debug.h"

int cmp(void* o1, void* o2)
{
    return *(int*)o1 - *(int*)o2;
}

void cpy(void* o, void* dst)
{
    DEBUG_PRINT("cpy: %d\n", *(int*)o);
    *(int*)dst = *(int*)o;
}

void del(void* o)
{
    DEBUG_PRINT("o: %d\n", *(int*)o);
}


void test_add_fix_up_left_left_nil()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int a5 = 5, a4 = 4, a3 = 3;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_red()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_nil_deeper()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_red_deeper()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int am1 = -1, a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_nil_deeper2()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_iallocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int am3 = -3, am2 = -2, am1 = -1, a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);

    dast_tree_free(tree);
}

int main(int argc, char** arcv)
{
    test_add_fix_up_left_left_nil();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_left_left_red();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_left_left_nil_deeper();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_left_left_red_deeper();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_left_left_nil_deeper2();
    DEBUG_PRINT("==========\n");

    return 0;
}