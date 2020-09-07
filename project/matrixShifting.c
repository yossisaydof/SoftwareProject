#include "spmatArray.h"
#include <math.h>

double max_col() {
    // TODO: check how the column is represented
}

double norm_l1(spmat *A) {
    /* ||C||_1 = max_j (sum_i (|C_ij)) */
    int i, n;
    double max = 0, tmp_sum;

    n = A -> n;

    for (i = 0; i < n; i++) {
        // TODO: if A is a symmetric matrix - can we go over half the columns?
        tmp_sum = max_col(A[i]); // TODO: check what max_col should get as parameter
        tmp_sum > max ? max = tmp_sum : ;
    }

    return max;
}
void shift_matrix(spmat *A) {
    /* perform matrix shifting on a matrix A */
    double norm;

    norm = norm_l1(A);




}





