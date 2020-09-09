#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "spmatArray.h"
#include "matrixStructure.h"
#include "group.h"

void create_random_vector(int n, double* randVector);
double calc_vector_magnitude(double *vector, int n);
double* calc_next_vector(spmat *matrix, double* currVector, int n, double *nextVector);
int check_diff(double *currVector, double *nextVector, int n);
double clac_eigenvalue(double *eigenVector, double *multVector, int n);
double power_iteration(matrixStructure *matrix_structure, group *g, double *eigenVector);
