#include "tree/tree.h"
#include "tree/tree_private.h"
#include <string.h>

char** dast_knot_print(dast_knot_t*      knot,
                       dast_u64_t*       strings,
                       dast_u64_t*       capacity,
                       dast_u64_t*       width,
                       dast_u64_t*       height,
                       dast_u64_t*       middle,
                       dast_allocator_t* allocator,
                       dast_display_t    display)
{
    char *line_left, *line_right;
    /* dast_u64_t height; */
    /* dast_u64_t middle; */
    dast_u64_t line_size_left, line_size_right;

    if (!(knot->left) && !(knot->right))
    {

        char* repr = display((char*)(knot) + sizeof(dast_knot_t), allocator);
        *width = strlen(repr);
        *height = 1;
        *middle = *width / 2;

        char* line = allocator->allocate(allocator->allocate, *width);
        memcpy(line, repr, *width);
        allocator->deallocate(allocator, repr);

        char** list_of_strs = allocator->allocate(allocator->allocate, 1);
        list_of_strs[0] = line;
        *strings = 1;
        *capacity = 1;

        return list_of_strs;
    }

    if (!(knot->right))
    {
        char**     lines = dast_knot_print(knot->left, strings, capacity, width, height, middle, allocator, display);
        char*      curr = display((char*)(knot) + sizeof(dast_knot_t), allocator);
        dast_u64_t curr_size = strlen(curr);

        char* first_line = allocator->allocate(allocator, *width + curr_size);
        memset(first_line, ' ', (*middle) + 1);
        memset(first_line + (*middle) + 1, '_', (*width) - (*middle) - 1);
        memcpy(first_line + *width, curr, curr_size);

        char* second_line = allocator->allocate(allocator, *width + curr_size);
        memset(second_line, ' ', *middle);
        memset(second_line + *middle, '/', 1);
        memset(second_line + *middle + 1, ' ', *width + curr_size - (*middle) - 1);

        allocator->deallocate(allocator, curr);

        *strings += 2;
        if (*capacity < *strings)
        {
            lines = allocator->reallocate(allocator, (void*)lines, (*strings) * 2);
            *capacity = (*strings) * 2;
        }
    }
}

char* dast_tree_print(dast_tree_t* tree, dast_display_t display) { dast_u64_t a; }