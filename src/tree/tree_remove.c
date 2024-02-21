#include "tree/tree.h"
#include "tree/tree_private.h"

void dast_tree_transplant(dast_tree_t* tree, dast_knot_t* x, dast_knot_t* y)
{
    if (!(x->parent))
    {
        tree->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    if (y)
    {
        y->parent = x->parent;
    }
}

void dast_tree_remove_fix_up(dast_tree_t* tree, dast_knot_t* x)
{
    dast_knot_t* w;
    while (x && x->parent && x->is_black)
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (!(w->is_black))
            {
                dast_tree_rotate_left(tree, w);
                w->is_black = 1;
                w->left->is_black = 0;
                w = x->parent->right;
            }
            if (w->left->is_black && w->right->is_black)
            {
                w->is_black = 0;
                x = x->parent;
            }
            else
            {
                if (w->right->is_black)
                {
                    dast_tree_rotate_right(tree, w->left);
                    w->is_black = 0;
                    w->parent->is_black = 1;
                    w = x->parent->right;
                }
                dast_tree_rotate_right(tree, w);
                w->is_black = w->left->is_black;
                w->left->is_black = 1;
                w->right->is_black = 1;
                x = tree->root;
            }
        }
        else
        {
            w = x->parent->left;
            if (!(w->is_black))
            {
                dast_tree_rotate_right(tree, w);
                w->is_black = 1;
                w->right->is_black = 0;
                w = x->parent->left;
            }
            if (w->right->is_black && w->left->is_black)
            {
                w->is_black = 0;
                x = x->parent;
            }
            else
            {
                if (w->left->is_black)
                {
                    dast_tree_rotate_left(tree, w->right);
                    w->is_black = 0;
                    w->parent->is_black = 1;
                    w = x->parent->left;
                }
                dast_tree_rotate_left(tree, w);
                w->is_black = w->right->is_black;
                w->right->is_black = 1;
                w->left->is_black = 1;
                x = tree->root;
            }
        }
    }
    if (x)
    {
        x->is_black = 1;
    }
}

dast_u8_t dast_tree_remove(dast_tree_t* tree, void* obj)
{
    dast_knot_t* z;
    dast_knot_t* x;
    dast_knot_t* y;
    dast_u8_t    y_is_black;
    void*        obj_to_delete;

    obj_to_delete = dast_tree_search(tree, obj);
    if (!obj_to_delete)
    {
        return 0;
    }

    y = z = (dast_knot_t*)((char*)(obj_to_delete) - sizeof(dast_knot_t));
    y_is_black = y->is_black;

    if (!(z->left))
    {
        x = z->right;
        dast_tree_transplant(tree, z, z->right);
    }
    else if (!(z->right))
    {
        x = z->left;
        dast_tree_transplant(tree, z, z->left);
    }
    else
    {
        y = dast_knot_min(z->right);
        y_is_black = y->is_black;
        x = y->right;
        if (y != z->right)
        {
            dast_tree_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        dast_tree_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->is_black = z->is_black;
    }
    tree->del(obj_to_delete);
    tree->allocator->deallocate(tree->allocator, z);

    if (y_is_black)
    {
        dast_tree_remove_fix_up(tree, x);
    }

    tree->size--;

    return 1;
}