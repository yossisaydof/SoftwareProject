#include "group.h"
#include "powerIteration.h"
#include "matrixStructure.h"

double sum_of_row(matrixStructure *matrix_structure, group *g);
void compute_f_g(matrixStructure *matrix_structure, group *g, double *f);
double compute_delta_Q(matrixStructure *matrix_structure, double *f, group *g, int *s);
void divide_g(group *g, group *g1,group *g2, int *s);