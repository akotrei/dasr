#ifndef __DAST_TREE_PRIVATE_H__
#define __DAST_TREE_PRIVATE_H__

#include "interface/iallocator.h"

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
    int (*cmp_f)(void* l, void* r);
    /* A copy function to copy an object before being added to the tree or when
     * the tree going to be copying deeply */
    void (*cpy_f)(void* obj, void* memory);
    /* A delete function to be invoked each time when the tree delete an object
     * or the tree going to be deletet itself */
    void (*del_f)(void* obj);

    unsigned long      obj_size;
    unsigned long      length;
    dast_iallocator_t* allocator;

    dast_knot_t* root;
} dast_tree_t;

/* Rotates to the left @tree around @x:
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

/* Rotates to the right @tree around @x:
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

#endif /* __DAST_TREE_PRIVATE_H__ */