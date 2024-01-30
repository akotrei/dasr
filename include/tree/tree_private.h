#ifndef __DAST_TREE_PRIVATE_H__
#define __DAST_TREE_PRIVATE_H__

#include "interface/allocator.h"
#include "interface/iterator.h"
#include "utils/cmp.h"
#include "utils/mem.h"

typedef struct _dast_knot_t dast_knot_t;

/* @dast_knot_t type is responsible for accessing to left, right and parent
 * knots nad keeping itself color. A user data is kept directly behind this data
 * type instance memory location:
 *
 *     *dast_knot_t -> |--dast_knot_t--|--data--|
 */
typedef struct _dast_knot_t
{
    unsigned char is_black;

    dast_knot_t* left;
    dast_knot_t* right;
    dast_knot_t* parent;
} dast_knot_t;

typedef struct _dast_tree_t
{
    /* A compare function to compare objects being added to the tree */
    dast_cmp_t cmp;
    /* A copy function to copy an object before being added to the tree or when
     * the tree going to be copying pr cloning deeply */
    dast_cpy_t cpy;
    /* A delete function to be invoked each time when the tree delete an object
     * or the tree going to be deletet itself */
    dast_del_t del;

    dast_u64_t        obj_size;
    dast_u64_t        size;
    dast_allocator_t* allocator;

    dast_knot_t* root;
} dast_tree_t;

typedef struct _dast_tree_iterator_t
{
    dast_iterator_t iterator;
    dast_tree_t*    tree;
    dast_knot_t*    curr;

} dast_tree_iterator_t;

/* Rotates to the left @tree around @x.right:
 *
 *          [p]                    [p]
 *           |                      |
 *          [x]                    [y]
 *         /   \       --->       /   \
 *       [a]   [y]              [x]   [c]
 *            /   \            /   \
 *          [b]   [c]        [a]   [b]
 */
void dast_tree_rotate_left(dast_tree_t* tree, dast_knot_t* x);

/* Rotates to the right @tree around @x.left:
 *
 *          [p]                    [p]
 *           |                      |
 *          [y]                    [x]
 *         /   \       <---       /   \
 *       [a]   [x]              [y]   [c]
 *            /   \            /   \
 *          [b]   [c]        [a]   [b]
 */
void dast_tree_rotate_right(dast_tree_t* tree, dast_knot_t* x);

void dast_tree_add_fix_up(dast_tree_t* tree, dast_knot_t* x);

unsigned long dast_knot_height(dast_knot_t* knot);

void* dast_tree_forward_iterator_next(void* self);
void* dast_tree_backward_iterator_next(void* self);
void  dast_tree_iterator_reset(void* self);

void* dast_knot_min(dast_knot_t* knot);
void* dast_knot_max(dast_knot_t* knot);

#endif /* __DAST_TREE_PRIVATE_H__ */