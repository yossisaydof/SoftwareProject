#include "divideIntoTwo.h"

/**
 /Algorithm 2 - Divide a group into two
*/

void sum_row_i(matrixStructure *matrix_structure, group *g, int i, double *sum1, double *sum2, const int *s) {
    /*  sum row i in B = SUM over j of (A_ij - (k_i * k_j / M) */
    int j, j_index, k_i, k_j, M, nnz_i, cnt_nnz = 0, row_start, row_end, A_ij, *nodes, *K;
    double B_ij;
    spmat *A;

    K = matrix_structure -> degreeList;
    M = matrix_structure -> M;
    A = matrix_structure -> A;
    nodes = g -> nodes;

    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start;

    k_i = K[i];
    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        if (i == j_index) continue;
        A_ij = 0;

        k_j =  K[j_index];
        if (cnt_nnz < nnz_i) {
            if (j_index == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        B_ij = (A_ij - (double)((double)(k_i * k_j) / M));

        *sum1 -= B_ij;
        *sum2 += (B_ij * s[j_index]);
    }
}

double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s) {

     /* deltaQ = 0.5 * (s^T * B_hat[g] * s)
     * notice that: s^T * B_hat[g] * s = SUM1 + SUM2
     * SUM1 = -SUM over i != j of (A_ij - (k_i * k_j / M))
     * SUM2 = 2 * SUM over i != j of [s_i * s_j *(A_ij - (k_i * k_j / M))]
    */
    double delta_Q, sum1 = 0, sum2 = 0;
    int i, index_i;
    for (i = 0; i < g -> size; i++) {
        index_i = g -> nodes[i];
        sum_row_i(matrix_structure, g, index_i, &sum1, &sum2, s);
        sum2 *= s[index_i];
    }

    sum2 *= 2;
    delta_Q = sum1 + sum2;

    return (delta_Q * 0.5);
}


void divide_g(group *g, group *g1, group *g2, const int *s) {
    int i, g1_index = 0, g2_index = 0;

    for (i = 0; i < g -> size; i++) {
        if (s[i] == 1) {
            g1 -> nodes[g1_index] = (g -> nodes[i]);
            g1_index++;
        } else {
            g2 -> nodes[g2_index] = (g -> nodes[i]);
            g2_index++;
        }
    }
}

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2) {
    int n, i, cnt_positive = 0, cnt_negative = 0, *s;
    double eigen_value, deltaQ, *eigen_vector;

    n = g -> size;

    /* compute leading eigenpair of the modularity matrix B_hat_g */
    eigen_vector = (double*) malloc(sizeof(double) * n);
    if (eigen_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    eigen_value = power_iteration(matrix_structure, g, eigen_vector);

    if (IS_NON_POSITIVE(eigen_value)) {
        /* eigen value is non zero so the group g is indivisible */
        g1 -> size = 0;
        g2 -> size = 0;
        return;
    }

    /* compute s = {s1,....,sn} where si in {+1, -1}, according to u1 */
    s = (int*) malloc(sizeof(int) * n);
    if (s == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++) {
        if (eigen_vector[i] < EPSILON()) {
            s[i] = -1;
            cnt_negative++;
        } else {
            s[i] = 1;
            cnt_positive++;
        }
    }

    /* compute deltaQ */
    deltaQ = compute_delta_Q(matrix_structure, g, s);

    if (eigen_value > EPSILON()) {
        deltaQ = improving_division_of_the_network(matrix_structure, g, s, deltaQ);
        cnt_negative = 0;
        cnt_positive = 0;
        for (i = 0; i < n; i++) {
            if (s[i] < 0)
                cnt_negative++;
            else
                cnt_positive++;
        }
    }

    if (IS_POSITIVE(deltaQ)) {
        /* */
        g1 -> size = cnt_positive;
        g1 -> nodes = malloc(sizeof(int) * cnt_positive);
        if (g1 -> nodes == NULL) {
            printf("%s", MALLOC_FAILED);
            exit(EXIT_FAILURE);
        }

        g2 -> size = cnt_negative;
        g2 -> nodes = malloc(sizeof(int) * cnt_negative);
        if (g2 -> nodes == NULL) {
            printf("%s", MALLOC_FAILED);
            exit(EXIT_FAILURE);
        }

        divide_g(g, g1, g2, s); /* divide g into two groups according to s */
    } else {
        /* if deltaQ <= 0 the group g is indivisible */
        g1 -> size = 0;
        g2 -> size = 0;
        return;
    }

    free(s);
    free(eigen_vector);
}
