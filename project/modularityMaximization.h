/*
 * TODO - finish documentation
 * modularityMaximization Summary:
 * Implementation of algorithm 4 - Modularity Maximization
 *
 * In this module we implemented a method to further optimize a division of a sub-graph into two groups.
 * This module is use to optimize the division into two groups in  divideIntoTwo.c (algorithm 2).
 *
 * allocate_unmoved                     -
 * find_max_index                       -
 * remove_index_from_unmoved            -
 * calc_score_i                         -
 * improving_division_of_the_network    -
 *
 */

#ifndef MODULARITYMAXIMIZATION_H
#define MODULARITYMAXIMIZATION_H

#include "matrixStructure.h"
#include "group.h"
#include <stdio.h>
#include <string.h>

#define EPSILON() (0.00001)
#define IS_POSITIVE(X) ((X) > EPSILON())
#define IS_NON_POSITIVE(X) ((X) <= EPSILON())

//void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *g_arr, double *s);
void improving_division_of_the_network2(matrixStructure *matrix_structure, group *g, int *g_arr, double *s, double Q_0);


#endif
