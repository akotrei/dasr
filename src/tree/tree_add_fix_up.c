#include "tree/tree.h"
#include "tree/tree_private.h"

#include "../dev/debug.h"

void dast_tree_add_fix_up(dast_tree_t* tree, dast_knot_t* x)
{
    dast_knot_t* uncle;
    dast_knot_t* grandfather;
    while (x->parent && !x->parent->is_black)
    {
        DEBUG_PRINT("o: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
        grandfather = x->parent->parent;
        /* case left*/
        if (x == x->parent->left)
        {
            /* case left*/
            if (x->parent == grandfather->left)
            {
                uncle = grandfather->right;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("o: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
            /* case right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("o: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_left(tree, x->parent);
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
        }
        /* case right */
        else
        {
            /* case left*/
            if (x->parent == grandfather->left)
            {
                uncle = grandfather->right;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("o: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
            /* case right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    DEBUG_PRINT("o: %d\n", *(int*)((char*)x + sizeof(dast_knot_t)));
                    dast_tree_rotate_left(tree, x->parent);
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
                    uncle->is_black = 1;
                    grandfather->left->is_black = 1;
                    grandfather->is_black = 0;
                    x = grandfather;
                }
            }
        }
    }

    tree->root->is_black = 1;
}