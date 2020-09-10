#include "divideIntoTwo.h"
#include "powerIteration.h"

/**
 /Algorithm 2 - Divide a group into two
*/

double sum_row_i(matrixStructure *matrix_structure, group *g, int i) {
    /* sum row i in B = SUM over j of (A_ij - (k_i * k_j / M) */
    int j, k_i, k_j, M, nnz_i, cnt_nnz = 0, row_start, row_end, A_ij, *nodes, *K;
    double sum_row = 0;
    spmat *A;

    K = matrix_structure -> degreeList;
    M = matrix_structure -> M;
    A = matrix_structure -> A;
    nodes = g -> nodes;

    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; // number of non-zero elements in row i

    k_i = K[i];
    for (j = 0; j < g -> size; j++) {
        if (i == nodes[j]) continue;
        A_ij = 0;
        k_j =  K[nodes[j]];
        if (cnt_nnz < nnz_i) {
            if (nodes[j] == A -> colind[row_start + cnt_nnz]) {
                /* A_ij != 0 */
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        sum_row += (A_ij - (double)((double)(k_i * k_j) / M));
    }

    return sum_row;
}

double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s) {
    /*
     * deltaQ = 0.5 * (s^T * B_hat[g] * s)
     * notice that: s^T * B_hat[g] * s = SUM1 + SUM2
     * SUM1 = -SUM over i != j of (A_ij - (k_i * k_j / M))
     * SUM2 = 2 * SUM over i != j of (A_ij - (k_i * k_j / M))
     */
    double delta_Q, B_i, sum1 = 0, sum2 = 0;
    int i;

    for (i = 0; i < g -> size; i++) {
        B_i = sum_row_i(matrix_structure, g, g -> nodes[i]);
        sum1 -= B_i;
        sum2 += B_i;
    }

    sum2 *= 2;
    delta_Q = sum1 + sum2;

    return (delta_Q * 0.5);
}

void divide_g(group *g, group *g1,group *g2, int *s) {
    int i, g1_index = 0, g2_index = 0;

    for (i = 0; i < g -> size; i++) {
        if (s[i] == 1) {
            g1[g1_index] = g[i];
            g1_index++;
        } else {
            g2[g2_index] = g[i];
            g2_index++;
        }
    }
}

void divideIntoTwo(matrixStructure *matrix_structure, group *g, group *g1, group *g2) {
    // TODO - check the note written after the algorithm in page 5
    int n, *s, i, cnt_positive = 0, cnt_negative = 0;
    double eigen_value, calc, deltaQ, *eigen_vector;

    n = g -> size;
    // compute f_g

    // compute leading eigenpair of the modularity matrix B_hat_g
    eigen_vector = (double*) malloc(sizeof(double) * n);
    eigen_value = power_iteration(matrix_structure, g, eigen_vector); // TODO: fix power iteration

    if (eigen_value <= 0) {
        g1 -> size = 0;
        g2 -> size = 0;
        return;
    }

    // compute s = {s1,....,sn} where si in {+1, -1}, according to u1
    s = (int*) malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) {
        if (eigen_vector[i] < 0) {
            s[i] = -1;
            cnt_negative++;
        } else {
            s[i] = 1;
            cnt_positive++;
        }
    }

    // compute deltaQ
    deltaQ = compute_delta_Q(matrix_structure, f, g, s);
    if (deltaQ > 0) {
        g1->size = cnt_positive;
        g1->nodes = malloc(sizeof(int) * cnt_positive);

        g2->size = cnt_negative;
        g2->nodes = malloc(sizeof(int) * cnt_negative);

        divide_g(g, g1, g2, s); // divide g into two groups according to s
    }
    // if deltaQ <= 0 the group g is indivisible

    free(s);
    free(eigen_vector);
}



