#include "spmatArray.h"


void add_row_array(struct _spmat *A, const int *row, int i) {
    int j, *colind, *rowptr, *values;
    static int index = 0;

    rowptr = (int*) A -> rowptr;
    values = (int*) A -> values;
    colind = (int*) A -> colind;

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
    int i, j, *values, *colind, *rowptr;
    double sum;

    rowptr = (int*) A -> rowptr;
    values = (int*) A -> values;
    colind = (int*) A -> colind;

    for (i = 0; i < A -> n; i++) {
        sum = 0;
        for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
            sum += (double) (values[j] * v[colind[j]]);
        }
        result[i] = sum;
    }
}

void free_array(struct _spmat *A) {
    free(A -> rowptr);
    free(A -> colind);
    free(A -> values);
    free(A);
}

spmat* spmat_allocate_array(int n, int nnz) {
    spmat *sparse_matrix;
    int *values, *colind, *rowptr;

    /* Allocating Memory for the sparse matrix */
    sparse_matrix = (spmat*) malloc(sizeof(spmat));
    if (sparse_matrix == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    values = (int*) malloc(nnz * sizeof(int));
    colind = (int*) malloc(nnz * sizeof(int));
    rowptr = (int*) malloc((n + 1) * sizeof(int));
    if (values == NULL || colind == NULL || rowptr == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_array;
    sparse_matrix -> free = free_array;
    sparse_matrix -> mult = mult_array;

    sparse_matrix -> values = values;
    sparse_matrix -> colind = colind;
    sparse_matrix -> rowptr = rowptr;

    sparse_matrix -> rowptr[0] = 0;
    sparse_matrix -> rowptr[n] = nnz;

    return sparse_matrix;
}
