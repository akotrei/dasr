#include "list/list_private.h"

void dast_list_destroy_from(dast_list_t* list)
{
    dast_node_t* node = list->head;
    dast_del_t   del = list->del;
    while (node)
    {
        del((char*)(node) + sizeof(dast_node_t));
        node = node->next;
    }
}

void dast_list_destroy(dast_list_t* list)
{
    dast_allocator_t* allocator = list->allocator;
    dast_list_destroy_from(list);
    allocator->deallocate(allocator, list);
}