#include <unity/unity.h>

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

char* dast_tree_str(dast_tree_t* tree, unsigned int max_obj_len, void (*print)(void* key, void* buffer))
{
    if (tree->root == 0) { return 0; }
    unsigned long height = dast_tree_height(tree);
    unsigned long width = ((1 << height) - 1)*max_obj_len;

    char* buffer = DAST_MALLOC(sizeof(char) * (width + 1) * height);
    DAST_MEMSET(buffer, 0x20, sizeof(char) * (width + 1) * height);
    for (unsigned long i = 0; i < height; i++)
    {
        buffer[width + (width + 1)*i] = 0xA;
    }

    unsigned long x, y;
    x = (width / (2 * max_obj_len)) * max_obj_len;
    y = 0;

    dast_knot_t* cursor = tree->root;
    dast_knot_t* mr = tree->root;
    while (cursor->left)
    {
        cursor = cursor->left;
        x -= (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
        y += 1;
    }

    int h = 0;
    while (mr->right){
        mr = mr->right;
        h += 1;
    }
    printf("h:%d\n", h);

    while (cursor)
    {
        print(DAST_KNOT_KEY(cursor), &(buffer[y*(width + 1) + x]));
        if (cursor->right)
        {
            cursor = cursor->right;
            x += (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
            y += 1;
            
            while (cursor->left)
            {
                cursor = cursor->left;
                x -= (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
                y += 1;
            }
        }
        else if (cursor->parent)
        {
            if (cursor == cursor->parent->left)
            {
                cursor = cursor->parent;
                y -= 1;
                x += (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
            }
            else
            {
                while (cursor->parent && cursor->parent->right == cursor)
                {
                    cursor = cursor->parent;
                    y -= 1;
                    x -= (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
                }
                cursor = cursor->parent;
                y -= 1;
                x += (((1 << (height - y - 1)) - 1) / 2 + 1)*max_obj_len;
            }
        }
        else
        {
            cursor = 0;
        }
        
    }

    return buffer;
}

void dast_tree_str_free_buffer(char* buffer) { DAST_FREE(buffer); };

void setUp(void) {}

void tearDown(void) {}

void copy_int_and_inc(void* src, void* dst) { *(int*)dst = (*(int*)src) + 1; }
void int_pointer_inc(void* obj) { (**((int**)obj))++; }
int cmp_int(void* l, void* r) { return *(int*)l - *(int*)r; }

void test_dast_tree_init_and_destroy()
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
    return UNITY_END();
}