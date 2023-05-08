#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

float matrix_get_value(matrix_t* m, int r, int c);             /* Returns a value at a given location (r/c start at 0) */
void matrix_set_value(matrix_t* m, int r, int c, float value); /* Sets a value at a spedified location (r/c start at 0) */

matrix_t* matrix_create(int r, int c)
{
	/* Allocate memory for a Matrix struct */
	matrix_t* m = calloc(1, sizeof(matrix_t));
	assert(m != NULL); /* Verify memory was allocated */

	/* Assign rows (r) and columns (c) */
	m->rows = r;
	m->columns = c;
	
	/* Allocate, assign, and verify memory for the Matrix values */
	m->values = (float*)calloc((r*c), sizeof(float));
	assert(m->values != NULL);

	return m;
}
 
void matrix_free(matrix_t* m)
{
	/* Verify an address has been passed */
	if(m == NULL) return;

	/* Free array, then free rest of Matrix */
	free(m->values);
	free(m);
} 

float matrix_get_value(matrix_t* m, int r, int c)
{
	/* Make sure arguments are valid */
	assert(m != NULL);
	int index = r*(m->columns)+c; /* Calculate array index for row and column */
	return (m->values)[index];	
}

void matrix_set_value(matrix_t* m, int r, int c, float value)
{
	if(m == NULL) return;
	int index = r*(m->columns)+c;
	m->values[index] = value;
}

matrix_t* matrix_multiply(matrix_t* m1, matrix_t* m2)
{
	/* Allocate memory for our new matrix */
	matrix_t* ret = matrix_create(m1->rows, m2->columns);
	int count = m1->rows * m2->columns;

	int i;
	int j;
	for(i=0; i<count; ++i) {
		int r = i/(ret->columns);
		int c = i%(ret->columns);
		float dot = 0;
		for(j=0; j<(m1->rows)-1; ++j) {
			dot += matrix_get_value(m1, r, j) * matrix_get_value(m2, j, c);
		}
		matrix_set_value(ret, r, c, dot);
	}

	return ret;
}

matrix_t* matrix_scalar_multiply( float s, matrix_t* m ) {
	matrix_t* ret = matrix_create(m->rows, m->columns);
	int count = m->rows * m->columns;
	int i;
	for(i=0; i<count; ++i)
		ret->values[i] = (m->values)[i] * s;

	return ret;
}

matrix_t* matrix_add( matrix_t* m1, matrix_t* m2 ) {
	matrix_t* ret = matrix_create(m1->rows, m1->columns);
	int count = m1->rows * m1->columns;

	int i;
	for(i=0; i<count; ++i)
		ret->values[i] = (m1->values)[i] + (m2->values)[i];

	return ret;
}

void matrix_print(matrix_t* m) {
	if( m == NULL ) return;
	int i;
	int j;
	fprintf(stdout, "[ ");
	/* Go over each row */
	for(i=0; i<(m->rows); ++i) {
		if(i != 0) fprintf(stdout, "  ");
		/* Print each item in row 'i'*/
		for(j=0; j<(m->columns); ++j) {
			fprintf(stdout, "%f ", matrix_get_value(m, i, j));
		}
		if(i+1 >= (m->rows)) fprintf(stdout, "]");
		printf("\n");
	}
}
