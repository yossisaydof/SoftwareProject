#include "powerIteration.h"

double EPSILON = 0.00001;

void create_random_vector(int n, double *randVector){
    /* Fill randVector with random values */
    int i;

    for (i = 0; i < n; i++) {
        randVector[i] = (double) rand();
    }
}

double calc_vector_magnitude(const double *vector, int n) {
    /* Calculates the vector's magnitude (i.e. square root of the vector's dot product with itself) */
    double sum_squares = 0;
    int i;

    for (i = 0; i < n; i++){
        sum_squares += vector[i] * vector[i];
    }

    return sqrt(sum_squares);
}


double calc_next_vector_i(matrixStructure *matrix, group *g, const double *curr_vector, int i) {

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
            if (j_index == A -> colind[row_start + cnt_nnz]) {
                A_ij = (int) A -> values[row_start + cnt_nnz];
                cnt_nnz++;
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (curr_vector[j] - curr_vector[i]));
    }
    return sum;
}


void mult_matrix_vector(matrixStructure *matrix, group *g, double* curr_vector, double* next_vector) {

     /* Calculates B_hat[h] * curr_vector */

    int i;

    for (i = 0; i < g -> size; i++) {
        next_vector[i] = calc_next_vector_i(matrix, g, curr_vector, g -> nodes[i]);
    }
}

void calc_next_vector(matrixStructure *matrix_structure, group *g, double* curr_vector, int n, double *next_vector) {
    double denominator;
    int i;
    /* calculates numerator (i.e matrix_structure * curr_vector) */
    mult_matrix_vector(matrix_structure, g, curr_vector, next_vector);

    /* calculates denominator (i.e ||(matrix_structure * curr_vector)||) */
    denominator = calc_vector_magnitude(next_vector, n);
    if (denominator == 0) {
        printf("%s", DIVIDE_BY_ZERO);
        exit(EXIT_FAILURE);
    }

    /* update next_vector */
    for (i = 0; i < n; i++) {
        next_vector[i] = (double) (next_vector[i] / denominator);
    }
}

int check_diff(double *curr_vector, double *next_vector, int n) {
    /* Returns 1 if all differences are smaller than EPSILON */
    int i;
    double diff;
    for (i = 0; i < n; i++) {
        diff = fabs(curr_vector[i] - next_vector[i]);
        if (diff > EPSILON) {
            return 1;
        }
    }
    return 0;
}

double clac_eigenvalue(matrixStructure *matrix_structure, group *g, double *eigen_vector) {
    /*
     * Calculates eigen value.
     * bk is our approximation of the dominant eigenvector, an approximation of the corresponding
     * dominant eigenvalue can be found as follows:
     *      eigen_value = (b_k * (A*b_k)) / (b_k * b_k)
     */
    int i;
    double *mult_vector, denominator = 0, numerator = 0;
    mult_vector = malloc((g -> size) * sizeof(double));
    if (mult_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    /* calculates B_hat[g] * eigen_vector */
    mult_matrix_vector(matrix_structure, g, eigen_vector, mult_vector);

    for (i = 0; i < g -> size; i++) {
        numerator += (mult_vector[i] * eigen_vector[i]);
        denominator += (eigen_vector[i] * eigen_vector[i]);
    }
    if (denominator == 0) exit(EXIT_FAILURE);

    free(mult_vector);

    return (numerator / denominator);
}

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigen_vector) {
    /*
     * Approximates the dominant eigenpair
     * Stores the corresponding eigenvector in eigen_vector
     * Returns the largest eigenvalue
     */

    double *curr_vector, eigen_value;
    int n, cnt_diff = 0, i;

    n = g -> size;
    curr_vector = malloc(sizeof(double) * n);
    if (curr_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    create_random_vector(n, curr_vector);

    while (1) { /* TODO: make sure this is not an infinite loop! */
        calc_next_vector(matrix_structure, g, curr_vector, n, eigen_vector);
        cnt_diff++;
        if (check_diff(curr_vector, eigen_vector, n) == 0) {
            /* the vector produced in the final iteration is the desired eigenvector */
            break;
        }
        for (i = 0; i < n; i++) {
            curr_vector[i] = eigen_vector[i];
        }
    }

    eigen_value = clac_eigenvalue(matrix_structure, g, eigen_vector);
    free(curr_vector);

    return (eigen_value - matrix_structure -> norm_1);
}
