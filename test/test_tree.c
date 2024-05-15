#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "ntype.h"
#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/allocator_std.h"
#include "utils/mem.h"

int cmp(void* o1, void* o2) { return *(int*)o1 - *(int*)o2; }

int height(dast_knot_t* root)
{
    if (root == NULL)
        return 0;
    return fmax(height(root->left), height(root->right)) + 1;
}

int getcol(int h)
{
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}

int isBalancedUtil(dast_knot_t* root, int* maxh, int* minh)
{
    // DEBUG_PRINT("maxh minh: %d %d\n", *maxh, *minh);
    // Base case
    if (!root)
    {
        *maxh = 0;
        *minh = 0;
        return 1;
    }

    int lmxh, lmnh; // To store max and min heights of left subtree
    int rmxh, rmnh; // To store max and min heights of right subtree

    // Check if left subtree is balanced, also set lmxh and lmnh
    if (!isBalancedUtil(root->left, &lmxh, &lmnh))
        return 0;

    // Check if right subtree is balanced, also set rmxh and rmnh
    if (!isBalancedUtil(root->right, &rmxh, &rmnh))
        return 0;

    // Set the max and min heights of this node for the parent call
    *maxh = fmax(lmxh, rmxh) + 1;
    *minh = fmin(lmnh, rmnh) + 1;

    // See if this node is balanced
    if ((*maxh) <= 2 * (*minh))
        return 1;

    return 0;
}

// A wrapper over isBalancedUtil()
int isBalanced(dast_tree_t* tree)
{
    int maxh, minh;
    return isBalancedUtil(tree->root, &maxh, &minh);
}

void printTree(int** M, int** C, dast_knot_t* root, int col, int row, int height)
{
    if (root == NULL)
        return;
    M[row][col] = *(int*)((char*)root + sizeof(dast_knot_t));
    C[row][col] = root->is_black;
    printTree(M, C, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, C, root->right, col + pow(2, height - 2), row + 1, height - 1);
}

void TreePrinter(dast_tree_t* tree)
{
    int   h = height(tree->root);
    int   col = getcol(h);
    int** M = (int**)malloc(sizeof(int*) * h);
    int** C = (int**)malloc(sizeof(int*) * h);
    for (int i = 0; i < h; i++)
    {
        M[i] = (int*)malloc(sizeof(int) * col);
        C[i] = (int*)malloc(sizeof(int) * col);
    }
    printTree(M, C, tree->root, col / 2, 0, h);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (M[i][j] == 0)
                printf("  ");
            else
                printf("%d %d", M[i][j], C[i][j]);
        }
        printf("\n");
    }
}

void print_tree(dast_knot_t* knot)
{
    if (knot)
    {
        print_tree(knot->left);
        printf("v: %d\n", *(int*)((char*)knot + sizeof(dast_knot_t)));
        print_tree(knot->right);
    }
}

void test_add_fix_up_left_left_nil()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int a5 = 5, a4 = 4, a3 = 3;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_left_red()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_left_nil_deeper()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_left_red_deeper()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am1 = -1, a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_left_nil_deeper2()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_right_nil()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_left_right_red()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33, a34 = 34;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);
    dast_tree_add(tree, &a34);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_right_left_black()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33, a34 = 34;
    int am25 = -25;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);
    dast_tree_add(tree, &a34);
    dast_tree_add(tree, &am25);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_right_left_red()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33, a34 = 34;
    int am25 = -25, am29 = -29;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);
    dast_tree_add(tree, &a34);
    dast_tree_add(tree, &am25);
    dast_tree_add(tree, &am29);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_right_right_black()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33, a34 = 34;
    int am25 = -25, am27 = -27, am28 = -28;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);
    dast_tree_add(tree, &a34);
    dast_tree_add(tree, &am25);
    dast_tree_add(tree, &am27);
    dast_tree_add(tree, &am28);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_right_right_red()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int am3 = -30, am2 = -20, am1 = -10, a1 = 10, a2 = 20, a3 = 30, a4 = 40, a5 = 50;
    int a35 = 35, a33 = 33, a34 = 34;
    int am25 = -25, am27 = -27, am28 = -28, am26 = -26;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &am1);
    dast_tree_add(tree, &am2);
    dast_tree_add(tree, &am3);
    dast_tree_add(tree, &a35);
    dast_tree_add(tree, &a33);
    dast_tree_add(tree, &a34);
    dast_tree_add(tree, &am25);
    dast_tree_add(tree, &am27);
    dast_tree_add(tree, &am28);
    dast_tree_add(tree, &am26);

    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a5);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    dast_tree_destroy_from(tree);
}

