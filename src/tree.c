#include "tree.h"

static dast_knot_t* dast_knot_min(dast_knot_t* knot);

static dast_knot_t* dast_knot_max(dast_knot_t* knot);

// Get @knot height
static unsigned long dast_knot_height(dast_knot_t* knot);

// Rotates to the left @tree around @x:
//
//          [p]                    [p]      *
//           |                      |       *
//          [y]                    [x]      *
//         /   \       --->       /   \     *
//       [a]   [x]              [y]   [c]   *
//            /   \            /   \        *
//          [b]   [c]        [a]   [b]      *
//
static void dast_tree_rotate_left(dast_tree_t* tree, dast_knot_t* x);

// Rotates to the right @tree around @x:
//
//          [p]                    [p]      *
//           |                      |       *
//          [x]                    [y]      *
//         /   \       <---       /   \     *
//       [a]   [y]              [x]   [c]   *
//            /   \            /   \        *
//          [b]   [c]        [a]   [b]      *
//
static void dast_tree_rotate_right(dast_tree_t* tree, dast_knot_t* x);

static void dast_tree_add_fix_up(dast_tree_t* tree, dast_knot_t* x);
static void dast_tree_remove_fix_up(dast_tree_t* tree, dast_knot_t* x);
static void dast_tree_transplant(dast_tree_t* tree, dast_knot_t* x, dast_knot_t* y);

void dast_tree_init(dast_tree_t* tree, int obj_size, int (*cmp)(void* l, void* r))
{
    tree->root = 0;
    tree->elems = 0;
    tree->cmp = cmp;
    tree->obj_size = 0;
}

void dast_tree_destroy(dast_tree_t* tree)
{
    dast_knot_t** glob = &(tree->root);
    dast_knot_t*  curr;

    while (*glob)
    {
        curr = *glob;
        if ((*glob = curr->left))
        {
        }
        else if ((*glob = curr->right))
        {
        }
        else if (curr->parent)
        {
            *glob = curr->parent;
            if (curr->parent->left == curr)
            {
                (*glob)->left = 0;
            }
            else
            {
                (*glob)->right = 0;
            }
            DAST_FREE(curr);
        }
        else
        {
            DAST_FREE(curr);
        }
    }
}

void dast_tree_copy(dast_tree_t* tree, dast_tree_t* dst)
{

}

void dast_tree_deepcopy(dast_tree_t* tree, dast_tree_t* dst, void (*cpy)(void* src, void* dst))
{

}

void dast_tree_clear(dast_tree_t* tree)
{
    dast_tree_destroy(tree);
    tree->elems = 0;
}

void dast_tree_deepclear(dast_tree_t* tree, void (*del)(void* obj))
{

}

unsigned long dast_tree_height(dast_tree_t* tree) { return dast_knot_height(tree->root); }


void dast_tree_add(dast_tree_t* tree, void* obj)
{
    int (*cmp)(void* l, void* r) = tree->cmp;

    dast_knot_t*      knot_new;
    dast_knot_t**     curr = &(tree->root);
    dast_knot_t*      parent = 0;
    int               compare_result;

    while (*curr)
    {
        parent = *curr;
        compare_result = cmp((char*)(parent) + sizeof(dast_knot_t), obj);
        if (compare_result > 0)
        {
            curr = &(parent->left);
        }
        else if (compare_result < 0)
        {
            curr = &(parent->right);
        }
        else
        {
            DAST_MEMCPY((char*)(parent) + sizeof(dast_knot_t), obj, tree->obj_size);
            return;
        }
    }

    knot_new = DAST_MALLOC(sizeof(dast_knot_t) + tree->obj_size);
    knot_new->is_black = 0;
    knot_new->left = 0;
    knot_new->right = 0;
    knot_new->parent = parent;
    DAST_MEMCPY((char*)(knot_new) + sizeof(dast_knot_t), obj, tree->obj_size);

    *curr = knot_new;
    tree->elems++;

    dast_tree_add_fix_up(tree, knot_new);
}

