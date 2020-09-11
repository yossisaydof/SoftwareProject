#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "powerIteration.h"
#include "spmatArray.h"
#include "matrixStructure.h"

double EPSILON = 0.00001;

void create_random_vector(int n, double* randVector){
    /* Fill randVector with random values */
    int i;

    for (i = 0; i < n; i++) {
        randVector[i] = (double) rand(); // TODO - check how to start rand()
    }
}

double calc_vector_magnitude(double *vector, int n) {
    /* Calculates the vector's magnitude (i.e. square root of the vector's dot product with itself) */
    double sum_squares = 0;
    int i;

    for (i = 0; i < n; i++){
        sum_squares += vector[i] * vector[i];
    }

    return sqrt(sum_squares);
}

double calc_next_vector_i(matrixStructure *matrix, group *g, double *currVector, int i) {
    /*
     * Calculates next_vector[i]
     * v = curr_vector so:
     * next_vector[i] = SUM over all j in g, j != i [(A_ij - k_i*k_j/M)*(v_j - v_i)]
     */

    int j = 0, A_ij, k_i, k_j, nnz_i, cnt_nnz = 0, row_start, row_end;
    double sum, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    k_i = K[i];
    M = matrix -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; /* number of non-zero elements in row i */

    for (j = 0; j < g -> size; j++) {
        if (i == j) continue;
        A_ij = 0;
        k_j = K[j];
        if (cnt_nnz < nnz_i) {
            if (j == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        sum += ((A_ij - ((k_i * k_j) / M)) * (currVector[j] - currVector[i]));
    }
    return sum;
}

void mult_matrix_vector(matrixStructure *matrix, group *g, double* currVector, double* nextVector) {
    /*
     * Calculates B_hat[h] * curr_vector
     */
    int i;

    for (i = 0; i < g -> size; i++) {
        nextVector[i] = calc_next_vector_i(matrix, g, currVector, i);
    }
}

double* calc_next_vector(matrixStructure *matrix, group *g, double* currVector, int n, double *nextVector) {
    double denominator;
    int i;
    // TODO - change naming of all elements from camelCase to the other one
    /* calculates numerator (i.e matrix * currVector) */
    mult_matrix_vector(matrix, g, currVector, nextVector); // TODO

    /* calculates denominator (i.e ||(matrix * currVector)||) */
    denominator = calc_vector_magnitude(nextVector, n);

    /* update nextVector */
    for (i = 0; i < n; i++) {
        nextVector[i] = (double) nextVector[i] / denominator;
    }

}

int check_diff(double *currVector, double *nextVector, int n) {
    /* Returns 1 if all differences are smaller than EPSILON */
    int i;
    for (i = 0; i < n; i++) {
        if (fabs(currVector[i] - nextVector[i]) >= EPSILON) {
            return 0;
        }
    }
    return 1;
}

double clac_eigenvalue(double *eigenVector, double *Abk, int n) {
    /* Calculates an approximation of the corresponding dominant eigenvalue */
    // TODO - change Abk naming
    int i;
    double denominator, numerator;
    for (i = 0; i < n ; i++) {
        denominator += eigenVector[i] * eigenVector[i];
        numerator += eigenVector[i] * Abk[i];
    }

    return numerator / denominator;
}

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigenVector) {
    /*
     * Approximates the dominant eigenpair
     * Stores the corresponding eigenvector in *eigenVector
     * Returns the largest eigenvalue in eigenValue
     */

    double *currVector;
    int n;

    n = g -> size;
    currVector = malloc(sizeof(double) * n);
    create_random_vector(n, currVector);

    while (1) { // TODO: make sure this is not an infinite loop!
        calc_next_vector(matrix, g, currVector, n, eigenVector);

        if (check_diff(currVector, eigenVector) == 1) {
            /* the vector produced in the final iteration is the desired eigenvector */
            break;
        }
    }

    eigenValue = clac_eigenvalue(nextVector, Abk, n); // TODO - change Abk naming

    free(currVector); // TODO - do we need to free each element in the vector?

    return eigenValue;
}