#include "tree/tree.h"
#include "tree/tree_private.h"

void* dast_tree_iterator_next(void* self)
{
    dast_tree_iterator_t* tree_iterator = (dast_tree_iterator_t*)self;
    dast_knot_t* knot = tree_iterator->knot;

    if (!knot)
    {
        return 0;
    }

    switch (tree_iterator->last_visited)
    {
        case LEFT:
            tree_iterator->knot = tree_iterator->knot->parent;
            tree_iterator->last_visited = PARENT;
            break;
        case PARENT:
            if (tree_iterator->knot->right)
            {
                tree_iterator->knot = tree_iterator->knot->right;
            }
            tree_iterator->last_visited = RIGHT;
            break;
        case RIGHT:
            break;

        default:
            break;
    }

    return (char*)knot + sizeof(dast_knot_t);
}