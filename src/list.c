#include "list.h"
#include "utils/mem.h"


void dast_list_init(dast_list_t* list, dast_allocator_t* allocator, int obj_size)
{
    list->allocator = allocator;
    list->elems = 0;
    list->elem_size = obj_size;
    list->head = 0;
    list->tail = 0;
}

void dast_list_destroy(dast_list_t* list)
{
    dast_node_t*      tmp;
    dast_node_t*      curr = list->head;
    dast_allocator_t* allocator = list->allocator;
    while (curr)
    {
        tmp = curr;
        curr = curr->next;
        allocator->deallocate(allocator, tmp);
    }
}

void dast_list_copy(dast_list_t* list, dast_list_t* dst)
{
    dast_allocator_t* allocator = list->allocator;
    int               elem_size = list->elem_size;
    dast_node_t*      origin = list->head;
    dast_node_t*      prev = 0;
    dast_node_t**     pp = &(dst->head);

    dast_cpy_generic(list, dst, sizeof(dast_list_t));
    while (origin)
    {
        *pp = allocator->allocate(allocator, sizeof(dast_node_t) + elem_size);
        dast_cpy_generic(origin, *pp, sizeof(dast_node_t) + elem_size);
        (*pp)->prev = prev;
        prev = *pp;
        pp = &((*pp)->next);
        origin = origin->next;
    }
    dst->tail = prev;
    *pp = 0;
}

void dast_list_deepcopy(dast_list_t* list, dast_list_t* dst, void (*cpy)(void* src, void* dst))
{
    dast_allocator_t* allocator = list->allocator;
    int               elem_size = list->elem_size;
    dast_node_t*      origin = list->head;
    dast_node_t*      prev = 0;
    dast_node_t**     pp = &(dst->head);

    dast_cpy_generic(list, dst, sizeof(dast_list_t));
    while (origin)
    {
        *pp = allocator->allocate(allocator, sizeof(dast_node_t) + elem_size);
        dast_cpy_generic(origin, *pp, sizeof(dast_node_t));
        cpy((char*)origin + sizeof(dast_node_t), (char*)(*pp) + sizeof(dast_node_t));
        (*pp)->prev = prev;
        prev = *pp;
        pp = &((*pp)->next);
        origin = origin->next;
    }
    dst->tail = prev;
    *pp = 0;
}

void dast_list_reverse(dast_list_t* list)
{
    dast_node_t* new_head = list->head;
    dast_node_t* tmp;

    while (new_head)
    {
        tmp = new_head->next;
        new_head->next = new_head->prev;
        new_head->prev = tmp;
        new_head = tmp;
    }

    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

void dast_list_clear(dast_list_t* list)
{
    dast_node_t* tmp;
    dast_node_t* node = list->head;
    dast_allocator_t* allocator = list->allocator;
    while (node)
    {
        tmp = node;
        node = node->next;
        allocator->deallocate(allocator, tmp);
    }
    list->elems = 0;
    list->head = list->tail = 0;
}

void dast_list_deepclear(dast_list_t* list, void (*del)(void* obj))
{
    dast_node_t*      tmp;
    dast_node_t*      node = list->head;
    dast_allocator_t* allocator = list->allocator;
    while (node)
    {
        tmp = node;
        node = node->next;
        del((char*)tmp + sizeof(dast_node_t));
        allocator->deallocate(allocator, tmp);
    }
    list->elems = 0;
    list->head = list->tail = 0;
}

void dast_list_append(dast_list_t* list, dast_node_t* cursor, void* obj)
{
    dast_node_t* new_node = list->allocator->allocate(list->allocator, sizeof(dast_node_t) + list->elem_size);
    dast_cpy_generic(obj, (char*)new_node + sizeof(dast_node_t), list->elem_size);

    if (list->elems == 0)
    {
        list->head = list->tail = new_node;
        new_node->next = new_node->prev = 0;
    }
    else
    {
        new_node->next = cursor->next;
        new_node->prev = cursor;
        if (cursor->next)
        {
            cursor->next->prev = new_node;
        }
        cursor->next = new_node;
        list->tail = list->tail == cursor ? new_node : list->tail;
    }
    list->elems++;
}

void dast_list_prepend(dast_list_t* list, dast_node_t* cursor, void* obj)
{
    dast_node_t* new_node = list->allocator->allocate(list->allocator, sizeof(dast_node_t) + list->elem_size);
    dast_cpy_generic(obj, (char*)new_node + sizeof(dast_node_t), list->elem_size);

    if (list->elems == 0)
    {
        list->head = list->tail = new_node;
        new_node->next = new_node->prev = 0;
    }
    else
    {
        new_node->next = cursor;
        new_node->prev = cursor->prev;
        if (cursor->prev)
        {
            cursor->prev->next = new_node;
        }
        cursor->prev = new_node;
        list->head = list->head == cursor ? new_node : list->head;
    }
    list->elems++;
}

void dast_list_remove(dast_list_t* list, dast_node_t* cursor)
{
    dast_node_t* curr = cursor;
    dast_node_t* prev = curr->prev;
    dast_node_t* next = curr->next;

    if (prev)
    {
        prev->next = next;
    }
    else
    {
        list->head = next;
    }

    if (next)
    {
        next->prev = prev;
    }
    else
    {
        list->tail = prev;
    }

    list->allocator->deallocate(list->allocator, curr);
    list->elems--;
}