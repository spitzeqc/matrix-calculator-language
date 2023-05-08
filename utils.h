#ifndef UTILS_H
#define UTILS_H

#include "structures/matrix.h"
#include "structures/tree.h"

matrix_t* utils_tree_to_matrix( tree_t* t, bucket_node_t** scope );
float utils_int_to_float( int i );

#endif