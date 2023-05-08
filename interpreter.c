#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "structures/tree.h"
#include "structures/hashmap.h"
#include "structures/matrix.h"

#include "semantic.h"

#include "free_will.tab.h"

stmt_result_t* execute_statement( tree_t* t, bucket_node_t** scope );

stmt_result_t* add( tree_t* t, bucket_node_t** scope) {
	stmt_result_t* ret = malloc( sizeof( stmt_result_t ) );
	ret->type = TYPE_ERROR;
	ret->val = NULL;
	if( t == NULL ) return ret;

	stmt_result_t* left_returned_val;
	stmt_result_t* right_returned_val;

	right_returned_val = execute_statement( t->right, scope );
	left_returned_val = execute_statement( t->left, scope );

	if( t->left == NULL && t->right != NULL && t->attribute.opval == MINUS ) {
		free(ret);
		if(right_returned_val->type == INTEGRAL || right_returned_val->type == INUM) 	*(int*)(right_returned_val->val) = -1 * *(int*)right_returned_val->val;
		if(right_returned_val->type == RATIONAL || right_returned_val->type == RNUM) 	*(float*)(right_returned_val->val) = -1 * *(float*)right_returned_val->val;
		if(right_returned_val->type == MATRIX) {
			fprintf(stderr, "ERROR: cannot make a negative matrix\n");
			exit(1);
		}

		ret = right_returned_val;
	}
	else if( t->left == NULL && t->right != NULL && t->attribute.opval == PLUS ) {
		free(ret);
		ret = right_returned_val;
	}

	/* type promotion */
	if( ( left_returned_val->type == INTEGRAL || left_returned_val->type == INUM ) && ( right_returned_val->type == RATIONAL || right_returned_val->type == RNUM ) ) {
		left_returned_val->type = RATIONAL;
		*(float*)left_returned_val->val = *(int*)left_returned_val->val * 1.0;
	}
	if( left_returned_val->type == RATIONAL && right_returned_val->type == INTEGRAL ) {
		right_returned_val->type = RATIONAL;
		*(float*)right_returned_val->val = *(int*)right_returned_val->val * 1.0;
	}

	if( left_returned_val->type == RNUM )  left_returned_val->type = RATIONAL;
	if( right_returned_val->type == RNUM )  right_returned_val->type = RATIONAL;
	if( left_returned_val->type == INUM )  left_returned_val->type = INTEGRAL;
	if( right_returned_val->type == INUM )  right_returned_val->type = INTEGRAL;

	if( left_returned_val->type == right_returned_val->type ) {
		free(ret);
		ret = left_returned_val;
		switch( ret->type ) {
		case INUM:
		case INTEGRAL:
			*(int*)ret->val = t->attribute.opval == PLUS ? *(int*)ret->val + *(int*)right_returned_val->val : *(int*)ret->val - *(int*)right_returned_val->val;
			break;
		case RNUM:
		case RATIONAL:
			*(float*)ret->val = t->attribute.opval == PLUS ? *(float*)ret->val + *(float*)right_returned_val->val : *(float*)ret->val - *(float*)right_returned_val->val;
			break;
		case MATRIX:
			matrix_t* left_matrix = (matrix_t*)left_returned_val->val;
			matrix_t* right_matrix = (matrix_t*)right_returned_val->val;
			if(left_matrix->rows != right_matrix->rows || left_matrix->columns != right_matrix->columns) {
				fprintf(stderr, "ERROR: matrix sizes do not match\n");
				exit(1);
			}

			ret->val = matrix_add( left_matrix, right_matrix );

			break;
		default:
			fprintf(stderr, "ERROR: unable to add types %d %d\n", left_returned_val->type, right_returned_val->type);
			exit(1);
		}
	} else if ( left_returned_val->type == MATRIX || right_returned_val->type == MATRIX ) {
		fprintf(stderr, "ERROR: unable to add number to matrix\n");
		exit(1);
	}
	else {
		fprintf(stderr, "An unexpected error has occurred %d %d\n", left_returned_val->type, right_returned_val->type);
		exit(1);
	}
	
	return ret;
}

