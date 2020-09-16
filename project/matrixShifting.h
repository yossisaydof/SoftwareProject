
/*
 * matrixShifting Summary:
 *
 * Calculates norm l1 of a matrix: norm_l1 = max_j(sum over i |matrix_ij|).
 * Norm l1 is calculated to perform matrix shifting (to ensure the eigenpair we find is the leading eigenpair).
 *
 * norm_l1      - Calculates norm1 of matrix B.
 * sum_of_row   - Calculates sum of row i in B.
 */

#ifndef MATRIXSHIFTING_H
#define MATRIXSHIFTING_H

#include "matrixStructure.h"

double norm_l1(matrixStructure *matrix_structure);

#endif
