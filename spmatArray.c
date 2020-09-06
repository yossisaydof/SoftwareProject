#include "spmatArray.h"
#include <stdlib.h>
#include <stdio.h>

spmat* spmat_allocate_array(int n, int nnz);
void add_row_array(struct _spmat *A, const double *row, int i);
void free_array(struct _spmat *A);
void mult_array(const struct _spmat *A, const double *v, double *result);


spmat* spmat_allocate_array(int n, int nnz) {
    spmat *sparse_matrix;

    /* Allocating Memory for the sparse matrix */
    sparse_matrix = malloc(sizeof(spmat));

    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_array;
    sparse_matrix -> free = free_array;
    sparse_matrix -> mult = mult_array;

    sparse_matrix -> values = malloc(nnz * sizeof(double));
    sparse_matrix -> colind = malloc(nnz * sizeof(int));
    sparse_matrix -> rowptr = malloc((n + 1) * sizeof(int));

    sparse_matrix -> rowptr[0] = 0;
    sparse_matrix -> rowptr[n] = nnz;

    return sparse_matrix;
}

void add_row_array(struct _spmat *A, const double *row, int i) {
    int j, *colind, *rowptr;
    double *values;
    static int index = 0;

    values = A -> values;
    colind = A -> colind;
    rowptr = A -> rowptr;

    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0)
            continue;
        values[index] = row[j];
        colind[index] = j;
        index++;
    }
    rowptr[i + 1] = index;
}

void mult_array(const struct _spmat *A, const double *v, double *result) {
    int i, j;
    double sum;

    double *values;
    int *colind, *rowptr;

    rowptr = A -> rowptr;
    values = A -> values;
    colind = A -> colind;

    for (i = 0; i < A -> n; i++) {
        sum = 0;
        for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
            sum += (double) (values[j] * v[colind[j]]);
        }
        result[i] = sum;
    }
}
