
/*
 * PowerIteration module:
 * This module is used to calculate the leading eigen-pair in divideIntoTwo.c (algorithm 2).
 *
 * Finding the leading eigen-vector: The power iteration algorithm starts with a random vector b0, and in each iteration
 * uses the current vector to produce a new vector, used in the next iteration. When done, the vector produced in the
 * final iteration is the desired eigenvector.
 *
 *  Finding the leading eigen-value = (b_k * A*b_k) / (b_k * b_k)
 *
 *  create_random_vector    - Creates a random vector.
 *  calc_vector_magnitude   - Calculates vector magnitude.
 *  calc_next_vector        - Calculates the next vector according to the power iteration algorithm.
 *  check_diff              - Check if the difference between the current vector to the previous one is smaller/greater
 *                            than EPSILON.
 *  clac_eigenvalue         - Calculates an approximation of the dominant eigen value.
 *  power_iteration         - Implementation of power iteration algorithm.
 * */
#ifndef POWERITERATION_H
#define POWERITERATION_H

#include "matrixStructure.h"
#include "modularityGroups.h"

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigen_vector, int *g_arr, double *f_g);

#endif
