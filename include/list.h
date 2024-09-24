#ifndef __DAST_LIST_H__
#define __DAST_LIST_H__

#include "memory.h"

// memory laoyot of @dast_node_t type:
// |--next--|--prev--|--value--|
typedef struct _dast_node_t dast_node_t;
typedef struct _dast_node_t { dast_node_t *next, *prev; } dast_node_t;

typedef struct _dast_list_t
{
    dast_node_t*      head;
    dast_node_t*      tail;
    unsigned long     elems;
    int               elem_size;
} dast_list_t;

// Initialize @list to hold elemtnts with @obj_size
void dast_list_init(dast_list_t* list, int obj_size);

// Delete @list and free all memory that is used by @list
void dast_list_destroy(dast_list_t* list);

// Copy @list to @dst. Note: it is a shallow copying
// Note: @dst supposed to be not initialized
void dast_list_copy(dast_list_t* list, dast_list_t* dst);

// Copy @list to @dst. Also it copies all objcets too (invokes @cpy for each object)
// Note: @dst supposed to be not initialized
void dast_list_deepcopy(dast_list_t* list, dast_list_t* dst, void (*cpy)(void* src, void* dst));

// Reverse all elements in @list (a[0] <-> a[n-1], a[1] <-> a[n-2], ...)
// where n - number of elements in @list
void dast_list_reverse(dast_list_t* list);

// Remove all lements from @list
void dast_list_clear(dast_list_t* list);

// Remove all lements from @list and invoke @del on each one
void dast_list_deepclear(dast_list_t* list, void (*del)(void* obj));

// Get first node from @list, if @list is empty return NULL
// To extract user data use @DAST_LIST_NODE_ELEM
#define DAST_LIST_FIRST(list) ((list)->head)

// Get last node from @list, if @list is empty return NULL
// To extract user data use @DAST_LIST_NODE_ELEM
#define DAST_LIST_LAST(list) ((list)->tail)

// Get next node of a list that follows @cursor
// @cursor should be a result of @DAST_LIST_FIRST, @DAST_LIST_LAST, @DAST_LIST_NEXT,
// and @DAST_LIST_PREV and not be a NULL
#define DAST_LIST_NEXT(cursor) ((cursor)->next)

// Get previous node of @cursor of a list i.e. that is before @cursor
// @cursor should be a result of  @DAST_LIST_FIRST, @DAST_LIST_LAST, @DAST_LIST_NEXT,
// and @DAST_LIST_PREV and not be a NULL
#define DAST_LIST_PREV(cursor) ((cursor)->prev)

// Get element from a node that is return by @DAST_LIST_FIRST, @DAST_LIST_LAST,
// @DAST_LIST_NEXT, and @DAST_LIST_PREV
#define DAST_NODE_ELEM(node) ((char*)(node) + sizeof(dast_node_t))

// Append @obj to @list after @cursor. if @list is empty ignore @cursor
// and just put @obj to @list, otherwise @cursor should be a result of
// @DAST_LIST_FIRST, @DAST_LIST_LAST, @DAST_LIST_NEXT,
// and @DAST_LIST_PREV and not be a NULL
void dast_list_append(dast_list_t* list, dast_node_t* cursor, void* obj);

// Insert right before @cursor @obj to @list. if @list is empty ignore @cursor
// and just put @obj to @list, otherwise @cursor should be a result of
// @DAST_LIST_FIRST, @DAST_LIST_LAST, @DAST_LIST_NEXT,
// and @DAST_LIST_PREV and not be a NULL
void dast_list_prepend(dast_list_t* list, dast_node_t* cursor, void* obj);

// Replace @cursor by @obj in @list.
// @cursor should be a result of @DAST_LIST_FIRST, @DAST_LIST_LAST, @DAST_LIST_NEXT,
// and @DAST_LIST_PREV and not be a NULL
#define DAST_LIST_REPLACE(list, cursor, obj) (dast_cpy_generic((obj), DAST_NODE_ELEM((cursor)), (list)->elem_size))

// Remove @cursor from @list @cursor should be a result of @DAST_LIST_FIRST,
// @DAST_LIST_LAST, @DAST_LIST_NEXT, and @DAST_LIST_PREV and not be a NULL.
// After removing @cursor can't be used as a cursor for any operation on @list
void dast_list_remove(dast_list_t* list, dast_node_t* cursor);

#endif // __DAST_LIST_H__
