#include "modularityMaximization.h"

/**
 * Algorithm 4
 */

void init_unmoved(int n, int *unmoved) {
    int i;

    for (i = 0; i < n; i++) {
        unmoved[i] = i;
    }
}

int find_max_index(const double *score, int n) {
    int i, max_index;
    double max, tmp_max;

    if (n == 0)
        return 0;

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

double calac_sum_Ai(matrixStructure *matrix_structure, group *g, const int *g_arr, const double *s, int i) {
    int i_index, j, j_index, row_start, row_end, *nodes, *colind;
    spmat *A;
    double sum = 0;

    nodes = g -> nodes;
    A = matrix_structure -> A;
    colind = A -> colind;
    i_index = nodes[i];
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];

    for (j = row_start; j < row_end; j++) {
        j_index = colind[j];
        if (g_arr[j_index] != 0) {
            sum += (s[g_arr[j_index] - 1]);
        }
    }
    return sum;
}

double calac_sum_Ki(matrixStructure *matrix_structure, group *g, const double *s, int i){
    int i_index, j, j_index, M, k_i, k_j, *K, *nodes;
    double sum = 0;
    M = matrix_structure -> M;
    K = matrix_structure -> degreeList;
    nodes = g -> nodes;
    i_index = nodes[i];
    k_i = K[i_index];

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        k_j = K[j_index];
        sum += ((k_i * k_j * s[j]) / M);
    }

    return sum;
}

void update_score(matrixStructure *matrix_structure, group *g, int *g_arr, double *s, double *score, const int *unmoved) {
    int k, i, k_i, M, *K;
    double sum_Ai, sum_ki;

    M = matrix_structure -> M;
    K = matrix_structure -> degreeList;

    for (i = 0; i < g -> size; i++) {
        if (unmoved[i] != -1){
            k = unmoved[i];
            s[k] *= (-1);

            k_i = K[g -> nodes[k]];
            sum_Ai = calac_sum_Ai(matrix_structure, g, g_arr, s, k);
            sum_ki = calac_sum_Ki(matrix_structure, g, s, k);
            score[k] = 4 * s[k] * (sum_Ai - sum_ki) + ((double)4 * (k_i * k_i) / M);

            s[k] *= (-1);
        }
    }
}

void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *g_arr, double *s) {
    int i, j, n, j_index, i_index, *indices, *unmoved;
    double delta_Q, *score, *improve, *d;

    n = g -> size;

    score = (double*) malloc(n * sizeof(double));
    improve = (double*) malloc(n * sizeof(double));
    indices = (int*) malloc(n * sizeof(int));
    unmoved = (int*) malloc(n * sizeof(int));
    d = (double*) malloc(n * sizeof(double));
    if (score == NULL || improve == NULL || indices == NULL || unmoved == NULL || d == NULL)
        ERROR_HANDLER(MALLOC_FAILED);

    memcpy(d, s, n * sizeof(double));

    do {
        memcpy(s, d, n * sizeof(double));
        init_unmoved(n, unmoved);

        for (i = 0; i < n; i++) {
            update_score(matrix_structure, g, g_arr, d, score, unmoved);

            j_index = find_max_index(score, n);
            d[j_index] *= (-1);
            indices[i] = j_index;

            if (i == 0) {
                improve[i] = score[j_index];
            } else {
                improve[i] = improve[i-1] + score[j_index];
            }
            unmoved[j_index] = -1;
        }

        /* Find the maximum improvement of s and update s accordingly */
        i_index = find_max_index(improve, n);
        for (i = n - 1; i > i_index; i--) {
            j = indices[i];
            d[j] *= (-1);
        }

        if (i_index == n - 1) {
            delta_Q = 0;
        }
        else {
            delta_Q = improve[i_index];
        }

    } while (delta_Q > EPSILON());

    free(score);
    free(improve);
    free(indices);
    free(unmoved);
    free(d);
}
