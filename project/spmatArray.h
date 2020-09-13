
/*
 * spmatArray module:
 * This module implements a data structure to represent a sparse matrix.
 * This module is used to store the input graph.
 *
 * We used a compressed sparse row implementation, which represents a matrix M by three (one-dimensional) arrays,
 * that respectively contain nonzero values, the extents of rows, and column indices..
 * */

#ifndef SPMATARRAY_H
#define SPMATARRAY_H

#include <stdlib.h>
#include "error_handler.h"
#include <stdio.h>

typedef struct _spmat {
    /* Matrix size (n*n) */
    int		n;
    int *values;
    int *colind, *rowptr;

    /* Adds row i the matrix. Called before any other call,
     * exactly n times in order (i = 0 to n-1) */
    void	(*add_row)(struct _spmat *A, const int *row, int i);

    /* Frees all resources used by A */
    void	(*free)(struct _spmat *A);

    /* Multiplies matrix A by vector v, into result (result is pre-allocated) */
    void	(*mult)(const struct _spmat *A, const double *v, double *result);

} spmat;

/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);
void free_array(struct _spmat *A);

#endif
