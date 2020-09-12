#include "modularityMaximization.h"
#include "powerIteration.h"

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2);
void sum_row_i(matrixStructure *matrix_structure, group *g, int i, double *sum1, double *sum2, const int *s);
double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s);
