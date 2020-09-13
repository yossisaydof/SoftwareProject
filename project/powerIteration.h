
/*
 * PowerIteration module:
 * This module is used to calculate the leading eigen-pair in divideIntoTwo.c (algorithm 2).
 *
 * Finding the leading eigen-vector: The power iteration algorithm starts with a random vector b0, and in each iteration
 * uses the current vector to produce a new vector, used in the next iteration. When done, the vector produced in the
 * final iteration is the desired eigenvector.
 *
 *  Finding the leading eigen-value = (b_k * A*b_k) / (b_k * b_k)
 * */
#ifndef POWERITERATION_H
#define POWERITERATION_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "matrixStructure.h"
#include "group.h"

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigen_vector);

#endif
