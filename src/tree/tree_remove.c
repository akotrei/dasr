#include "tree/tree.h"
#include "tree/tree_private.h"

dast_u8_t dast_tree_remove(dast_tree_t* tree, void* obj)
{
    void* obj_to_delete = dast_tree_search(tree, obj);
    if (!obj_to_delete)
    {
        return 0;
    }

    return 1;
}