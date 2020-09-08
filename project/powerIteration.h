#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "powerIteration.h"
#include "spmat.h"
#include "spmatArray.h"
#include "spmatList.h"

void create_random_vector(int n, double* randVector);
double calc_vector_magnitude(double *vector, int n);
double* calc_next_vector(spmatArray *matrix, double* currVector, int n, double *nextVector);
int check_diff(double *currVector, double *nextVector, int n);
double clac_eigenvalue(double *eigenVector, double *multVector, int n);
double power_iteration(spmat *matrix, int n, double *eigenVector, double eigenValue);
