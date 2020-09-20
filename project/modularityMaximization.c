#include "modularityMaximization.h"

/**
 * Algorithm 4
 */

void init_unmoved(int n, int *unmoved) {
    /*
     * Initialize unmoved array
     */
    int i;

    for (i = 0; i < n; i++) {
        unmoved[i] = i;
    }
}

int find_max_index_unmoved(const double *score, const int *unmoved, int n) {
    /*
     * Return argMax{score[j]: j in unmoved}
     */
    int i, max_index = 0, first_flag = 1;
    double max = 0;

    for (i = 0; i < n; i++) {
        if (unmoved[i] != -1) {
            /* i is unmoved vertex */
            if (first_flag) {
                max = score[i];
                max_index = i;
                first_flag = 0;
            } else {
                if (score[i] > max) {
                    max = score[i] ;
                    max_index = i;
                }
            }
        }
    }
    return max_index;
}

int find_max_index(const double *improve, int n) {
    /*
     * Return argMax{improve[j]: j}
     */
    int i, max_index;
    double max;

    max_index = 0;
    max = improve[0];

    for (i = 1; i < n; i++) {
        if (improve[i] > max) {
            max = improve[i];
            max_index = i;
        }
    }
    return max_index;
}

double calc_sum_Ai(matrixStructure *matrix_structure, group *g, const int *g_arr, const double *s, int i) {
    /*
     * Calculates sum over j of A[g]_ij * s[j]
     */
    int i_index, j, j_index, row_start, row_end, *nodes, *colind;
    double sum = 0;
    spmat *A;

    nodes = g -> nodes;
    A = matrix_structure -> A;
    colind = A -> colind;
    i_index = nodes[i];
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];

    for (j = row_start; j < row_end; j++) {
        /* Iterating over row i in A[g] */
        j_index = colind[j];
        if (g_arr[j_index] != 0) {
            /* j_index is in g */
            sum += ((double) (s[g_arr[j_index] - 1]));
        }
    }
    return sum;
}

double calc_sum_Ki(matrixStructure *matrix_structure, group *g, const double *s) {
    /*
     * Calculates sum over j of k_j * s_j
     */
    int j, j_index, k_j, *K, *nodes;
    double sum = 0;

    K = matrix_structure -> degreeList;
    nodes = g -> nodes;

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        k_j = K[j_index];
        sum += (double) (k_j * s[j]);
    }

    return sum;
}

void update_score(matrixStructure *matrix_structure, group *g, int *g_arr, double *s, double *score, const int *unmoved) {
    /*
     * Updates score array according to the following formula:
     *      score[i] = 4 * s[i] * [sum over j of ((A[g]_ij - K[g]_ij) * s[j]) + 4K[g]_ij
     */
    int i, i_index, k_i, M, *K;
    double sum_Ai, sum_ki, tmp;

    M = matrix_structure -> M;
    K = matrix_structure -> degreeList;

    for (i = 0; i < g -> size; i++) {
        if (unmoved[i] != -1) {
            /* i is unmoved vertex */
            s[i] *= (-1);

            i_index = g -> nodes[i];
            k_i = K[i_index];
            sum_Ai = calc_sum_Ai(matrix_structure, g, g_arr, s, i);
            sum_ki = ((double) k_i / M) * calc_sum_Ki(matrix_structure, g, s);
            tmp = (double) ((double) (k_i * k_i) / M);
            score[i] = 4.0 * s[i] * (sum_Ai - sum_ki) + (4.0 * tmp);

            s[i] *= (-1);
        }
    }
}


void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *g_arr, double *s) {
    /*
     * An implementation of algorithm 4
     */
    int i, j, n, j_index, i_index, *indices, *unmoved;
    double delta_Q, *score, *improve;

    n = g -> size;

    score = (double*) malloc(n * sizeof(double));
    improve = (double*) malloc(n * sizeof(double));
    indices = (int*) malloc(n * sizeof(int));
    unmoved = (int*) malloc(n * sizeof(int));

    if (score == NULL || improve == NULL || indices == NULL || unmoved == NULL)
        ERROR_HANDLER(MALLOC_FAILED)

    do {
        init_unmoved(n, unmoved);

        for (i = 0; i < n; i++) {
            /* Computing deltaQ for the move of each unmoved vertex */
            update_score(matrix_structure, g, g_arr, s, score, unmoved);

            /* Moving vertex j_index with a maximal score */
            j_index = find_max_index_unmoved(score, unmoved, n);
            s[j_index] *= (-1);
            indices[i] = j_index;

            if (i == 0) {
                improve[i] = score[j_index];
            } else {
                improve[i] = improve[i - 1] + score[j_index];
            }
            unmoved[j_index] = -1;
        }

        /* Find the maximum improvement of s and update s accordingly */
        i_index = find_max_index(improve, n);
        for (i = n - 1; i > i_index; i--) {
            j = indices[i];
            s[j] *= (-1);
        }

        if (i_index == n - 1) {
            delta_Q = 0;
        } else {
            delta_Q = improve[i_index];
        }

    } while (IS_POSITIVE(delta_Q));

    free(score);
    free(improve);
    free(indices);
    free(unmoved);
}