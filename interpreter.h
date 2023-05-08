#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "structures/tree.h"
#include "structures/hashmap.h"

#include "utils.h"

typedef struct stmt_result_s {
	int type;
	void* val;
} stmt_result_t;

stmt_result_t* interpreter_run( tree_t* program, bucket_node_t** scope );

#endif