void test_add_fix_up_all_cases()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // dast_tree_add
    int h0 = 0, h1 = 1000, h2 = 2000, h3 = 3000, h4 = 4000;
    int d0 = 3500, d1 = 3400, d2 = 3300, d3 = 3200, d4 = 3100;
    int a0 = 3110, a1 = 3120, a2 = 3130, a3 = 3140, a4 = 3150;
    int c0 = 3141, c1 = 3142, c2 = 3143, c3 = 3144, c4 = 3145;

    dast_tree_add(tree, &h0);
    dast_tree_add(tree, &h1);
    dast_tree_add(tree, &h2);
    dast_tree_add(tree, &h3);
    dast_tree_add(tree, &h4);
    dast_tree_add(tree, &d0);
    dast_tree_add(tree, &d1);
    dast_tree_add(tree, &d2);
    dast_tree_add(tree, &d3);
    dast_tree_add(tree, &d4);
    dast_tree_add(tree, &a0);
    dast_tree_add(tree, &a1);
    dast_tree_add(tree, &a2);
    dast_tree_add(tree, &a3);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &c0);
    dast_tree_add(tree, &c1);
    dast_tree_add(tree, &c2);
    dast_tree_add(tree, &c3);
    dast_tree_add(tree, &c4);

    // dast_tree_clear(tree);
    // printf("tree->root: %p\n", tree->root);

    // int v = 0;
    // for (; v < 10000000; v++)
    // {
    //     dast_tree_add(tree, &v);
    // }

    // print_tree(tree->root);
    int is_balanced = isBalanced(tree);
    printf("size: %ld, height: %ld\n", dast_tree_size(tree), dast_tree_height(tree));
    DEBUG_PRINT("is_balanced: %d, height: %d\n", is_balanced, height(tree->root));
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree), height(tree->root));

    int   to_search = 1234567;
    void* search_res;
    search_res = dast_tree_search(tree, &to_search);
    if (search_res)
    {
        printf("search_res: %d\n", *(int*)(search_res));
    }
    else
    {
        printf("search_res: NOT FOUND\n");
    }

    dast_tree_clear(tree);

    int g = 0;
    for (; g < 10; g++)
    {
        dast_tree_add(tree, &g);
    }

    dast_iterator_t* tree_iter;
    tree_iter = dast_tree_iterator_new(tree, 0);
    void* obj;

    obj = tree_iter->next(tree_iter);
    if (obj)
    {
        printf("vn: %d, ", *(int*)(obj));
    }
    obj = tree_iter->next(tree_iter);
    if (obj)
    {
        printf("vn: %d, ", *(int*)(obj));
    }

    obj = tree_iter->prev(tree_iter);
    if (obj)
    {
        printf("vp: %d, ", *(int*)(obj));
    }
    obj = tree_iter->prev(tree_iter);
    if (obj)
    {
        printf("vp: %d, ", *(int*)(obj));
    }
    obj = tree_iter->prev(tree_iter);
    if (obj)
    {
        printf("vp: %d, ", *(int*)(obj));
    }
    obj = tree_iter->prev(tree_iter);
    if (obj)
    {
        printf("vpe: %d, ", *(int*)(obj));
    }

    obj = tree_iter->next(tree_iter);
    if (obj)
    {
        printf("vn: %d, ", *(int*)(obj));
    }

    tree_iter->reset(tree_iter);
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    while (1)
    {
        void* obj;
        obj = tree_iter->next(tree_iter);
        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
        // obj = tree_iter->next(tree_iter);
        // if (!obj)
        // {
        //     break;
        // }
        // printf("v: %d, ", *(int*)(obj));
        obj = tree_iter->prev(tree_iter);
        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
    }
    printf("===\n");
    // tree_iter->reset(tree_iter);
    while (1)
    {
        void* obj = tree_iter->prev(tree_iter);

        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
    }
    dast_tree_iterator_delete(tree_iter);

    printf("\n");
    printf("=== === ===\n");

    tree_iter = dast_tree_iterator_new(tree, 1);
    while (1)
    {
        void* obj = tree_iter->next(tree_iter);

        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
    }
    printf("!!!\n");
    tree_iter->reset(tree_iter);
    while (1)
    {
        void* obj = tree_iter->next(tree_iter);

        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
    }
    dast_tree_iterator_delete(tree_iter);

    printf("MIN: %d\n", *(int*)dast_tree_min(tree));
    printf("MAX: %d\n", *(int*)dast_tree_max(tree));
    printf("dast_knot_t_size: %d\n", sizeof(dast_knot_t));

    dast_tree_destroy_from(tree);
}

