
/*
 * This module contain functions to help calculate all multiplications of B_hat matrix.
 *
 *
 *
 */

#ifndef BMATRIXFUNCTIONS_H
#define BMATRIXFUNCTIONS_H

#include "matrixStructure.h"
#include "group.h"

void calc_f_g (matrixStructure *matrix_structure, group *g, double *f);
void mult_f_v (double *f, double *v, double *result, int n);
void mult_A_g (spmat *A, double *v, group *g, double *result);
void mult_kkM (matrixStructure *matrix_structure, group *g, double *v, double *result);


#endif
