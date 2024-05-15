#include "tree/tree.h"
#include "tree/tree_private.h"

#include "../dev/debug.h"

void dast_tree_add_fix_up(dast_tree_t* tree, dast_knot_t* x)
{
    dast_knot_t* uncle;
    dast_knot_t* grandfather;
    while (x->parent && !x->parent->is_black)
    {
        grandfather = x->parent->parent;
        /* case left*/
        if (x == x->parent->left)
        {
            /* case left-left */
            if (x->parent == grandfather->left)
            {
                uncle = grandfather->right;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("ll-b: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    DEBUG_PRINT("ll-r: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
            /* case left-right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("lr-b: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_right(tree, x->parent);
                    dast_tree_rotate_left(tree, grandfather);
                    grandfather->is_black = 0;
                    x->is_black = 1;
                    return;
                }
                else
                {
                    DEBUG_PRINT("lr-r: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    uncle->is_black = 1;
                    grandfather->right->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
        }
        /* case right */
        else
        {
            /* case right-left */
            if (x->parent == grandfather->left)
            {
                uncle = grandfather->right;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("rl-b: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_left(tree, x->parent);
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->is_black = 1;
                    return;
                }
                else
                {
                    DEBUG_PRINT("rl-r: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
            /* case right-right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("rr-b: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_left(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    DEBUG_PRINT("rr-r: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    uncle->is_black = 1;
                    grandfather->right->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
        }
    }

    tree->root->is_black = 1;
}