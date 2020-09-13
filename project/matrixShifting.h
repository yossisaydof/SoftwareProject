
/*
 * Calculates norm l1 of a matrix:
 *      norm_l1 = max_j(sum over i |matrix_ij|)
 *
 * Norm l1 is calculated to perform matrix shifting (to ensure the eigenpair we find is the leading eigenpair).
 */

#ifndef MATRIXSHIFTING_H
#define MATRIXSHIFTING_H

#include "matrixStructure.h"

double norm_l1(matrixStructure *matrix_structure);

#endif
