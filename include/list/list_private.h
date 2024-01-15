#ifndef __DAST_LIST_PRIVATE_H__
#define __DAST_LIST_PRIVATE_H__

#include "interface/iallocator.h"

typedef struct _dast_list_node_t dast_list_node_t;

typedef struct _dast_list_node_t
{
    dast_list_node_t* node_next;
    dast_list_node_t* node_prev;
} _dast_list_node_t;

typedef struct _dast_list_t
{
    dast_list_node_t* nodes;
    unsigned long     count;
    dast_allocator_t* allocator;

    void* (*copy_f)(void* obj, void* memory);
    void (*del_f)(void* obj);
} _dast_list_t;

#endif /* __DAST_LIST_PRIVATE_H__ */