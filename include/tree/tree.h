#ifndef __DAST_TREE_H__
#define __DAST_TREE_H__

#include "interface/iallocator.h"
#include "interface/iiterator.h"
#include "utils/cmp.h"
#include "utils/mem.h"

typedef struct _dast_tree_t dast_tree_t;

/* Returns number of bytes that a @dast_tree_t instance needs */
unsigned long dast_tree_sizeof();

/* Initialises a new instance of @dast_tree_t on @dst memory, (@dst should point
 * to at least @dast_tree_sizeof() bytes).
 * @allocator is used for internal allocation;
 * @obj_size is size of objects in bytes that the tree can handle;
 * @cmp is a compare function that is used to add new element, it accepts two
 * objects a and b and returns 1 if a > b, 0 if a = b and -1 if a < b;
 * @cpy is a copy function that is used everytime when new an item is being added;
 * @del is a deletion function that is used everytime when an item is being deleted */
dast_tree_t* dast_tree_init(void*              dst,
                            dast_iallocator_t* allocator,
                            dast_u64_t         obj_size,
                            dast_cmp_t         cmp,
                            dast_cpy_t         cpy,
                            dast_del_t         del);

/* Does the same as @dast_tree_init except that it returns a new instance of @dast_tree_t on a memory
 * that is internally allocated by @allocator */
dast_tree_t* dast_tree_new(dast_iallocator_t* allocator,
                           dast_u64_t         obj_size,
                           dast_cmp_t         cmp,
                           dast_cpy_t         cpy,
                           dast_del_t         del);

/* Should be used to deinitialize a @dast_tree_t instance that was created by @dast_tree_init
 * @tree is a @dast_tree_t instance to be deinitialized */
void dast_tree_deinit(dast_tree_t* tree);

/* Should be used to destroy a @dast_tree_t instance that was created by @dast_tree_new
 * @tree is a @dast_tree_t instance to be destroyed */
void dast_tree_denew(dast_tree_t* tree);

/* Creates a copy of @tree that is being wrriten to @dst (@dst should point to at least
 * @dast_tree_sizeof() bytes) */
void dast_tree_copy(dast_tree_t* tree, void* dst, dast_u64_t size);

/* Does the same as @dast_tree_copy but additionally copies (uses objects copy function)
 * trees objects to the new tree */
void dast_tree_deepcopy(dast_tree_t* tree, void* dst, dast_u64_t size);

/* Does the same as @dast_tree_copy except that it returns a new instance of @dast_tree_t on a memory
 * that is internally allocated by @tree allocator */
dast_tree_t* dast_tree_clone(dast_tree_t* tree);

/* Does the same as @dast_tree_deepcopy except that it returns a new instance of @dast_tree_t on a memory
 * that is internally allocated by the @tree allocator */
dast_tree_t* dast_tree_deepclone(dast_tree_t* tree);

/* Deletes all elements from the @tree */
void dast_tree_clear(dast_tree_t* tree);

/* Returns number of elements in the @tree */
dast_u64_t dast_tree_size(dast_tree_t* tree);

/* Returns height of the @tree */
dast_u64_t dast_tree_height(dast_tree_t* tree);

/* Creates a new iterator for the @tree to iterate by elements */
dast_iterator_t* dast_tree_forward_iterator_new(dast_tree_t* tree);
dast_iterator_t* dast_tree_backward_iterator_new(dast_tree_t* tree);

/* Should be used to delete a tree iterator created by @dast_iterator_t
 * Note: lifetime of the tree iterator should not be more than a tree
 * for which this iterator is created */
void dast_tree_iterator_delete(dast_iterator_t* iterator);

/* Adds a new element to the @tree
 * @obj is an element to be added */
void dast_tree_add(dast_tree_t* tree, void* obj);

/* Searchs an element in the @tree
 * @obj is an element to be searched
 * Returns a pointer to the searched element or 0 if the element does not present in the tree */
void* dast_tree_search(dast_tree_t* tree, void* obj);

/* Removes the @obj from the @tree. @obj can be a pointer from @dast_tree_search */
void dast_tree_remove(dast_tree_t* tree, void* obj);

/* Returs a smallest elemets from the @tree */
void* dast_tree_min(dast_tree_t* tree);

/* Returs a largest elemets from the @tree */
void* dast_tree_max(dast_tree_t* tree);

#endif /* __DAST_TREE_H__ */