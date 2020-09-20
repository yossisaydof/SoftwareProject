/*
 * modularityMaximization Summary:
 * Implementation of algorithm 4 - Modularity Maximization
 *
 * In this module we implemented a method to further optimize a division of a sub-graph into two groups.
 * This module is use to optimize the division into two groups in divideIntoTwo.c (algorithm 2).
 *
 * init_unmoved                         - Initialize unmoved array.
 * find_max_index_unmoved               - Return argMax{score[j]: j in unmoved}
 * find_max_index                       - Return argMax{improve[j]: j}
 * calc_sum_Ai                          - Calculates sum over j of A[g]_ij * s[j]
 * calc_sum_Ki                          - Calculates sum over j of (k_j * s_j)
 * update_score                         - Computing deltaQ for the move of each unmoved vertex.
 * improving_division_of_the_network    - An implementation of algorithm 4.
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


void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *g_arr, double *s) ;
/*void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, double *s);*/


#endif
