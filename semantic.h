#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "structures/hashmap.h"
#include "structures/tree.h"

bucket_node_t* semantic_insert( bucket_node_t** scope, char* name );
bucket_node_t* semantic_lookup( bucket_node_t** scope, char* name );
void semantic_verify_matrix( tree_t* t );
int semantic_type_of( tree_t* t );

#endif