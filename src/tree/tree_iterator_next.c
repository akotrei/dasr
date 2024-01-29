#include "tree/tree_private.h"

void* dast_tree_forward_iterator_next(void* self)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)self;
    dast_knot_t *         out, *tmp;
    out = tmp = tree_iterator->curr;

    if (!out)
    {
        return out;
    }

    if (tmp->right)
    {
        tmp = tmp->right;
        while (tmp->left)
        {
            tmp = tmp->left;
        }
        tree_iterator->curr = tmp;
    }
    else if (tmp->parent)
    {
        if (tmp == tmp->parent->left)
        {
            tree_iterator->curr = tmp->parent;
        }
        else
        {
            while (tmp->parent && tmp->parent->right == tmp)
            {
                tmp = tmp->parent;
            }
            tree_iterator->curr = tmp->parent;
        }
    }

    return (char*)out + sizeof(dast_knot_t);
}

void* dast_tree_backward_iterator_next(void* self)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)self;
    dast_knot_t *         out, *tmp;
    out = tmp = tree_iterator->curr;

    if (!out)
    {
        return out;
    }

    if (tmp->left)
    {
        tmp = tmp->left;
        while (tmp->right)
        {
            tmp = tmp->right;
        }
        tree_iterator->curr = tmp;
    }
    else if (tmp->parent)
    {
        if (tmp == tmp->parent->right)
        {
            tree_iterator->curr = tmp->parent;
        }
        else
        {
            while (tmp->parent && tmp->parent->left == tmp)
            {
                tmp = tmp->parent;
            }
            tree_iterator->curr = tmp->parent;
        }
    }

    return (char*)out + sizeof(dast_knot_t);
}