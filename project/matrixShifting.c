#include "spmatArray.h"
#include <math.h>
#include <stdlib.h>

double sum_of_row(spmat *A) {
    // TODO: check how the column is represented
    int i, sum;

    sum = 0;



    return sum;
}

double norm_l1(spmat *A) { // A = B_hat[g]
    /* ||C||_1 = max_j (sum_i (|C_ij)) */
    int i, n;
    double max = 0, tmp_sum;

    n = A -> n;

    for (i = 0; i < n; i++) {
        // TODO: if A is a symmetric matrix - can we go over half the columns?
        tmp_sum = sum_of_row(A); // TODO: check what max_col should get as parameter
        tmp_sum > max ? max = tmp_sum : max;
    }

    return max;
}
void shift_matrix(spmat *A) {
    /* perform matrix shifting on a matrix A */
    double norm;

    norm = norm_l1(A);




}





