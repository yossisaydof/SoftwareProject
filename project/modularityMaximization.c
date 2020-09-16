#include "modularityMaximization.h"

/**
 * Algorithm 4
 */
/*double EPSILON = 0.00001; */

int* allocate_unmoved(group *g, int *unmoved) {
    int i;

    for (i = 0; i < g -> size; i++) {
        unmoved[i] = i;
    }
    return unmoved;
}

int find_max_index(double *score, int n) {
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

double calc_score_i(matrixStructure *matrix_structure, group *g, int i, double *d) {
    /*
     * score[i] = 4 * d_j * sum over j in g of [(A[g]_ij - (k_i * k_j)/M) * d_j] + 4 * (k_i * k_i)/M
     */
    int j, j_index, i_index, A_ij, M, n, k_i, k_j, nnz_i, cnt_nnz = 0, row_start, row_end, *nodes;
    double sum = 0;
    spmat *A;

    M = matrix_structure -> M;
    A = matrix_structure -> A;
    nodes = g -> nodes;
    i_index = nodes[i];
    k_i = matrix_structure -> degreeList[i_index];
    n = g -> size;
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];
    nnz_i = row_end - row_start;

    for (j = 0; j < n; j++) {
        j_index = nodes[j];
        A_ij = 0;
        k_j = matrix_structure -> degreeList[j_index];
        if (cnt_nnz < nnz_i) {
            while (j_index > (A -> colind)[row_start + cnt_nnz]) {
                cnt_nnz++;
            }
            if (j_index == (A -> colind)[row_start + cnt_nnz]) {
                A_ij = (int) A -> values[row_start + cnt_nnz];
                cnt_nnz++;
            }
        }
        sum += ((A_ij - (k_j * k_j / M)) * d[j]);
    }

    sum = (4 * d[i]) * sum + (4 * (k_i * k_i / M));

    return sum;
}
double improving_division_of_the_network(matrixStructure *matrix_structure, group *g, double *s, double Q_0) {
    int i, k, j, n, j_index, i_index, *indices, *unmoved, last_available_index;
    double *score, *improve, delta_Q, *d;

    n = g -> size;

    score = (double*) malloc(n * sizeof(double));
    improve = (double*) malloc(n * sizeof(double));
    indices = (int*) malloc(n * sizeof(int));
    unmoved = (int*) malloc(n * sizeof(int));
    d = (double*) malloc(sizeof(double) * n);
    if (score == NULL || improve == NULL || indices == NULL || unmoved == NULL || d == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    /* copy vector s to vector d */
    memcpy(d, s, n * sizeof(double));

    unmoved = allocate_unmoved(g, unmoved);
    last_available_index = n - 1; /* last available index in unmoved array */
    do {
        for (i = 0; i < n; i++) {

            /* Computing delta Q for the move of each unmoved vertex */
            for (k = 0; k <= last_available_index; k++) {
                d[k] = d[k] * (-1);
                score[k] = calc_score_i(matrix_structure, g, k, d);
                d[k] = d[k] * (-1);
            }

            j_index = find_max_index(score, n);
            d[j_index] =  d[j_index] * (-1);
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
            d[j] = d[j] * (-1);
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

    if (delta_Q > Q_0) {
        memcpy(s, d, n * sizeof(double));
    } else {
        delta_Q = Q_0;
    }
    free(d);
    return delta_Q;
}
