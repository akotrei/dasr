#include "list/list_private.h"

dast_u64_t dast_list_sizeof() { return sizeof(dast_list_t); }

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

void dast_list_destroy_from(void* list)
{  
    dast_list_t*      l = (dast_list_t*)list;
    dast_node_t*      tmp;
    dast_node_t*      node = l->head;
    dast_del_t        del = l->del;
    dast_allocator_t* allocator = l->allocator;
    while (node)
    {
        tmp = node;
        node = node->next;
        del((char*)(tmp) + sizeof(dast_node_t));
        allocator->deallocate(allocator, tmp);
    }
}

void dast_list_destroy(dast_list_t* list)
{
    dast_allocator_t* allocator = list->allocator;
    dast_list_destroy_from(list);
    allocator->deallocate(allocator, list);
}

dast_u64_t dast_list_size(dast_list_t* list) { return list->elems; }

dast_u64_t dast_list_obj_size(dast_list_t* list) { return list->elem_size; }

void dast_list_clear(dast_list_t* list)
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
    iter->iterator.next = dast_list_iterator_next;
    iter->iterator.prev = dast_list_iterator_prev;
    iter->iterator.reset = dast_list_iterator_reset;
    iter->reversed = reversed;
    iter->curr = reversed ? list->tail : list->head;
    iter->iterator.elem = iter->curr ? (char*)(iter->curr) + sizeof(dast_node_t) : 0;
    return (dast_iterator_t*)iter;
}

void dast_list_iterator_delete(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    iter->list->allocator->deallocate(iter->list->allocator, iter);
}

void dast_list_iterator_next(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    iter->curr = iter->curr->next;
    iter->iterator.elem = iter->curr ? (char*)(iter->curr) + sizeof(dast_node_t) : 0;
}

void dast_list_iterator_prev(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    iter->curr = iter->curr->prev;
    iter->iterator.elem = iter->curr ? (char*)(iter->curr) + sizeof(dast_node_t) : 0;
}

void dast_list_iterator_reset(dast_iterator_t* iterator)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    iter->curr = iter->reversed ? iter->list->tail : iter->list->head;
    iter->iterator.elem = iter->curr ? (char*)(iter->curr) + sizeof(dast_node_t) : 0;
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
    }
    else
    {
        new_node->next = 0;
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
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
    }
    else
    {
        new_node->next = list->head;
        new_node->prev = 0;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->elems++;
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
    dast_node_t* node = list->head;
    if (!list->head)
    {
        return 0;
    }

    if (dst)
    {
        list->cpy((char*)(list->head) + sizeof(dast_node_t), dst, list->elem_size);
    }

    list->head = list->head->next;
    if (!list->head)
    {
        list->tail = 0;
    }
    else
    {
        list->head->prev = 0;
    }
    list->elems--;
    list->del((char*)(node) + sizeof(dast_node_t));
    list->allocator->deallocate(list->allocator, node);
    return 1;
}

dast_u8_t dast_list_remove_back(dast_list_t* list, void* dst)
{
    dast_node_t* node = list->tail;
    if (!list->tail)
    {
        return 0;
    }

    if (dst)
    {
        list->cpy((char*)(list->tail) + sizeof(dast_node_t), dst, list->elem_size);
    }

    list->tail = list->tail->prev;
    if (!list->tail)
    {
        list->head = 0;
    }
    else
    {
        list->tail->next = 0;
    }
    list->elems--;
    list->del((char*)(node) + sizeof(dast_node_t));
    list->allocator->deallocate(list->allocator, node);
    return 1;
}

dast_u8_t dast_list_insert_before(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;
    dast_allocator_t*     allocator = list->allocator;
    dast_u8_t             status = 0;

    if (!list->elems)
    {
        dast_list_append(list, obj);
        iter->curr = list->head;
        iter->iterator.elem = (char*)(iter->curr) + sizeof(dast_node_t);
        status = 1;
    }
    else if (curr)
    {
        dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
        list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);
        new_node->next = curr;
        new_node->prev = curr->prev;
        if (curr->prev)
        {
            curr->prev->next = new_node;
        }
        curr->prev = new_node;
        iter->curr = new_node;
        list->head = list->head == curr ? new_node : list->head;
        iter->iterator.elem = (char*)(new_node) + sizeof(dast_node_t);
        list->elems++;
        status = 1;
    }
    return status;
}

