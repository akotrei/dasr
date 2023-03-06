# dast
**Main Data Structures And Algorithms For ANSI C Programming Language**

## Data structures

1. **array** (alike python list or c++ std::vector)
2. **list** (alike c++ std::list)
3. **set** (alike c++ std::unordered_set)
4. **table** (alike c++ std::unordered_map)
5. **tree** (alike c++ std::map)

## Algorithms

1. **bsearch** (binary search)
2. **qsort** (quick sort)
3. **msort** (merge sort)

## Interfaces

1. **allocator** (memory allocation, reallocation, freeing)
    * current implementations (include/allocator/allocator_std.h): dast_allocator_std (just a wrapper over glibc malloc, free, realloc)
2. **iterator** (allows to iterate over structure that implements this interface)

## Utils

1. **cmp** (functions for objects comparing: pointers, integer, floats, ascii strings)
2. **cpy** (functions for objects copying: only shallow)
2. **hash** (functions for hash calculating: pointers, integer, floats, ascii strings)

