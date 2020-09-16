#include "matrixStructure.h"


void free_matrix_structure(matrixStructure *matrix_structure) {
    free_array(matrix_structure -> A);
    free(matrix_structure -> degreeList);
    free(matrix_structure);
}


matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n) {
    matrixStructure* matrix_structure;

    matrix_structure = (matrixStructure*) malloc(sizeof(matrixStructure));
    if (matrix_structure == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    matrix_structure -> degreeList = K;
    matrix_structure -> A = spmat_matrix;
    matrix_structure -> M = M;
    matrix_structure -> n = n;
    matrix_structure -> norm_1 = 0;

    matrix_structure -> free = free_matrix_structure;

    return matrix_structure;
}


double mult_Bg_vector_i(matrixStructure *matrix, group *g, double *vector, int i, int norm_flag) {
    /*
     * Calculates: mult_vector[i] = sum over j in g, j != i of (A_ij - k_i*k_j/M) * (v_j - j_i)
     * if norm_flag == 1 then we add (norm * v_i) for i == j
     */

    int j, A_ij, k_i, k_j, j_index, i_index, nnz_i, cnt_nnz = 0, row_start, row_end, *K, *nodes;
    double sum = 0, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    nodes = g -> nodes;
    i_index = nodes[i];
    k_i = K[i_index];
    M = matrix -> M;
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];
    nnz_i = row_end - row_start;

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        if (i_index == j_index) {
            if (norm_flag == 1)
                sum += ((matrix -> norm_1) * vector[i]); /* matrix shifting */
            continue;
        }

        A_ij = 0;
        k_j = K[j_index];
        if (cnt_nnz < nnz_i) {
            while (((row_start + cnt_nnz) < M) && (j_index > (A -> colind)[row_start + cnt_nnz])) {
                cnt_nnz++;
                if (((row_start + cnt_nnz) < M)) break;
            }
            if ((row_start + cnt_nnz) < M) {
                if ((j_index == (A->colind)[row_start + cnt_nnz])) {
                    A_ij = (int) A->values[row_start + cnt_nnz];
                    cnt_nnz++;
                }
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (vector[j] - vector[i]));
    }
    return sum;
}


void mult_Bg_vector(matrixStructure *matrix, group *g, double* vector, double* next_vector, int norm_flag) {
    /* Calculates B_hat[g] * vector */
    double tmp;
    int i;

    for (i = 0; i < g -> size; i++) {
        tmp = mult_Bg_vector_i(matrix, g, vector, i, norm_flag);
        next_vector[i] = tmp;
    }
}

