#include "powerIteration.h"


/**
 /Algorithm 2 - Divide a group into two
*/

double compute_f(matrixStructure *matrix_structure, group *g, int i) {
    int j;

    for (j = 0; j < g -> size; j++) {

    }
    return 6.6;
}

double mult_row_i_with_s(matrixStructure *matrix_structure, group *g, int *s, int i) {
    /* calculates sum of row i in B_hat */
    int j, k_i, k_j, nnz_i, cnt_nnz = 0, A_ij, row_start, row_end, *K;
    double M, result
    spmat *A;

    A = matrix_structure -> A;
    K = matrix_structure -> degreeList;
    k_i = K[i];
    M = matrix_structure -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; // number of non-zero elements in row i
    // TODO
    for (j = 0; j < g -> size; j++) {
        if (i == g -> nodes[j]) continue;
        A_ij = 0;
        k_j = K[g -> nodes[j]];
        if (cnt_nnz < nnz_i) {
            if (g -> nodes[j] == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];

            }
        }
        result = A_ij - (k_i * k_j / M);
    }

    return result;
}

double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s) {
    double delta_Q = 0, tmp;
    int i;

    for (i = 0; i < g -> size; i++) {
        tmp = mult_row_i_with_s(matrix_structure, g, s, g -> nodes[i]);
        delta_Q += (double) (tmp * s[i]);
    }
    return delta_Q;
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
    double eigen_value, calc, *f, deltaQ, *eigen_vector;

    n = g -> size;
    // compute f_g
    f = (double*)malloc(sizeof(double) * n);
    compute_f_g(matrix_structure, g, f);

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



