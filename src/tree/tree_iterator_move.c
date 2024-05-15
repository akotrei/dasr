#include "tree/tree_private.h"

dast_u8_t dast_tree_iterator_reached(dast_iterator_t* iterator) { return ((dast_tree_iterator_t*)iterator)->curr == 0; }

void* dast_tree_iterator_get(dast_iterator_t* iterator)
{
    return (char*)(((dast_tree_iterator_t*)iterator)->curr) + sizeof(dast_knot_t);
}

void dast_tree_iterator_next(dast_iterator_t* iterator)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)iterator;
    dast_knot_t*          tmp = tree_iterator->curr;

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
    else
    {
        tree_iterator->curr = 0;
    }
}

void dast_tree_iterator_prev(dast_iterator_t* iterator)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)iterator;
    dast_knot_t*          tmp = tree_iterator->curr;

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
    else
    {
        tree_iterator->curr = 0;
    }
}