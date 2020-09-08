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
        randVector[i] = (double) rand();
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

double* calc_next_vector(matrixStructure matrix, double* currVector, int n, double *nextVector) {
    double denominator;
    int i;

    /* calculates numerator (i.e matrix * currVector) */
    nextVector = mult(matrix, currVector); /* TODO : check if the matrix should be A or B or B_hat */



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

double power_iteration(spmat *matrix, int n, double *eigenVector, double eigenValue) {
    /*
     * Approximates the dominant eigenpair
     * Stores the corresponding eigenvector in *eigenVector
     * Returns the largest eigenvalue in eigenValue
     */

    double *currVector;

    currVector = malloc(sizeof(double) * n);
    create_random_vector(n, currVector);

    while (1) {
        calc_next_vector(matrix, currVector, n, eigenVector);

        if (check_diff(currVector, eigenVector) == 1) {
            break;
        }
    }

    eigenValue = clac_eigenvalue(nextVector, Abk, n); // TODO - change Abk naming

    free(currVector);

    return eigenValue;
}