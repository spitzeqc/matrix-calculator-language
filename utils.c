#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "structures/matrix.h"
#include "structures/hashmap.h"

#include "semantic.h"
#include "interpreter.h"

#include "free_will.tab.h"

float utils_int_to_float( int i ) {
	return 1.0*i;
}

matrix_t* utils_tree_to_matrix( tree_t* t, bucket_node_t** scope ) {
	if( t == NULL ) return NULL;

	tree_print( t );
	int row_count = 0;
	int col_count = 0;
	tree_t* current_node = t;
	/* count rows and cols */
	while(current_node != NULL ) {
		row_count++;
		current_node = current_node->left;
	}
	current_node = t->right;
	while(current_node != NULL ) {
		col_count++;
		current_node = current_node->left;
	}

	matrix_t* ret = matrix_create(row_count, col_count);

	/* populate matrix */
	tree_t *current_row, *current_col;
	current_col = t;
	int col = col_count-1;
	int row = 0;
	stmt_result_t val;
	while(current_col != NULL) {
		current_row = current_col->right;
		while(current_row != NULL) {
			/* calculate current item value */
			val = *( interpreter_run( current_row->right, scope ) );

			float set_val;
			if( val.type == INTEGRAL || val.type == INUM ) set_val = utils_int_to_float( *(int*)val.val );
			if( val.type == MATRIX ) {
				fprintf(stderr, "ERROR: cannot put a matrix in a matrix\n");
				exit(1);
			}
			if( val.type == RATIONAL || val.type == RNUM ) set_val = *(float*)val.val;

			matrix_set_value(ret, row, col, set_val); 
			col--;
			current_row = current_row->left;
		}
		current_col = current_col->left;
		col=col_count-1;
		row++;
	}

	return ret;
}