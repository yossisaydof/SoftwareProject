#include "spmatArray.h"
#include "matrixStructure.h"
#include <math.h>
#include <stdlib.h>

double sum_of_row(matrixStructure *matrix_structure, int i) {
    /* calculates sum of row i in B_hat */
    int j, k_i, k_j, nnz_i, cnt_nnz = 0, A_ij = 0, row_start, row_end, *K;
    double M;
    spmat *A, sum1 = 0, sum2 = 0;

    A = matrix_structure -> A;
    K = matrix_structure -> degreeList;
    k_i = K[i];
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; // number of non-zero elements in row i

    for (j = 0; j < A -> n; j++) {
        if (i == j) continue;

        if (cnt_nnz < nnz_i) {
            // TODO: check if it should be <=
            if (j == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = A -> values[row_start + cnt_nnz];
            }
        }
        sum1 += A_ij - (k_i * k_j) / M;
        sum2 += abs(A_ij - (k_i * k_j) / M);
    }

    sum1 = abs(sum1);

    return sum1 + sum2;
}


double norm_l1(matrixStructure *matrix_structure) {
    /* ||C||_1 = max_j (sum_i (|C_ij)) */
    int i, n;
    double max = 0, tmp_sum;

    for (i = 0; i < A -> n; i++) {
        tmp_sum = sum_of_row(matrix_structure, i);
        tmp_sum > max ? max = tmp_sum : max;
    }

    return max;
}





