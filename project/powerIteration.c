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
        sum_squares += (vector[i] * vector[i]);
    }

    return sqrt(sum_squares);
}


void calc_next_vector(matrixStructure *matrix_structure, group *g, double* curr_vector, int n, double *next_vector) {
    double denominator;
    int i;
    /* calculates numerator (i.e matrix_structure * curr_vector) */
    mult_Bg_vector(matrix_structure, g, curr_vector, next_vector, 1);

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
    mult_vector = (double*) malloc((g -> size) * sizeof(double));
    if (mult_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    /* calculates B_hat[g] * eigen_vector */
    mult_Bg_vector(matrix_structure, g, eigen_vector, mult_vector, 1);

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
    int n, cnt_diff = 0;

    n = g -> size;
    curr_vector = (double*) malloc(sizeof(double) * n);
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
        memcpy(curr_vector, eigen_vector, n * sizeof(double));
    }

    eigen_value = clac_eigenvalue(matrix_structure, g, eigen_vector);
    free(curr_vector);

    return (eigen_value - (matrix_structure -> norm_1));
}
