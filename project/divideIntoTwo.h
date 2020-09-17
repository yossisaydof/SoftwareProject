
/*
 * divideIntoTwo Summary:
 *
 * Implementation of algorithm 2 - Divide a group of nodes into two to maximise the modularity.
 *
 * compute_delta_Q          - Computes deltaQ.
 * divide_g                 - Divide a group g according to s.
 * divide_into_two          - Implementation of algorithm 2.
 *
 */

#ifndef DIVIDEINTOTWO_H
#define DIVIDEINTOTWO_H

#include "modularityMaximization.h"
#include "powerIteration.h"

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2, double *eigen_vector);

#endif