dast_knot_t* dast_tree_get(dast_tree_t* tree, void* obj)
{
    int (*cmp)(void* l, void* r) = tree->cmp;

    dast_knot_t** knot = &(tree->root);
    int           compare_result;

    while (*knot)
    {
        compare_result = cmp((char*)(*knot) + sizeof(dast_knot_t), obj);
        if (compare_result > 0)
        {
            knot = &((*knot)->left);
        }
        else if (compare_result < 0)
        {
            knot = &((*knot)->right);
        }
        else
        {
            return *knot;
        }
    }

    return 0;
}

void dast_tree_remove(dast_tree_t* tree, dast_knot_t* knot)
{
    dast_knot_t* z;
    dast_knot_t* x;
    dast_knot_t* y;
    unsigned char y_is_black;

    y = z = knot;
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
    DAST_FREE(z);

    if (y_is_black)
    {
        dast_tree_remove_fix_up(tree, x);
    }

    tree->elems--;
}

dast_knot_t* dast_tree_min(dast_tree_t* tree) { return dast_knot_min(tree->root); }

dast_knot_t* dast_tree_max(dast_tree_t* tree) { return dast_knot_max(tree->root); }

dast_knot_t* dast_tree_next(dast_tree_t* tree, dast_knot_t* cursor)
{
    if (cursor->right)
    {
        cursor = cursor->right;
        while (cursor->left)
        {
            cursor = cursor->left;
        }
    }
    else if (cursor->parent)
    {
        if (cursor == cursor->parent->left)
        {
            cursor = cursor->parent;
        }
        else
        {
            while (cursor->parent && cursor->parent->right == cursor)
            {
                cursor = cursor->parent;
            }
            cursor = cursor->parent;
        }
    }
    else
    {
        cursor = 0;
    }
    return cursor;
}

dast_knot_t* dast_tree_prev(dast_tree_t* tree, dast_knot_t* cursor)
{
    if (cursor->left)
    {
        cursor = cursor->left;
        while (cursor->right)
        {
            cursor = cursor->right;
        }
    }
    else if (cursor->parent)
    {
        if (cursor == cursor->parent->right)
        {
            cursor = cursor->parent;
        }
        else
        {
            while (cursor->parent && cursor->parent->left == cursor)
            {
                cursor = cursor->parent;
            }
            cursor = cursor->parent;
        }
    }
    else
    {
        cursor = 0;
    }
    return cursor;
}

/// private

dast_knot_t* dast_knot_min(dast_knot_t* knot)
{
    if (knot)
    {
        while (knot->left)
        {
            knot = knot->left;
        }
    }
    return knot;
}

dast_knot_t* dast_knot_max(dast_knot_t* knot)
{
    if (knot)
    {
        while (knot->right)
        {
            knot = knot->right;
        }
    }
    return knot;
}

unsigned long dast_knot_height(dast_knot_t* knot)
{
    if (!knot) { return 0; }
    unsigned long lheight = dast_knot_height(knot->left);
    unsigned long rheight = dast_knot_height(knot->right);
    return lheight >= rheight ? lheight + 1 : rheight + 1;
}

void dast_tree_rotate_left(dast_tree_t* tree, dast_knot_t* x)
{
    dast_knot_t* y = x->right;
    x->right = y->left;
    if (y->left)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == 0)
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
    y->left = x;
    x->parent = y;
}

void dast_tree_rotate_right(dast_tree_t* tree, dast_knot_t* x)
{
    dast_knot_t* y = x->left;
    x->left = y->right;
    if (y->right)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == 0)
    {
        tree->root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

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
            /* case left-right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    dast_tree_rotate_right(tree, x->parent);
                    dast_tree_rotate_left(tree, grandfather);
                    grandfather->is_black = 0;
                    x->is_black = 1;
                    return;
                }
                else
                {
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
                    dast_tree_rotate_left(tree, x->parent);
                    dast_tree_rotate_right(tree, grandfather);
                    grandfather->is_black = 0;
                    x->is_black = 1;
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
            /* case right-right */
            else
            {
                uncle = grandfather->left;
                if (!uncle || uncle->is_black)
                {
                    dast_tree_rotate_left(tree, grandfather);
                    grandfather->is_black = 0;
                    x->parent->is_black = 1;
                    return;
                }
                else
                {
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

static void dast_tree_remove_fix_up(dast_tree_t* tree, dast_knot_t* x)
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