stmt_result_t* mult( tree_t* t, bucket_node_t** scope) {
	stmt_result_t* ret = malloc( sizeof( stmt_result_t ) );
	ret->type = TYPE_ERROR;
	ret->val = NULL;
	if( t == NULL ) return ret;

	stmt_result_t* left_returned_val;
	stmt_result_t* right_returned_val;

	right_returned_val = execute_statement( t->right, scope );
	left_returned_val = execute_statement( t->left, scope );

	/* type promotion */
	if( ( left_returned_val->type == INTEGRAL || left_returned_val->type == INUM ) && ( right_returned_val->type == RATIONAL || right_returned_val->type == RNUM ) ) {
		left_returned_val->type = RATIONAL;
		*(float*)left_returned_val->val = *(int*)left_returned_val->val * 1.0;
	}
	if( left_returned_val->type == RATIONAL && right_returned_val->type == INTEGRAL ) {
		right_returned_val->type = RATIONAL;
		*(float*)right_returned_val->val = *(int*)right_returned_val->val * 1.0;
	}

	if( left_returned_val->type == RNUM )  left_returned_val->type = RATIONAL;
	if( right_returned_val->type == RNUM )  right_returned_val->type = RATIONAL;
	if( left_returned_val->type == INUM )  left_returned_val->type = INTEGRAL;
	if( right_returned_val->type == INUM )  right_returned_val->type = INTEGRAL;

	if( left_returned_val->type == right_returned_val->type ) {
		free(ret);
		ret = left_returned_val;
		switch( ret->type ) {
		case INUM:
		case INTEGRAL:
			if(t->attribute.opval == STAR) *(int*)ret->val  = *(int*)ret->val * *(int*)right_returned_val->val;
			else if(t->attribute.opval == DIV || t->attribute.opval == SLASH) *(int*)ret->val = *(int*)ret->val / *(int*)right_returned_val->val;
			else *(int*)ret->val = *(int*)ret->val % *(int*)right_returned_val->val;
			break;
		case RNUM:
		case RATIONAL:
			if(t->attribute.opval == MOD) {
				fprintf(stderr, "ERROR: unable to mod rationals\n");
				exit(1);
			}
			*(float*)ret->val = t->attribute.opval == STAR ? *(float*)ret->val * *(float*)right_returned_val->val : *(float*)ret->val / *(float*)right_returned_val->val;
			break;
		case MATRIX:
			if(t->attribute.opval == SLASH || t->attribute.opval == DIV) {
				fprintf(stderr, "ERROR: unable to divide matricies\n");
				exit(1);
			} 
			if(t->attribute.opval == MOD){
				fprintf(stderr, "ERROR: unable to mod matricies\n");
				exit(1);
			}
			matrix_t* left_matrix = left_returned_val->val;
			matrix_t* right_matrix = right_returned_val->val;
			if(left_matrix->columns != right_matrix->rows) {
				fprintf(stderr, "ERROR: matrix sizes do not match\n");
				exit(1);
			}

			ret->val = matrix_multiply( left_matrix, right_matrix );
			break;
		default:
			fprintf(stderr, "ERROR: unable to add types %d %d\n", left_returned_val->type, right_returned_val->type);
			exit(1);
		}
	} else if ( right_returned_val->type == MATRIX ) {
		if(t->attribute.opval == SLASH || t->attribute.opval == DIV) {
				fprintf(stderr, "ERROR: unable to divide matricies\n");
				exit(1);
			} 
			if(t->attribute.opval == MOD){
				fprintf(stderr, "ERROR: unable to mod matricies\n");
				exit(1);
			}

		if( ( left_returned_val->type == INTEGRAL || left_returned_val->type == INUM ) )
			*(float*)left_returned_val->val = *(int*)left_returned_val->val * 1.0;

		ret->val = matrix_scalar_multiply( *(float*)left_returned_val->val, right_returned_val->val );
	} else if ( left_returned_val->type == MATRIX ) {
		fprintf(stderr, "ERROR: unable to multiply a number by a matrix\n");
		exit(1);
	} else {
		fprintf(stderr, "An unexpected error has occurred %d %d\n", left_returned_val->type, right_returned_val->type);
		exit(1);
	}
	
	return ret;
}