void test_delete_case_1()
{
    unsigned long     alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char          mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init_on(mem_tree, allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    printf("++++++++++++++++++++++++++++++++\n");
    int i = 0;
    for (; i < 10; i++)
    {
        dast_tree_add(tree, &i);
        // TreePrinter(tree);
        // printf("===\n");
    }

    dast_iterator_t* tree_iter = dast_tree_iterator_new(tree, 0);
    while (1)
    {
        void* obj = tree_iter->next(tree_iter);

        if (!obj)
        {
            break;
        }
        printf("v: %d, ", *(int*)(obj));
    }
    printf("END\n");
    dast_tree_iterator_delete(tree_iter);
    TreePrinter(tree);

    i = 6;
    int status = dast_tree_remove(tree, &i);
    printf("status: %d\n", status);
    TreePrinter(tree);

    dast_tree_destroy_from(tree);
}

void test_iter_empty()
{
    dast_u64_t        alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    dast_tree_t* tree = dast_tree_init(allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    // printf("tree->min %p\n", tree->min.left);
    printf("tree->min %p\n", dast_tree_min(tree));
    printf("tree->max %p\n", dast_tree_max(tree));

    dast_iterator_t* iter = dast_tree_iterator_new(tree, 0);
    printf("iter\n");
    printf("iter->next %p %p\n", iter->next(iter), iter->next(iter));
    printf("iter->prev %p %p\n", iter->prev(iter), iter->prev(iter));
}

void test_iter_one_elem()
{
    printf("=======================\n");
    dast_u64_t        alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    dast_tree_t* tree = dast_tree_init(allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    int val = 10;
    dast_tree_add(tree, &val);

    printf("tree->min %p %d\n", dast_tree_min(tree), *(int*)dast_tree_min(tree));
    printf("tree->max %p %d\n", dast_tree_max(tree), *(int*)dast_tree_max(tree));

    dast_iterator_t* iter = dast_tree_iterator_new(tree, 0);
    printf("iter\n");
    printf("iter->next %p ---", iter->next(iter));
    printf("iter->next %p\n", iter->next(iter));
    printf("iter->prev %p ---", iter->prev(iter));
    printf("iter->prev %p\n", iter->prev(iter));
}

void test_iter_many_elem()
{
    printf("=======================\n");
    dast_u64_t        alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    dast_tree_t* tree = dast_tree_init(allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    int val = 0;
    while (val < 10)
    {
        dast_tree_add(tree, &val);
        val++;
    }

    printf("tree->min %p %d\n", dast_tree_min(tree), *(int*)dast_tree_min(tree));
    printf("tree->max %p %d\n", dast_tree_max(tree), *(int*)dast_tree_max(tree));

    dast_iterator_t* iter = dast_tree_iterator_new(tree, 0);
    void*            o;
    while ((o = iter->next(iter)))
    {
        printf("v: %d\n", *(int*)o);
    }
    printf("iter\n");
    printf("iter->next %p %p\n", iter->next(iter), iter->next(iter));
    printf("iter->prev %p %p\n", iter->prev(iter), iter->prev(iter));
}

void test_iter_many_elem_next_and_prev()
{
    printf("=======================\n");
    dast_u64_t        alloc_size = dast_allocator_std_sizeof();
    char              mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_std_init(mem_allocator);

    dast_tree_t* tree = dast_tree_init(allocator, sizeof(int), cmp, dast_cpy_generic, dast_del_dummy);

    int val = 0;
    while (val < 12)
    {
        dast_tree_add(tree, &val);
        val++;
    }

    printf("tree->min %d\n", *(int*)dast_tree_min(tree));
    printf("tree->max %d\n", *(int*)dast_tree_max(tree));

    dast_iterator_t* iter = dast_tree_iterator_new(tree, 0);
    void*            o;
    while (1)
    {
        o = iter->next(iter);
        o = iter->next(iter);
        o = iter->prev(iter);

        if (!o)
        {
            break;
        }
        printf("v: %d\n", *(int*)o);
    }
    printf("---\n");
}

int main(int argc, char** arcv)
{
    // test_add_fix_up_left_left_nil();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_left_red();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_left_nil_deeper();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_left_red_deeper();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_left_nil_deeper2();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_right_nil();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_left_right_red();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_right_left_black();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_right_left_red();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_right_right_black();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_right_right_red();
    // DEBUG_PRINT("==========\n");
    // test_add_fix_up_all_cases();
    test_iter_empty();
    test_iter_one_elem();
    test_iter_many_elem();
    test_iter_many_elem_next_and_prev();
    // test_delete_case_1();
    DEBUG_PRINT("==========\n");

    return 0;
}