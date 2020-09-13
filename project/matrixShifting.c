#include "matrixShifting.h"

double sum_of_row(matrixStructure *matrix_structure, int i) {
    int j, k_i, k_j, nnz_i, cnt_nnz = 0, A_ij, row_start, row_end, *K;
    double M, sum1 = 0, sum2 = 0;
    spmat *A;

    A = matrix_structure -> A;
    K = matrix_structure -> degreeList;
    k_i = K[i];
    M = matrix_structure -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start;

    for (j = 0; j < A -> n; j++) {
        if (i == j) continue;
        A_ij = 0;
        k_j = K[j];
        if (cnt_nnz < nnz_i) {
            if (j == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        sum1 += A_ij - (double)((k_i * k_j) / M);
        sum2 += fabs(A_ij - (double)((k_i * k_j) / M));
    }

    sum1 = fabs(sum1);

    return sum1 + sum2;
}


double norm_l1(matrixStructure *matrix_structure) {
    /*
     * Calculates norm l1 of the matrix
     * ||C||_1 = max_j (sum_i (|C_ij))
     * Notice that our matrix represents undirected graph, therefore sum of row i = sum of column i
     * */
    int i;
    double max = 0, tmp_sum;
    spmat *A;

    A = matrix_structure -> A;
    for (i = 0; i < A -> n; i++) {
        tmp_sum = sum_of_row(matrix_structure, i); /* instead of calculating sum of column, we calculate sum of row */
        max = tmp_sum > max ? tmp_sum : max;
    }
    return max;
}