stmt_result_t* execute_statement( tree_t* t, bucket_node_t** scope ) {
	stmt_result_t* ret = malloc( sizeof( stmt_result_t ) );
	ret->type = TYPE_ERROR;
	ret->val = NULL;

	if(t == NULL) return ret;

	bucket_node_t* id_ptr;
	int type_val, old_type_val;
	void* returned_val = NULL;	


	switch( t->type ) {
	case ASSIGNOP:
		id_ptr = t->left->attribute.sval;

		old_type_val = id_ptr->type;				
		type_val = semantic_type_of( t->right ); /* get new type */
		id_ptr->type = type_val;

		ret = execute_statement( t->right, scope );

		/* free old value */
		if( old_type_val == MATRIX )		matrix_free( id_ptr->value.mval );
		else if( old_type_val == INTEGRAL )	free( id_ptr->value.ival );
		else if( old_type_val == RATIONAL )	free( id_ptr->value.rval );

		/* set new value */
		if( type_val == MATRIX )	id_ptr->value.mval = ret->val;
		else if( type_val == INTEGRAL )	(id_ptr->value).ival = (int*)(ret->val);
		else if( type_val == RATIONAL )	(id_ptr->value).rval = (float*)(ret->val);

		break;
	case INUM:
		returned_val = malloc( sizeof( int* ) );
		*(int*)returned_val = t->attribute.ival;
		ret->type = INUM;
		ret->val = returned_val;
		break;
	case RNUM:
		returned_val = malloc( sizeof( float* ) );
		*(float*)returned_val = t->attribute.rval;
		ret->type = RNUM;
		ret->val = returned_val;
		break;
	case MATRIX:
		ret->type = MATRIX;
		ret->val = t->attribute.mval;
		break;
	case PRINT:
		free(ret);
		ret = execute_statement( t->left, scope );
		if( ret->type == ID ) { 
			ret->type = ((bucket_node_t*)ret->val)->type;
			if(ret->type == INTEGRAL || ret->type == INUM)	{ ret->val = ((bucket_node_t*)ret->val)->value.ival; }
			if(ret->type == RATIONAL || ret->type == RNUM)	{ ret->val = ((bucket_node_t*)ret->val)->value.rval; }
			if(ret->type == MATRIX)				{ ret->val = ((bucket_node_t*)ret->val)->value.mval; }
		}

		if( ret->type == INTEGRAL || ret->type == INUM )	{ fprintf(stdout, "%d\n", *(int*)(ret->val)); break; }
		if( ret->type == RATIONAL || ret->type == RNUM )	{ fprintf(stdout, "%f\n", *(float*)(ret->val)); break; }
		if( ret->type == MATRIX )				{ matrix_print( ret->val ); break; }

		fprintf(stderr, "Attempting to print unknown token(%d)\n", ret->type);
		exit(1);
		break;
	case SCAN:
		fprintf(stderr, "Scanning not implemented\n");
		break;
	case STMT_LIST:
		free( ret );
		ret = execute_statement( t->right, scope );
		break;
	case ADDOP:
		free( ret );
		ret = add( t, scope );
		break;
	case MULOP:
		free( ret );
		ret = mult( t, scope );
		break;
	case MATRIX_ACCESS:
		ret->type = RATIONAL;
		int row = t->right->left->attribute.ival;
		int col = t->right->right->attribute.ival;
		matrix_t* m = t->left->attribute.sval->value.mval;
		ret->val = malloc( sizeof(float) );
		*(float*)ret->val = matrix_get_value(m, row, col);
		break;
	case ID:
		ret->type = ID;
		ret->val = t->attribute.sval;
		break;
	default:
		fprintf( stderr, "ERROR: unknown token(%d) encountered\n", t->type );
		exit(1);
	}

	return ret;
}

stmt_result_t* interpreter_run( tree_t* program, bucket_node_t** scope ) {
	stmt_result_t* ret = malloc( sizeof( stmt_result_t ) );
	if (program == NULL) return ret;

	tree_print(program);
	fprintf(stderr, "\n");

	tree_t* current_node = program;
	while(current_node != NULL) {
		ret = execute_statement( current_node, scope );
		current_node = current_node->left;
	}
	return ret;
}