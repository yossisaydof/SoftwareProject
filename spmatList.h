#ifndef SOFTWAREPROJECT_SPMATLIST_H
#define SOFTWAREPROJECT_SPMATLIST_H

typedef struct _spmat {
    /* Matrix size (n*n) */
    int		n;
    double value;
    int column_index;
    struct LinkedListNode *next;

    /* Adds row i the matrix. Called before any other call,
     * exactly n times in order (i = 0 to n-1) */
    void	(*add_row)(struct _spmat *A, const double *row, int i);

    /* Frees all resources used by A */
    void	(*free)(struct _spmat *A);

    /* Multiplies matrix A by vector v, into result (result is pre-allocated) */
    void	(*mult)(const struct _spmat *A, const double *v, double *result);

} spmat;

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate_list(int n);



#endif
