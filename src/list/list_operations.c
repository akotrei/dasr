#include "list/list_private.h"

dast_u64_t dast_list_sizeof() { return sizeof(dast_list_t); }

dast_u64_t dast_list_size(dast_list_t* list) { return list->elems; }
dast_u64_t dast_list_obj_size(dast_list_t* list) { return list->elem_size; }
void       dast_list_clear(dast_list_t* list)
{
    dast_node_t* node = list->head;
    dast_del_t   del = list->del;
    while (node)
    {
        del((char*)(node) + sizeof(dast_node_t));
        node = node->next;
    }
    list->elems = 0;
    list->head = list->tail = 0;
}

void* dast_list_front(dast_list_t* list)
{
    if (!(list->head))
    {
        return 0;
    }
    return (char*)(list->head) + sizeof(dast_node_t);
}

void* dast_list_back(dast_list_t* list)
{
    if (!(list->tail))
    {
        return 0;
    }
    return (char*)(list->tail) + sizeof(dast_node_t);
}

dast_iterator_t* dast_list_iterator_new(dast_list_t* list, dast_u8_t reversed)
{
    dast_list_iterator_t* iter = list->allocator->allocate(list->allocator, sizeof(dast_list_iterator_t));
    iter->list = list;
    iter->reversed = reversed;
    if (reversed)
    {
        iter->curr = list->tail;
    }
    else
    {
        iter->curr = list->head;
    }
    iter->iterator.reached = dast_list_iterator_reached;
    iter->iterator.get = dast_list_iterator_get;
    iter->iterator.next = dast_list_iterator_next;
    iter->iterator.prev = dast_list_iterator_prev;
    iter->iterator.reset = dast_list_iterator_reset;
    return (dast_iterator_t*)iter;
}

void dast_list_iterator_delete(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    iter->list->allocator->deallocate(iter->list->allocator, iter);
}

dast_u8_t dast_list_iterator_reached(dast_iterator_t* iterator) { return ((dast_list_iterator_t*)iterator)->curr == 0; }

void* dast_list_iterator_get(dast_iterator_t* iterator)
{
    return (char*)(((dast_list_iterator_t*)iterator)->curr) + sizeof(dast_node_t);
}

void dast_list_iterator_next(dast_iterator_t* iterator)
{
    dast_list_iterator_t* list_iterator = (dast_list_iterator_t*)iterator;
    list_iterator->curr = list_iterator->curr->next;
}

void dast_list_iterator_prev(dast_iterator_t* iterator)
{
    dast_list_iterator_t* list_iterator = (dast_list_iterator_t*)iterator;
    list_iterator->curr = list_iterator->curr->prev;
}

void dast_list_iterator_reset(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    if (iter->reversed)
    {
        iter->curr = iter->list->tail;
    }
    else
    {
        iter->curr = iter->list->head;
    }
}

void dast_list_append(dast_list_t* list, void* obj)
{
    dast_allocator_t* allocator = list->allocator;

    dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
    list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);

    if (!(list->tail))
    {
        list->head = list->tail = new_node;
        new_node->next = new_node->prev = 0;
        return;
    }

    new_node->next = 0;
    new_node->prev = list->tail;
    list->tail = new_node;
    list->elems++;
}

void dast_list_prepend(dast_list_t* list, void* obj)
{
    dast_allocator_t* allocator = list->allocator;

    dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
    list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);

    if (!(list->head))
    {
        list->head = list->tail = new_node;
        new_node->next = new_node->prev = 0;
        return;
    }

    new_node->next = list->head;
    new_node->prev = 0;
    list->head = new_node;
    list->elems++;
}

dast_u8_t dast_list_insert_before(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;
    dast_allocator_t*     allocator = list->allocator;

    if (!curr)
    {
        return 0;
    }

    list->elems++;
    if (curr == list->head)
    {
        dast_list_prepend(list, obj);
        return 1;
    }

    if (curr == list->tail)
    {
        dast_list_append(list, obj);
        return 1;
    }

    dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
    list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);
    new_node->next = curr;
    new_node->prev = curr->prev;
    curr->prev->next = new_node;
    curr->prev = new_node;
    return 1;
}

dast_u8_t dast_list_insert_after(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;
    dast_allocator_t*     allocator = list->allocator;

    if (!curr)
    {
        return 0;
    }

    list->elems++;
    if (curr == list->head)
    {
        dast_list_prepend(list, obj);
        return 1;
    }

    if (curr == list->tail)
    {
        dast_list_append(list, obj);
        return 1;
    }

    dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
    list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);
    new_node->next = curr->next;
    new_node->prev = curr;
    curr->next->prev = new_node;
    curr->prev = new_node;
    return 1;
}

dast_u8_t dast_list_replace(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;
    dast_allocator_t*     allocator = list->allocator;

    if (!curr)
    {
        return 0;
    }

    list->del((char*)(curr) + sizeof(dast_node_t));
    list->cpy(obj, (char*)(curr) + sizeof(dast_node_t), list->elem_size);
    return 1;
}

dast_u8_t dast_list_replace_front(dast_list_t* list, void* obj)
{
    if (!list->head)
    {
        return 0;
    }

    list->del((char*)(list->head) + sizeof(dast_node_t));
    list->cpy(obj, (char*)(list->head) + sizeof(dast_node_t), list->elem_size);
    return 1;
}

dast_u8_t dast_list_replace_back(dast_list_t* list, void* obj)
{
    if (!list->tail)
    {
        return 0;
    }

    list->del((char*)(list->tail) + sizeof(dast_node_t));
    list->cpy(obj, (char*)(list->tail) + sizeof(dast_node_t), list->elem_size);
    return 1;
}

dast_u8_t dast_list_remove_front(dast_list_t* list, void* dst)
{
    if (!list->head)
    {
        return 0;
    }

    if (dst)
    {
        list->cpy((char*)(list->tail) + sizeof(dast_node_t), dst, list->elem_size);
    }
    list->del((char*)(list->head) + sizeof(dast_node_t));

    list->head = list->head->next;
    if (!list->head)
    {
        list->tail = 0;
    }
    list->elems--;
    return 1;
}

dast_u8_t dast_list_remove_back(dast_list_t* list, void* dst)
{
    if (!list->tail)
    {
        return 0;
    }

    if (dst)
    {
        list->cpy((char*)(list->tail) + sizeof(dast_node_t), dst, list->elem_size);
    }
    list->del((char*)(list->tail) + sizeof(dast_node_t));

    list->tail = list->tail->prev;
    if (!list->tail)
    {
        list->head = 0;
    }
    list->elems--;
    return 1;
}

dast_u8_t dast_list_remove(dast_iterator_t* iterator, void* dst, dast_u8_t move_backward)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;

    if (curr)
    {
        return 0;
    }

    dast_node_t* prev = curr->prev;
    dast_node_t* next = curr->next;

    if (prev)
    {
        prev->next = next;
    }
    else
    {
        list->head = curr->next;
    }

    if (next)
    {
        prev = prev;
    }
    else
    {
        list->tail = curr->prev;
    }

    if (dst)
    {
        list->cpy((char*)curr + sizeof(dast_node_t), dst, list->elem_size);
    }
    list->del((char*)curr + sizeof(dast_node_t));
    list->allocator->deallocate(iter->list->allocator, curr);
    list->elems--;
    if (move_backward)
    {
        iter->curr = prev;
    }
    else
    {
        iter->curr = next;
    }
    return 1;
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