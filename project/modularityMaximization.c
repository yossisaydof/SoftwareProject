#include "modularityMaximization.h"

/**
 * Algorithm 4
 */
/*double EPSILON = 0.00001; */


/* TODO - delete from here*/
double calc_next_vector_i_3(matrixStructure *matrix, group *g, const int *curr_vector, int i) {

    /* Calculates: next_vextor[i] = norm * v_i + sum over j in g, j != i of (A_ij - k_i*k_j/M) * (v_j - j_i) */

    int j, A_ij, k_i, k_j, j_index, nnz_i, cnt_nnz = 0, row_start, row_end, *K, *nodes;
    double sum = 0, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    k_i = K[i];
    M = matrix -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start;
    nodes = g -> nodes;

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        if (i == j_index) {
            sum += ((matrix -> norm_1) * curr_vector[i]); /* matrix shifting */
            continue;
        }
        A_ij = 0;
        k_j = K[j_index];
        if (cnt_nnz < nnz_i) {
            while (j_index > (A->colind)[row_start + cnt_nnz]) {
                cnt_nnz++;
            }
            if (j_index == (A->colind)[row_start + cnt_nnz]) {
                A_ij = (int) A->values[row_start + cnt_nnz];
                cnt_nnz++;
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (curr_vector[j] - curr_vector[i]));
    }
    return sum;
}


void mult_matrix_vector_3(matrixStructure *matrix, group *g, int *curr_vector, double* next_vector) {

    /* Calculates B_hat[h] * curr_vector */
    double tmp;
    int i;

    for (i = 0; i < g -> size; i++) {
        tmp = calc_next_vector_i_3(matrix, g, curr_vector, g -> nodes[i]);
        next_vector[i] = tmp;
    }
}


double compute_delta_Q_2(matrixStructure *matrix_structure, group *g, int *s) {
    /*
     * deltaQ = s^t * B_hat[g] * s
     */
    int i;
    double delta_Q, *mult_vector;

    mult_vector = (double*) malloc(sizeof(double) * g -> size);
    mult_matrix_vector_3(matrix_structure, g, s, mult_vector);

    for (i = 0; i < g -> size; ++i) {
        delta_Q += s[i] * mult_vector[i];
    }
    return (delta_Q * 0.5);
}

/*TODO - delete until here */

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


double improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *s, double Q_0) {
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
                score[k] = (compute_delta_Q_2(matrix_structure, g, s)); /*TODO!!!!!!!!!!!! */
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

    return delta_Q - Q_0; /* TODO: Delete Q_0*/
}
