#include "list/list_private.h"

dast_list_t* dast_list_init_on(void*             memory,
                               dast_allocator_t* allocator,
                               dast_u64_t        obj_size,
                               dast_cpy_t        cpy,
                               dast_del_t        del)
{
    dast_list_t* list = (dast_list_t*)memory;
    list->elems = 0;
    list->elem_size = obj_size;
    list->head = 0;
    list->tail = 0;
    list->allocator = allocator;
    list->cpy = cpy;
    list->del = del;
    return list;
}

dast_list_t* dast_list_init(dast_allocator_t* allocator, dast_u64_t obj_size, dast_cpy_t cpy, dast_del_t del)
{
    dast_list_t* list = allocator->allocate(allocator, sizeof(dast_list_t));
    list = dast_list_init_on(list, allocator, obj_size, cpy, del);
    return list;
}
