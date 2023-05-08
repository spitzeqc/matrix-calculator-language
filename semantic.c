#include "semantic.h"

#include <stdio.h>
#include <stdlib.h>

#include "structures/hashmap.h"
#include "structures/tree.h"
#include "free_will.tab.h"

bucket_node_t* semantic_insert( bucket_node_t** scope, char* name ) {
	bucket_node_t* ret = hashmap_get_node( scope, name );

	if(ret != NULL) {
		fprintf(stderr, "ERROR: name(%s) has already been defined\n", name);
		exit(1);
	} 
	ret = hashmap_add_node( scope, name );

	return ret;
}

bucket_node_t* semantic_lookup( bucket_node_t** scope, char* name ) {
	bucket_node_t* p = hashmap_get_node( scope, name );
	if( p == NULL ) {
		fprintf(stderr, "ERROR: name(%s) used but never defined\n", name);
		exit(1);
	}
	return p;
}

/* get the "type" of a tree */
int semantic_type_of( tree_t* t ) {
	if(t == NULL) return TYPE_ERROR;
	int left_type, right_type;

	switch( t->type ) {
	case ID:
		return t->attribute.sval->type;
	case INTEGRAL:
	case INUM:
		return INTEGRAL;
	case RATIONAL:
	case RNUM:
		return RATIONAL;
	case MULOP:
	case ADDOP:
		left_type = semantic_type_of( t->left );
		right_type = semantic_type_of( t->right );

		/* allow type promotion */
		if(left_type == MATRIX || right_type == MATRIX) return MATRIX;
		if(left_type == RATIONAL || right_type == RATIONAL) return RATIONAL;
		return left_type;
	case MATRIX:
		return MATRIX;
	case MATRIX_ACCESS:
		return RATIONAL;
	default:
		return TYPE_ERROR;
	}
}


/* verify dimentions */
void semantic_verify_matrix( tree_t* t ) {
	if(t == NULL) return;

	int previous_column_count = -1;
	int current_column_count = 0;
	tree_t* current_column = t;
	tree_t* current_row;
	while( current_column != NULL ) {
		current_column_count = 0;
		current_row = current_column->right;
		while( current_row != NULL ) {
			current_column_count++;
			current_row = current_row->left;
		}
		if(previous_column_count == -1) previous_column_count = current_column_count;
		if(current_column_count != previous_column_count) {
			fprintf(stderr, "ERROR: Matrix dimentions are inconsistent\n");
			exit(1);
		}
		current_column = current_column->left;
	}
}