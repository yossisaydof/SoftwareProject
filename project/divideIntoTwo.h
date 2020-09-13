
/*
 * Implementation of algorithm 2 - Divide a group into two
 * 1. Compute leading eigenpair u1 and β1 of the modularity matrix B_hat[g] (using power iteration)
 * 2. if (β1 ≤ 0):
 *      2.1. The group g is indivisible
 * 3. Compute s = {s1,...,sn} where si in {+1,-1}, according to u1
 * 4. if (s^T * B_hat[g] * s ≤ 0):
 *      4.1. The group g is indivisible
 * 5. return a division of g into two groups according to s
 */

#ifndef DIVIDEINTOTWO_H
#define DIVIDEINTOTWO_H

#include "modularityMaximization.h"
#include "powerIteration.h"


void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2);
void sum_row_i(matrixStructure *matrix_structure, group *g, int i, double *sum1, double *sum2, const int *s);
double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s);

#endif
