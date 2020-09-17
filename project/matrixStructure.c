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



double calc_k_v_mult(matrixStructure *matrix, group *g, const double *vector) {
    int i, i_index, *K, *nodes;
    double sum = 0;
    K = matrix -> degreeList;
    nodes = g -> nodes;

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        sum += (K[i_index] * vector[i]);
    }
    return sum;
}


void mult_kg_vector(matrixStructure *matrix, group *g, double *vector, double *result) {
    int i, i_index, M, *K, *nodes;
    double tmp;

    M = matrix -> M;
    K = matrix -> degreeList;
    nodes = g -> nodes;
    tmp = calc_k_v_mult(matrix, g, vector);

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        result[i] = (tmp / M) * K[i_index];
    }
}

int sum_k_g(matrixStructure *matrix, group *g) {
    /*
     * Calculates (k_1 + ... + k_n) according to indices in g
     */
    int i, i_index, *nodes, *K;
    int sum = 0;

    nodes = g -> nodes;
    K = matrix -> degreeList;
    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        sum += K[i_index];
    }

    return sum;
}

void mult_Kg_vector(matrixStructure *matrix, group *g, const double* vector, double* result) {
    int i, i_index, sum, *nodes, *K;

    sum = sum_k_g(matrix, g);
    nodes = g -> nodes;
    K = matrix -> degreeList;

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        result[i] -= (sum * vector[i] * K[i_index]);
    }
}

void mult_f_g_v(matrixStructure *matrix, group *g, double* vector, double* f_g) {
    /*
     * Calculates f_g and update f_g vector according to it.
     * Reminder: f_g is a diagonal matrix, so we hold its values in a vector.
     */

    mult_Kg_vector(matrix, g, vector, f_g);
}

void mult_Ag_v(matrixStructure *matrix, group *g, int *g_arr, double* vector, double* result) {
    /*
     * Calculates A[g]*v
     */
    int i, i_index, j, j_index, row_start, row_end, *nodes, *colind, *values;
    spmat *A;
    double sum;

    nodes = g -> nodes;
    A = matrix -> A;
    colind = A -> colind;
    values = A -> values;

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        row_start = A -> rowptr[i_index];
        row_end = A -> rowptr[i_index + 1];
        sum = 0;
        for (j = row_start; j < row_end; j++) {
            j_index = colind[j];
            if (g_arr[j_index] == 1) {
                sum += (values[j] * vector[???]);
            }
        }
        result[i] = sum;
    }
}

void mult_Bg_vector(matrixStructure *matrix, group *g, int *g_arr, double* vector, double* next_vector, int norm_flag) {
    /* Calculates B_hat[g] * vector */
    double tmp;
    int i;

    mult_kg_vector(matrix, g, vector, next_vector);
    mult_f_g_v(matrix, g, vector, f_g); /* TODO - think if we want to create a new vector f_g */


}