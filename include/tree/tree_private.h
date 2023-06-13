#ifndef __DAST_TREE_PRIVATE_H__
#define __DAST_TREE_PRIVATE_H__

#include "interface/iallocator.h"
#include "interface/iiterator.h"
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
    dast_cmp_f cmp;
    /* A copy function to copy an object before being added to the tree or when
     * the tree going to be copying deeply */
    dast_cpy_f cpy;
    /* A delete function to be invoked each time when the tree delete an object
     * or the tree going to be deletet itself */
    dast_del_f del;

    unsigned long     obj_size;
    unsigned long     count;
    dast_allocator_t* allocator;

    dast_knot_t* root;
} dast_tree_t;


typedef enum _dast_tree_iterator_last_visited_e {LEFT, PARENT, RIGHT} dast_tree_iterator_last_visited_e;

typedef struct _dast_tree_iterator_t
{
    dast_iterator_t iterator;
    dast_knot_t* knot;
    dast_tree_iterator_last_visited_e last_visited;
    
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

void* dast_tree_iterator_next(void* self);
void dast_tree_iterator_reset(void* self);

#endif /* __DAST_TREE_PRIVATE_H__ */