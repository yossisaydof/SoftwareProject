/*
 * Implementation of algorithm 4 - Modularity Maximization
 *
 * In this module we implemented a method to further optimize a division of a sub-graph into two groups.
 * This module is use to optimize the division into two groups in  divideIntoTwo.c (algorithm 2).
 *
 * The main idea of the algorithm: Suppose {g1, g2} is an initial division of g into two groups. To optimize the
 * division further:
 *      find a vertex v that, when moved the other group, will give the biggest increase in modularity of the
 *      complete network, or the smallest decrease if no increase is possible, and move it to the other group.
 *      Repeat this process with the constraint that each vertex may only be moved once, until all vertices have been moved.
 *      Once done, from all the states of division into two groups during the operation, find the state that has the
 *      maximal modularity, and start again from this state.
 *      We repeat the entire process iteratively until no further improvement is found.
 * */

#include "matrixStructure.h"
#include "group.h"

#define EPSILON() (0.00001)
#define IS_POSITIVE(X) ((X) > EPSILON())
#define IS_NON_POSITIVE(X) ((X) <= EPSILON())

void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *s, double Q_0);
