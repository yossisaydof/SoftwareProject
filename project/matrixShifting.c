#include "group.h"
#include "spmatArray.h"
#include "matrixStructure.h"
#include <math.h>
#include <stdlib.h>

double sum_of_row(spmat *A, int i) {
    // TODO
    /* calculates sum of row i in A */
    int i, sum;

    sum = 0;
    for (i = 0; i < A -> n; i++) {
        for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
            sum += (double) (values[j]);
        }
    }

    sum = 0;

    return sum;
}


void calc_sum_of_col(matrixStructure *matrix_structure, group *g, double *f , int col_index) {
    int i, j, k, l;
    spmat *A;
    double *values;
    int *colind, *rowptr, k_i, k_j;

    A = matrix_structure -> A;
    rowptr = A -> rowptr;
    values = A -> values;
    colind = A -> colind;




}

double norm_l1(matrixStructure *matrix_structure) { // A = B_hat[g]
    /* ||C||_1 = max_j (sum_i (|C_ij)) */
    int i, n;
    double max = 0, tmp_sum, *sum_vector;
    spmat *A;

    A = matrix_structure -> A;
    n = A -> n;

    sum_vector = (double*) malloc((A->n) * sizeof(double));
    calc_sum_of_col(matrix_structure, sum_vector);

    for (i = 0; i < n; i++) {
        // TODO: if A is a symmetric matrix - can we go over half the columns?
        tmp_sum = sum_of_row(A, i); // TODO: check what max_col should get as parameter
        tmp_sum > max ? max = tmp_sum : max;
    }

    free(sum_vector);
    return max;
}
void shift_matrix(spmat *A) {
    /* perform matrix shifting on a matrix A */
    double norm;

    norm = norm_l1(A);




}





