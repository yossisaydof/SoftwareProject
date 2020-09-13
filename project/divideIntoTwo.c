#include "divideIntoTwo.h"
#include "error_handler.h"
#include <stdio.h>

/**
 /Algorithm 2 - Divide a group into two
*/


int* allocate_unmoved(group *g, int *unmoved) {
    int i;

    for (i = 0; i < g -> size; i++) {
        unmoved[i] = i;
    }
    return unmoved;
}

int find_max_index(const double *score, int n) {
    int i, max_index;
    double max, tmp_max;

    max = score[0];
    max_index = 0;
    for (i = 1; i < n; i++) {
        tmp_max = score[i];
        if (tmp_max > max) {
            max = tmp_max;
            max_index = i;
        }
    }
    return max_index;
}

void remove_index_from_unmoved(int *unmoved, int max_index, int last_available_index) {
    unmoved[max_index] = unmoved[last_available_index];
    unmoved[last_available_index] = -1;
}


void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *s, double Q_0) {
    int i, k, j, n, j_index, i_index, *indices, *unmoved, last_available_index;
    double *score, *improve, delta_Q;

    n = g -> size;
    score = (double*) malloc(n * sizeof(double));
    improve = (double*) malloc(n * sizeof(double));
    indices = (int*) malloc(n * sizeof(int));
    unmoved = (int*) malloc(n * sizeof(int));
    if (score == NULL || improve == NULL || indices == NULL || unmoved == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    unmoved = allocate_unmoved(g, unmoved);
    last_available_index = n - 1; /* last available index in unmoved array */
    do {
        for (i = 0; i < n; i++) {

            /* Computing delta Q for the move of each unmoved vertex */
            for (k = 0; k < last_available_index; k++) {
                s[k] = s[k] * (-1);
                score[k] = compute_delta_Q(matrix_structure, g, s) - Q_0;
                s[k] = s[k] * (-1);
            }

            j_index = find_max_index(score, n);
            s[j_index] =  s[j_index] * (-1);
            indices[i] = j_index;

            if (i == 0)
                improve[i] = score[j_index];
            else
                improve[i] = improve[i - 1] + score[j_index];

            remove_index_from_unmoved(unmoved, j_index, last_available_index);
            last_available_index--;
        }

        /* Find the maximum improvement of s and update s accordingly */
        i_index = find_max_index(improve, n);
        for (i = n - 1; i > i_index; i--) {
            j = indices[i];
            s[j] = s[j] * (-1);
        }

        if (i_index == n - 1)
            delta_Q = EPSILON();
        else
            delta_Q = improve[i_index];

    } while (IS_POSITIVE(delta_Q));

    free(score);
    free(improve);
    free(indices);
    free(unmoved);
}

void sum_row_i(matrixStructure *matrix_structure, group *g, int i, double *sum1, double *sum2, const int *s) {
    /* sum row i in B = SUM over j of (A_ij - (k_i * k_j / M) */
    int j, k_i, k_j, M, nnz_i, cnt_nnz = 0, row_start, row_end, A_ij, *nodes, *K;
    double B_ij;
    spmat *A;

    K = matrix_structure -> degreeList;
    M = matrix_structure -> M;
    A = matrix_structure -> A;
    nodes = g -> nodes;

    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; /* number of non-zero elements in row i */

    k_i = K[i];
    for (j = 0; j < g -> size; j++) {
        if (i == nodes[j]) continue;

        A_ij = 0;
        k_j =  K[nodes[j]];
        if (cnt_nnz < nnz_i) {
            if (nodes[j] == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        B_ij = (A_ij - (double)((double)(k_i * k_j) / M));

        *sum1 -= B_ij;
        *sum2 += (B_ij * s[nodes[j]]);
    }
}


double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s) {
    /*
     * deltaQ = 0.5 * (s^T * B_hat[g] * s)
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
            g1 -> nodes[g1_index] = g -> nodes[i];
            g1_index++;
        } else {
            g2 -> nodes[g2_index] = g -> nodes[i];
            g2_index++;
        }
    }
}

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2) {
    /* TODO - check the note written after the algorithm in page 5 */
    int n, *s, i, cnt_positive = 0, cnt_negative = 0;
    double eigen_value, deltaQ, *eigen_vector;

    n = g -> size;
    /* compute f_g */

    /* compute leading eigenpair of the modularity matrix B_hat_g */
    eigen_vector = (double*) malloc(sizeof(double) * n);
    if (eigen_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    eigen_value = power_iteration(matrix_structure, g, eigen_vector); /* TODO: fix power iteration */

    if (IS_NON_POSITIVE(eigen_value)) {
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
        if (eigen_vector[i] < 0) { /* TODO: check epsilon */
            s[i] = -1;
            cnt_negative++;
        } else {
            s[i] = 1;
            cnt_positive++;
        }
    }

    /* compute deltaQ */
    deltaQ = compute_delta_Q(matrix_structure, g, s);

    if (eigen_value > 0) { /* TODO: check epsilon */
        improving_division_of_the_network(matrix_structure, g, s, deltaQ);
        cnt_negative = 0;
        cnt_positive = 0;
        for (i = 0; i < n; i++) {
            if (s[i] < 0)
                cnt_negative++;
            else
                cnt_positive++;
        }
    }

    if (deltaQ > 0) { /* TODO: check epsilon */
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
    }
    /* if deltaQ <= 0 the group g is indivisible */

    free(s);
    free(eigen_vector);
}
