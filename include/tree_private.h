#ifndef __DAST_TREE_PRIVATE_H__
#define __DAST_TREE_PRIVATE_H__

#include "interface/allocator.h"
#include "interface/iterator.h"
#include "utils/cmp.h"
#include "utils/display.h"
#include "utils/mem.h"




/* Rotates to the left @tree around @x:
 *
 *          [p]                    [p]
 *           |                      |
 *          [y]                    [x]
 *         /   \       --->       /   \
 *       [a]   [x]              [y]   [c]
 *            /   \            /   \
 *          [b]   [c]        [a]   [b]
 */
void dast_tree_rotate_left(dast_tree_t* tree, dast_knot_t* x);

/* Rotates to the right @tree around @x:
 *
 *          [p]                    [p]
 *           |                      |
 *          [x]                    [y]
 *         /   \       <---       /   \
 *       [a]   [y]              [x]   [c]
 *            /   \            /   \
 *          [b]   [c]        [a]   [b]
 */
void dast_tree_rotate_right(dast_tree_t* tree, dast_knot_t* x);

void dast_tree_add_fix_up(dast_tree_t* tree, dast_knot_t* x);

dast_u64_t dast_knot_height(dast_knot_t* knot);

dast_knot_t* dast_knot_min(dast_knot_t* knot);
dast_knot_t* dast_knot_max(dast_knot_t* knot);

void dast_tree_remove_fix_up(dast_tree_t* tree, dast_knot_t* x);
void dast_tree_transplant(dast_tree_t* tree, dast_knot_t* x, dast_knot_t* y);

char** dast_knot_print(dast_knot_t*      knot,
                       dast_u64_t*       strings,
                       dast_u64_t*       capacity,
                       dast_u64_t*       width,
                       dast_u64_t*       height,
                       dast_u64_t*       middle,
                       dast_allocator_t* allocator,
                       dast_display_t    display);

#endif /* __DAST_TREE_PRIVATE_H__ */