#include "matrixStructure.h"


void free_matrix_structure(matrixStructure *matrix_structure) {
    free_array(matrix_structure -> A);
    free(matrix_structure -> degreeList);
    free(matrix_structure);
}


matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n) {
    /*
     * Allocating a new matrixStructure, and reset its fields
     */
    matrixStructure* matrix_structure;

    matrix_structure = (matrixStructure*) malloc(sizeof(matrixStructure));
    if (matrix_structure == NULL) ERROR_HANDLER(MALLOC_FAILED)

    matrix_structure -> degreeList = K;
    matrix_structure -> A = spmat_matrix;
    matrix_structure -> M = M;
    matrix_structure -> n = n;
    matrix_structure -> norm_1 = 0;

    matrix_structure -> free = free_matrix_structure;

    return matrix_structure;
}



double calc_k_v_mult(matrixStructure *matrix, group *g, const double *vector) {
    /*
     * Calculates k_i * vector[i]
     */
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
    /*
     * Calculates K[g] * vector.
     *      K[g]_ij = k_i * k_j / M
     *      i and j according to g
     */
    int i, i_index, M, *K, *nodes;
    double tmp;

    M = matrix -> M;
    K = matrix -> degreeList;
    nodes = g -> nodes;
    tmp = calc_k_v_mult(matrix, g, vector);

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        result[i] -= ((tmp / M) * K[i_index]);
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


void mult_Ag_v(matrixStructure *matrix, group *g, const int *g_arr, const double* vector, double* result) {
    /*
     * Calculates A[g] * v
     */
    int i, i_index, j, j_index, row_start, row_end, *nodes, *colind;
    spmat *A;
    double sum;

    nodes = g -> nodes;
    A = matrix -> A;
    colind = A -> colind;

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        row_start = A -> rowptr[i_index];
        row_end = A -> rowptr[i_index + 1];
        sum = 0;
        for (j = row_start; j < row_end; j++) {
            j_index = colind[j];
            if (g_arr[j_index] != 0) {
                sum += (vector[g_arr[j_index] - 1]);
            }
        }
        result[i] = sum;
    }
}


void mult_Kg_f_vector(matrixStructure *matrix, group *g, const double* vector, double* result) {
    /*
     * Calculates: (sum over j in g of K[g]_ij) * vector[i]
     *      K[g]_ij = ((k_i * k_j) / M)   (according to indices in g)
     * Notice that each sum of row i is of the form: ((k_i/M) * (k_0 + ... + k_n))
     * So we calculate (k_0 + ... + k_n) only once and use it in every row.
     */
    int i, i_index, sum, *nodes, *K, M;

    sum = sum_k_g(matrix, g);
    nodes = g -> nodes;
    K = matrix -> degreeList;
    M = matrix -> M;

    for (i = 0; i < g -> size; i++) {
        i_index = nodes[i];
        result[i] += (double)(sum * vector[i] * K[i_index]) / M;
    }
}

double mult_Ag_i_f(matrixStructure *matrix, group *g, const int *g_arr, int i) {
    /*
     * Calculates sum of row i in A[g] (according to indices in g).
     */
    int i_index, j, j_index, row_start, row_end, *nodes, *colind;
    spmat *A;
    double sum;

    nodes = g -> nodes;
    A = matrix -> A;
    colind = A -> colind;

    i_index = nodes[i];
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];
    sum = 0;
    for (j = row_start; j < row_end; j++) {
        j_index = colind[j];
        if (g_arr[j_index] != 0) {
            sum += 1;
        }
    }
    return sum;
}

void mult_Ag_f(matrixStructure *matrix, group *g, int *g_arr, const double* vector, double *result) {
    /*
     * Calculates: (sum over j in g of A[g]_ij) * vector[i]
     */
    int i;
    double tmp;

    for (i = 0; i < g -> size; i++) {
        tmp = mult_Ag_i_f(matrix, g, g_arr, i);
        result[i] -= (tmp * vector[i]);
    }
}

void mult_f_g_v(matrixStructure *matrix, group *g, int *g_arr, double* vector, double* f_g) {
    /*
     * Calculates f_g and update f_g vector according to it.
     * Reminder: f_g is a diagonal matrix, so we hold its values in a vector.
     */
    mult_Ag_f(matrix, g, g_arr, vector, f_g);
    mult_Kg_f_vector(matrix, g, vector, f_g);

}

void add_norm(matrixStructure *matrix, const double* vector, double* next_vector, int n) {
    /*
     * Perform matrix shifting
     */
    int i;

    for (i = 0; i < n; i++) {
        next_vector[i] += (matrix -> norm_1 * vector[i]);
    }
}

void mult_Bg_vector(matrixStructure *matrix, group *g, int *g_arr, double* vector, double* next_vector, int norm_flag) {
    /*
     * Calculates B_hat[g] * vector
     */
    int total_n = matrix -> n;
    mult_Ag_v(matrix, g, g_arr, vector, next_vector);
    mult_kg_vector(matrix, g, vector, next_vector);

    if (g -> size != total_n) {
        mult_f_g_v(matrix, g, g_arr, vector, next_vector);
    }

    if (norm_flag == 1) {
        add_norm(matrix, vector, next_vector, g -> size);
    }
}
