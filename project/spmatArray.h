#ifndef SOFTWAREPROJECT_SPMATARRAY_H
#define SOFTWAREPROJECT_SPMATARRAY_H

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

    void (*print)(struct _spmat *A, int n); //TODO: Delete

} spmat;

/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);
void free_array(struct _spmat *A);

#endif
