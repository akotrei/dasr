#include "array/array_private.h"

dast_u64_t dast_array_size(dast_array_t* array) { return array->elems; }
dast_u64_t dast_array_obj_size(dast_array_t* array) { return array->elem_size; }