dast_u8_t dast_list_insert_after(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;
    dast_allocator_t*     allocator = list->allocator;
    dast_u8_t             status = 0;

    if (!list->elems)
    {
        dast_list_append(list, obj);
        iter->curr = list->head;
        iter->iterator.elem = (char*)(iter->curr) + sizeof(dast_node_t);
        status = 1;
    }
    else if (curr)
    {
        dast_node_t* new_node = allocator->allocate(allocator, sizeof(dast_node_t) + list->elem_size);
        list->cpy(obj, (char*)(new_node) + sizeof(dast_node_t), list->elem_size);
        new_node->next = curr->next;
        new_node->prev = curr;
        if (curr->next)
        {
            curr->next->prev = new_node;
        }
        curr->next = new_node;
        iter->curr = new_node;
        list->tail = list->tail == curr ? new_node : list->tail;
        iter->iterator.elem = (char*)(new_node) + sizeof(dast_node_t);
        list->elems++;
        status = 1;
    }
    return status;
}

dast_u8_t dast_list_replace(dast_iterator_t* iterator, void* obj)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;

    if (!curr)
    {
        return 0;
    }

    list->del((char*)(curr) + sizeof(dast_node_t));
    list->cpy(obj, (char*)(curr) + sizeof(dast_node_t), list->elem_size);
    return 1;
}

dast_u8_t dast_list_remove(dast_iterator_t* iterator, void* dst, dast_u8_t move_backward)
{
    dast_list_iterator_t* iter = (dast_list_iterator_t*)iterator;
    dast_list_t*          list = iter->list;
    dast_node_t*          curr = iter->curr;

    if (!curr)
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

    iter->curr = move_backward ? prev : next;
    iter->iterator.elem = iter->curr ? (char*)(iter->curr) + sizeof(dast_node_t) : 0;

    if (dst)
    {
        list->cpy((char*)curr + sizeof(dast_node_t), dst, list->elem_size);
    }
    list->del((char*)curr + sizeof(dast_node_t));
    list->allocator->deallocate(list->allocator, curr);
    list->elems--;

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

void dast_list_copy_to(void* list, void* memory, dast_u64_t size)
{
    dast_list_t* l =  (dast_list_t*)list;
    dast_list_t* new_list = (dast_list_t*)memory;
    dast_allocator_t* allocator = l->allocator;
    dast_u64_t elem_size = l->elem_size;
    dast_node_t* origin = l->head;
    dast_node_t* prev = 0;
    dast_node_t** pp = &(new_list->head);

    dast_cpy_generic(l, new_list, size);

    while (origin)
    {
        *pp = allocator->allocate(allocator, sizeof(dast_node_t) + elem_size);
        dast_cpy_generic(origin, *pp, sizeof(dast_node_t) + elem_size);
        (*pp)->prev = prev;
        prev = *pp;
        pp = &((*pp)->next);
        origin = origin->next;
    }
    new_list->tail = prev;
    *pp = 0;
}

void dast_list_deepcopy_to(void* list, void* memory, dast_u64_t size)
{
    dast_list_t* l = (dast_list_t*)list;
    dast_list_t* new_list = (dast_list_t*)memory;
    dast_allocator_t* allocator = l->allocator;
    dast_cpy_t cpy = l->cpy;
    dast_u64_t elem_size = l->elem_size;
    dast_node_t* origin = l->head;
    dast_node_t* prev = 0;
    dast_node_t** pp = &(new_list->head);

    dast_cpy_generic(l, new_list, size);

    while (origin)
    {
        *pp = allocator->allocate(allocator, sizeof(dast_node_t) + elem_size);
        dast_cpy_generic(origin, *pp, sizeof(dast_node_t));
        cpy((char*)origin + sizeof(dast_node_t), (char*)(*pp) + sizeof(dast_node_t), elem_size);
        (*pp)->prev = prev;
        prev = *pp;
        pp = &((*pp)->next);
        origin = origin->next;
    }
    new_list->tail = prev;
    *pp = 0;
}

dast_list_t* dast_list_copy(dast_list_t* list, dast_allocator_t* allocator)
{
    allocator = allocator ? allocator : list->allocator;
    dast_list_t* new_list = allocator->allocate(allocator, sizeof(dast_list_t));
    dast_list_copy_to(list, new_list, sizeof(dast_list_t));
    new_list->allocator = allocator;
    return new_list;
}

dast_list_t* dast_list_deepcopy(dast_list_t* list, dast_allocator_t* allocator)
{
    allocator = allocator ? allocator : list->allocator;
    dast_list_t* new_list = allocator->allocate(allocator, sizeof(dast_list_t));
    dast_list_deepcopy_to(list, new_list, sizeof(dast_list_t));
    new_list->allocator = allocator;
    return new_list;
}