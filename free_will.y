%{
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "structures/tree.h"
#include "structures/matrix.h"
#include "structures/hashmap.h"

#include "semantic.h"
#include "interpreter.h"
#include "utils.h"

bucket_node_t** scope;
bucket_node_t* id_ptr;

int yylex(void);
int yyerror(char* s);
%}

%union {
	int	ival;    /* INUM */
	float	rval;    /* RNUM */
	char	*sval;   /* ID */
	int	opval;   /* MULOP, ADDOP */
	int	typeval; /* INTEGRAL, RATIONAL */

	tree_t	*tval;   /* tree attribute */
}

%token MATRIX
%token <opval> ADDOP
%token PLUS MINUS
%token <opval> MULOP
%token STAR SLASH DIV MOD
%token <sval> ID
%token INTEGRAL RATIONAL
%token <ival> INUM
%token <rval> RNUM
%token ASSIGNOP
%token PRINT
%token SCAN
%token VAR

%type <tval> statement_list
%type <tval> statement
%type <tval> expression
%type <tval> term
%type <tval> factor
%type <tval> var
%type <tval> program

%token STMT_LIST
%token MATRIX_ACCESS
%token MATRIX_ROW_COL
%token MATRIX_ROW
%token MATRIX_COL

%type <tval> matrix
%type <tval> matrix_col
%type <tval> matrix_row

%token TYPE_ERROR
%%

program: statement_list
	{
		$$ = $1;
		interpreter_run( $$, scope );
	}
	;

statement_list: statement ';'
		{ $$ = make_tree( STMT_LIST, NULL, $1 ); }
	| statement ';' statement_list
		{ $$ = make_tree( STMT_LIST, $3, $1 ); }	
	;

statement: var ASSIGNOP expression
		{ 
			$$ = make_tree( ASSIGNOP, $1, $3 ); 
		}
	| PRINT expression
		{ $$ = make_tree( PRINT, $2, NULL ); }
	| SCAN var
		{ $$ = make_tree( SCAN, $2, NULL ); }
	| VAR ID
		{ $$ = NULL; semantic_insert( scope, $2 ); }
	;

expression: term
		{ $$ = $1; }
	| ADDOP term
		{ $$ = make_tree( ADDOP, $2, NULL ); $$->attribute.opval = $1; }
	| expression ADDOP term
		{ $$ = make_tree( ADDOP, $1, $3 ); $$->attribute.opval = $2; }
	;

term: factor
		{ $$ = $1; }
	| term MULOP factor
		{ $$ = make_tree( MULOP, $1, $3 ); $$->attribute.opval = $2; }
	;

factor: var
		{ $$ = $1; }
	| matrix
		{ $$ = make_matrix( utils_tree_to_matrix( $1, scope ) ); }
	| INUM
		{ $$ = make_inum( $1 ); }
	| RNUM
		{ $$ = make_rnum( $1 ); }
	| '(' expression ')'
		{ $$ = $2; }
	;

var: ID
		{ $$ = make_id( semantic_lookup( scope, $1 ) ); }
	| ID '[' expression ',' expression ']' /* access matrix element at <row>, <column> */
		{ $$ = make_tree( MATRIX_ACCESS, make_id( semantic_lookup( scope, $1 ) ), make_tree( MATRIX_ROW_COL, $3, $5 ) ); }
	;

matrix_row: expression
		{ $$ = make_tree( MATRIX_ROW, NULL, $1 ); }
	| matrix_row ',' expression
		{ $$ = make_tree( MATRIX_ROW, $1, $3 ); }
	;
matrix_col: matrix_row ';'
		{ $$ = make_tree( MATRIX_COL, NULL, $1 ); }
	| matrix_row ';' matrix_col
		{ $$ = make_tree( MATRIX_COL, $3, $1 ); }
	;
matrix: '[' matrix_col ']'
		{ 
			semantic_verify_matrix( $2 );
			$$ = $2; 
		}
	;

%%

int main() {
	scope = hashmap_new();
	yyparse();
}