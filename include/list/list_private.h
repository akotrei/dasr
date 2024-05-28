#ifndef __DAST_LIST_PRIVATE_H__
#define __DAST_LIST_PRIVATE_H__

#include "interface/allocator.h"
#include "list/list.h"

typedef struct _dast_node_t          dast_node_t;
typedef struct _dast_list_iterator_t dast_list_iterator_t;

typedef struct _dast_node_t
{
    dast_node_t* next;
    dast_node_t* prev;
} dast_node_t;

typedef struct _dast_list_iterator_t
{
    dast_iterator_t iterator;
    dast_list_t*    list;
    dast_node_t*    curr;
    dast_u8_t       reversed;

} dast_list_iterator_t;

typedef struct _dast_list_t
{
    dast_u64_t elems;
    dast_u64_t elem_size;

    dast_node_t* head;
    dast_node_t* tail;

    dast_allocator_t* allocator;

    dast_cpy_t cpy;
    dast_del_t del;
} dast_list_t;

void dast_list_iterator_next(dast_iterator_t* iterator);
void dast_list_iterator_prev(dast_iterator_t* iterator);
void dast_list_iterator_reset(dast_iterator_t* iterator);

#endif /* __DAST_LIST_PRIVATE_H__ */