#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hashmap.h"
#include "matrix.h"

#include "../free_will.tab.h"

#define max(a, b) a>b?a:b

/* basic constructor */
tree_t *make_tree( int v, tree_t *l, tree_t *r )
{
	tree_t *p = (tree_t *)malloc(sizeof(tree_t));
	assert(p!=NULL);

	p->type = v;
	p->left = l;
	p->right = r;

	return p;
}

tree_t* make_id( bucket_node_t* b )
{
	tree_t *p = make_tree(ID, NULL, NULL);
	(p->attribute).sval =  b;
	return p;
}
tree_t* make_inum(int i) {
	tree_t *p = make_tree(INUM, NULL, NULL);
	p->attribute.ival = i;
	return p;
}
tree_t* make_rnum(float r) {
	tree_t *p = make_tree(RNUM, NULL, NULL);
	p->attribute.rval = r;
	return p;
}
tree_t* make_matrix( matrix_t* m )
{
	tree_t *p = make_tree(MATRIX, NULL, NULL);
	p->attribute.mval = m;
	return p;
}

/* calculates the rank for each node in the tree */
void tree_calculate_rank( tree_t* t, bool is_left ) {
	/* we are at a leaf */
	if(t->left == NULL && t->right == NULL) {
		t->rank = is_left ? 1 : 0;
		return;
	}

	/* calculate rank for left and right nodes */
	tree_calculate_rank( t->left, true );
	tree_calculate_rank( t->right, false );

	/* calculate rank for current node */
	int l_rank = (t->left)->rank;
	int r_rank = (t->right)->rank;
	
	if(l_rank == r_rank)
		t->rank = l_rank + 1;
	else
		t->rank = max(l_rank, r_rank);
}

/* pre-order tree traversal */
void aux_print_tree( const tree_t* t, int spaces );

void tree_print( const tree_t *t )
{
	fprintf(stderr, "\nPrinting tree:\n");
	aux_print_tree(t, 0);
}


void aux_print_tree( const tree_t* t, int spaces )
{
	if (t == NULL) return;

	/* indent tree */
	int i;
	for(i=0; i<spaces; ++i)
		fprintf(stderr, " ");

	switch(t->type) {
	
	case SCAN:
		fprintf(stderr, "[SCAN]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case PRINT:
		fprintf(stderr, "[PRINT]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case ADDOP:
		fprintf(stderr, "[ADDOP:%d]\n", t->attribute.opval);
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case MULOP:
		fprintf(stderr, "[MULTOP:%d]\n", t->attribute.opval);
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case ID:
		fprintf(stderr, "[ID:%s]\n", t->attribute.sval->name);
		break;
	case RNUM:
		fprintf(stderr, "[RNUM:%f]\n", t->attribute.rval);
		break;
	case INUM:
		fprintf(stderr, "[INUM:%d]\n", t->attribute.ival);
		break;
	case INTEGRAL:
		fprintf(stderr, "[INTEGRAL]\n");
		break;
	case RATIONAL:
		fprintf(stderr, "[RATIONAL]\n");
		break;
	case STMT_LIST:
		fprintf(stderr, "[STATEMENT LIST]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case ASSIGNOP:
		fprintf(stderr, "[ASSIGNOP]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case MATRIX_ROW:
		fprintf(stderr, "[MATRIX ROW]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case MATRIX_COL:
		fprintf(stderr, "[MATRIX COL]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	case MATRIX_ROW_COL:
		fprintf(stderr, "[MATRIX ROW COL]\n");
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
		break;
	default:
		fprintf(stderr, "[TOKEN(%d)]\n", t->type);
		fprintf(stderr, "L");
		aux_print_tree(t->left, spaces+4);
		fprintf(stderr, "R");
		aux_print_tree(t->right, spaces+4);
	}
}

/*
void aux_eval_tree( tree_t* t );
int eval_tree( tree_t* t ) {
	aux_eval_tree(t);
	return t->total;		
}
void aux_eval_tree ( tree_t* t ) {
	switch (t->type) {
	case RNUM:
	case INUM:
		return;
	case '+':
		aux_eval_tree( t->left );
		aux_eval_tree( t->right );
		t->total = (t->left)->total + (t->right)->total;
		break;
	case '*':
		aux_eval_tree( t->left );
		aux_eval_tree( t->right );
		t->total = (t->left)->total * (t->right)->total;
		break;
	default:
		fprintf( stderr, "Unknown token detected" );
		exit(1);
	}
}*/
