#ifndef __DAST_LIST_H__
#define __DAST_LIST_H__

#include "interface/allocator.h"

// memory laoyot of @dast_node_t type:
// |--next--|--prev--|--value--|
typedef struct _dast_node_t dast_node_t;
typedef struct _dast_node_t { dast_node_t *next, *prev; } dast_node_t;

typedef struct _dast_list_t
{
    dast_allocator_t* allocator;
    dast_node_t*      head;
    dast_node_t*      tail;
    unsigned long     elems;
    int               elem_size;
} dast_list_t;

// Initialize @list to hold elemtnts with @obj_size
// @allocator is used to manage all internal memory usage
void dast_list_init(dast_list_t* list, dast_allocator_t* allocator, int obj_size);

// Delete @list and free all memory that is used by @list
void dast_list_destroy(dast_list_t* list);

// Copy @list to @dst. Note: it is a shallow copying
void dast_list_copy(dast_list_t* list, dast_list_t* dst);

// Copy @list to @dst. Also it copies all objcets too (invokes @cpy for each object)
void dast_list_deepcopy(dast_list_t* list, dast_list_t* dst, void (*cpy)(void* src, void* dst));

// Reverse all elements in @list (a[0] <-> a[n-1], a[1] <-> a[n-2], ...)
// where n - number of elements in @list
void dast_list_reverse(dast_list_t* list);

// Remove all lements from @list
void dast_list_clear(dast_list_t* list);

// Remove all lements from @list and invoke @del on each one
void dast_list_deepclear(dast_list_t* list, void (*del)(void* obj));

// Get first element from @list, if @list is empty return NULL
void* dast_list_first(dast_list_t* list);

// Get last element from @list, if @list is empty return NULL
void* dast_list_last(dast_list_t* list);

// Get next element of @list that follows @cursor
// @cursor should be a result of @dast_list_first, @dast_list_last, @dast_list_next,
// and @dast_list_prev and not be a NULL
void* dast_list_next(dast_list_t* list, void* cursor);

// Get previous element of @cursor of @list i.e. that is before @cursor
// @cursor should be a result of @dast_list_first, @dast_list_last, @dast_list_next,
// and @dast_list_prev and not be a NULL
void* dast_list_prev(dast_list_t* list, void* cursor);

// Append @obj to @list after @cursor. if @list is empty ignore @cursor
// and just put @obj to @list, otherwise @cursor should be a result of
// @dast_list_first, @dast_list_last, @dast_list_next,
// and @dast_list_prev and not be a NULL
void dast_list_append(dast_list_t* list, void* cursor, void* obj);

// Insert right before @cursor @obj to @list. if @list is empty ignore @cursor
// and just put @obj to @list, otherwise @cursor should be a result of
// @dast_list_first, @dast_list_last, @dast_list_next,
// and @dast_list_prev and not be a NULL
void dast_list_prepend(dast_list_t* list, void* cursor, void* obj);

// Replace @cursor by @obj in @list.
// @cursor should be a result of @dast_list_first, @dast_list_last,
// @dast_list_next, and @dast_list_prev and not be a NULL
void dast_list_replace(dast_list_t* list, void* cursor, void* obj);

// Remove @cursor from @list @cursor should be a result of @dast_list_first,
// @dast_list_last, @dast_list_next, and @dast_list_prev and not be a NULL.
// After removing @cursor can't be used as a cursor for any operation on @list
void dast_list_remove(dast_list_t* list, void* cursor);

#endif /* __DAST_LIST_H__ */
