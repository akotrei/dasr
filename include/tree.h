#ifndef __DAST_TREE_H__
#define __DAST_TREE_H__

#include "interface/allocator.h"
#include "interface/iterator.h"
#include "utils/cmp.h"
#include "utils/display.h"
#include "utils/mem.h"

// @dast_knot_t type is responsible for accessing to left, right and parent
// knots nad keeping itself color. A user data is kept directly behind this data
// type instance memory location:
// *dast_knot_t -> |--dast_knot_t--|--data--|
typedef struct _dast_knot_t dast_knot_t;
typedef struct _dast_knot_t
{
    dast_knot_t*  left;
    dast_knot_t*  right;
    dast_knot_t*  parent;
    unsigned char is_black;
} dast_knot_t;

typedef struct _dast_tree_t
{
    dast_knot_t*  root;
    unsigned long elems;
    int           (*cmp)(void* l, void* r);
    int           obj_size;
} dast_tree_t;

// Initialise @tree
// @obj_size is size of objects in bytes that the tree holds;
// @cmp is a compare function that is used to add new element, it accepts two
// objects a and b and returns 1 if a > b, 0 if a = b and -1 if a < b;
void dast_tree_init(dast_tree_t* tree, int obj_size, int (*cmp)(void* l, void* r));

// Destroy @tree and free all resources
void dast_tree_destroy(dast_tree_t* tree);

//
void dast_tree_copy(dast_tree_t* tree, dast_tree_t* dst);

// Does the same as @dast_tree_copy_to but additionally copies (uses objects copy function)
// trees objects to the new tree
void dast_tree_deepcopy(dast_tree_t* tree, dast_tree_t* dst, void (*cpy)(void* src, void* dst));

// Deletes all elements from the @tree
void dast_tree_clear(dast_tree_t* tree);

void dast_tree_deepclear(dast_tree_t* tree, void (*del)(void* obj));

// Returns height of the @tree
int dast_tree_height(dast_tree_t* tree);

#define DAST_KNOT_OBJ(knot) ((char*)(knot) + sizeof(dast_knot_t))

// Adds a new element to the @tree
// @obj is an element to be added
// Note: If such object is already presented it will be overwritten
void dast_tree_add(dast_tree_t* tree, void* obj);

// Searchs an element in the @tree
// @obj is an element to be searched
// Returns a pointer to the searched element or 0 if the element does not present in the tree
dast_knot_t* dast_tree_get(dast_tree_t* tree, void* obj);

// Removes the @obj from the @tree
// Returns 1 if @obj has been found and deleted on 0 otherwise
void dast_tree_remove(dast_tree_t* tree, dast_knot_t* obj);

// Returs a smallest elemet from the @tree
dast_knot_t* dast_tree_min(dast_tree_t* tree);

// Returs a largest elemet from the @tree
dast_knot_t* dast_tree_max(dast_tree_t* tree);

dast_knot_t* dast_tree_next(dast_tree_t* tree, dast_knot_t* cursor);

dast_knot_t* dast_tree_prev(dast_tree_t* tree, dast_knot_t* cursor);

// Returs a largest elemet from the @tree
char* dast_tree_display(dast_tree_t* tree, dast_display_t display);

#endif // __DAST_TREE_H__