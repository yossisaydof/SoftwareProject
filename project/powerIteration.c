#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "powerIteration.h"
#include "spmatArray.h"

double EPSILON = 0.00001;

void create_random_vector(int n, double* randVector){
    /* Fill randVector with random values */
    int i;

    for (i = 0; i < n; i++) {
        randVector[i] = (double) rand(); // TODO - check how to start rand()
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
    /*
     * Calculates next_vector[i]
     * v = curr_vector so:
     * next_vector[i] = SUM over all j in g, j != i [(A_ij - k_i*k_j/M)*(v_j - v_i)]
     */

    int j = 0, A_ij, k_i, k_j, nnz_i, cnt_nnz = 0, row_start, row_end, *K;
    double sum = 0, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    k_i = K[i];
    M = matrix -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; /* number of non-zero elements in row i */

    for (j = 0; j < g -> size; j++) {
        if (i == j)
            continue;

        A_ij = 0;
        k_j = K[j];
        if (cnt_nnz < nnz_i) {
            if (j == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (curr_vector[j] - curr_vector[i]));
    }
    return sum;
}

void mult_matrix_vector(matrixStructure *matrix, group *g, double* curr_vector, double* next_vector) {
    /*
     * Calculates B_hat[h] * curr_vector
     */
    int i;

    for (i = 0; i < g -> size; i++) {
        next_vector[i] = calc_next_vector_i(matrix, g, curr_vector, i);
    }
}

void calc_next_vector(matrixStructure *matrix, group *g, double* curr_vector, int n, double *next_vector) {
    double denominator;
    int i;
    /* calculates numerator (i.e matrix * curr_vector) */
    mult_matrix_vector(matrix, g, curr_vector, next_vector); // TODO

    /* calculates denominator (i.e ||(matrix * curr_vector)||) */
    denominator = calc_vector_magnitude(next_vector, n);
    if (denominator == 0)
        exit(EXIT_FAILURE);

    /* update next_vector */
    for (i = 0; i < n; i++) {
        next_vector[i] = (double) (next_vector[i] / denominator);
    }
}

int check_diff(double *curr_vector, double *next_vector, int n) {
    /* Returns 1 if all differences are smaller than EPSILON */
    int i;
    for (i = 0; i < n; i++) {
        if (fabs(curr_vector[i] - next_vector[i]) >= EPSILON) {
            return 1;
        }
    }
    return 0;
}

double clac_eigenvalue(const double *curr_vector, const double *eigen_vector, int n) {
    /* Calculates an approximation of the corresponding dominant eigenvalue */
    // TODO - ask Dalit about the formula
    int i;
    double denominator = 0 , numerator = 0;

    for (i = 0; i < n ; i++) {
        denominator += (curr_vector[i] * curr_vector[i]);
        numerator += (curr_vector[i] * eigen_vector[i]); // TODO
    }
    if (denominator == 0)
        exit(EXIT_FAILURE);
    return numerator / denominator;
}

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigen_vector) {
    /*
     * Approximates the dominant eigenpair
     * Stores the corresponding eigenvector in *eigen_vector
     * Returns the largest eigenvalue in eigen_value
     */

    double *curr_vector, eigen_value;
    int n;

    n = g -> size;
    curr_vector = malloc(sizeof(double) * n);
    create_random_vector(n, curr_vector);

    while (1) { // TODO: make sure this is not an infinite loop!
        calc_next_vector(matrix_structure, g, curr_vector, n, eigen_vector);

        if (check_diff(curr_vector, eigen_vector, n) == 0) {
            /* the vector produced in the final iteration is the desired eigenvector */
            break;
        }
    }

    eigen_value = clac_eigenvalue(curr_vector, eigen_vector, n);

    free(curr_vector);

    return eigen_value;
}
