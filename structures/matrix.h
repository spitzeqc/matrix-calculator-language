#ifndef MATRIX_H
#define MATRIX_H
#include <stdint.h>

typedef struct matrix_s {
	int 	rows;    /* Number of rows in the Matrix */
	int 	columns; /* Number of columns in the Matrix */
	float*	values;  /* Array containing all values stored in the Matrix */
} matrix_t;

matrix_t* matrix_create(int r, int c); /* Creates a matrix with the specified number of rows and columns */
void matrix_free(matrix_t* m);                   /* Frees an allocated Matrix */       

matrix_t* matrix_multiply( matrix_t* m1, matrix_t* m2 ); /* Multiples two Matrixes and returns a new Matrix* */
matrix_t* matrix_scalar_multiply( float s, matrix_t* m ); /* multiply a matrix by a scalar */
matrix_t* matrix_add( matrix_t* m1, matrix_t* m2 ); /* add two matrixes and return a new matrix */
void matrix_set_value(matrix_t* m, int r, int c, float value);
float matrix_get_value(matrix_t* m, int r, int c);

void matrix_print( matrix_t* m ); /* Print a Matrix */

#endif
