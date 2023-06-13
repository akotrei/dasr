#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree/tree.h"
#include "tree/tree_private.h"
#include "utils/allocator.h"
#include "debug.h"

int cmp(void* o1, void* o2)
{
    return *(int*)o1 - *(int*)o2;
}

void cpy(void* o, void* dst)
{
    /* DEBUG_PRINT("cpy: %d\n", *(int*)o); */
    *(int*)dst = *(int*)o;
}

void del(void* o)
{
    /* DEBUG_PRINT("o: %d, c: %d\n", *(int*)o, ((dast_knot_t*)((char*)o - sizeof(dast_knot_t)))->is_black); */
}

int height(dast_knot_t *root) {
    if (root == NULL)
        return 0;
    return fmax(height(root->left), height(root->right)) + 1;
}
 
int getcol(int h) {
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}

int isBalancedUtil(dast_knot_t *root, int *maxh, int *minh)
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
    if ((*maxh) <= 2*(*minh))
        return 1;
 
    return 0;
}
 
// A wrapper over isBalancedUtil()
int isBalanced(dast_tree_t *tree)
{
    int maxh, minh;
    return isBalancedUtil(tree->root, &maxh, &minh);
}
 
void printTree(int **M, int **C, dast_knot_t *root, int col, int row, int height) {
    if (root == NULL)
        return;
    M[row][col] = *(int*)((char*)root + sizeof(dast_knot_t));
    C[row][col] = root->is_black;
    printTree(M, C, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, C, root->right, col + pow(2, height - 2), row + 1, height - 1);
}
 
void TreePrinter(dast_tree_t* tree) {
    int h = height(tree->root);
    int col = getcol(h);
    int **M = (int**)malloc(sizeof(int*) * h);
    int **C = (int**)malloc(sizeof(int*) * h);
    for (int i = 0; i < h; i++) {
        M[i] = (int*)malloc(sizeof(int) * col);
        C[i] = (int*)malloc(sizeof(int) * col);
    }
    printTree(M, C, tree->root, col / 2, 0, h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < col; j++) {
            if (M[i][j] == 0)
                printf("  ");
            else
                printf("%d %d", M[i][j], C[i][j]);
        }
        printf("\n");
    }
}

void test_add_fix_up_left_left_nil()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

    // dast_tree_add
    int a5 = 5, a4 = 4, a3 = 3;
    dast_tree_add(tree, &a5);
    dast_tree_add(tree, &a4);
    dast_tree_add(tree, &a3);

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_red()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_nil_deeper()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_red_deeper()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_left_nil_deeper2()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("%d\n", is_balanced);
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_right_nil()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_left_right_red()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_right_left_black()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_right_left_red()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_right_right_black()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_right_right_red()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

    dast_tree_free(tree);
}

void test_add_fix_up_all_cases()
{
    unsigned long alloc_size = dast_allocator_sizeof();
    char mem_allocator[alloc_size];
    dast_allocator_t* allocator = dast_allocator_init(mem_allocator);

    unsigned long tree_size = dast_tree_sizeof();
    char mem_tree[tree_size];

    dast_tree_t* tree;
    tree = dast_tree_init(mem_tree, allocator, sizeof(int), cmp, cpy, del);

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

    TreePrinter(tree);
    int is_balanced = isBalanced(tree);
    DEBUG_PRINT("is_balanced: %d, height: %d\n", is_balanced, height(tree->root));
    DEBUG_PRINT("HEIGHT: %lu %d\n", dast_tree_height(tree) ,height(tree->root));

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
    test_add_fix_up_left_right_nil();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_left_right_red();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_right_left_black();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_right_left_red();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_right_right_black();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_right_right_red();
    DEBUG_PRINT("==========\n");
    test_add_fix_up_all_cases();
    DEBUG_PRINT("==========\n");

    return 0;
}