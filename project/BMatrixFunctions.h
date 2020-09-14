
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
void mult_f_v_double (double *f, double *v, double *result, int n);
void mult_f_v_int (double *f, int *v, double *result, int n);
void mult_A_g_double (spmat *A, double *v, group *g, double *result);
void mult_A_g_int (spmat *A, int *v, group *g, double *result);
void mult_kkM_double (matrixStructure *matrix_structure, group *g, double *v, double *result);
void mult_kkM_int (matrixStructure *matrix_structure, group *g, int *v, double *result);

#endif
