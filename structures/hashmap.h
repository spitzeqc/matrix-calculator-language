#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

#include "matrix.h"

#define PRIME 211

typedef struct bucket_node_s {
	char* name;  /* identifier name */
	int type;    /* INTEGRAL, RATIONAL, MATRIX */

	union {
		int* ival;       /* INTEGRAL val */
		float* rval;     /* RATIONAL val */
		matrix_t* mval;  /* MATRIX val */
	} value; /* current value identifier is holding */
	struct bucket_node_s* next;
} bucket_node_t;

bucket_node_t** hashmap_new();
void hashmap_free( bucket_node_t** h );

bucket_node_t* hashmap_add_node( bucket_node_t** h, char* name );
bucket_node_t* hashmap_get_node( bucket_node_t** h, char* name );
bool hashmap_remove( bucket_node_t** h, char* name );
char* hashmap_get_value( bucket_node_t** h, char* name );

void hashmap_print( bucket_node_t** h );
#endif
