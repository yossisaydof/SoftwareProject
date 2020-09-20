
/*
 * divideIntoGroups Summary:
 *
 * Implementation of algorithm 3 - Divide a network into modularity groups.
 *
 * create_P             - Creates a modularity group that contains a group with nodes {0,...,n-1}.
 * update_g_arr         - Updates an array of size n which contains non zero values in the indices corresponding to g.
 * divide_into_groups   - Implementation of algorithm 3.
 *
 */

#ifndef DIVIDEINTOGROUPS_H
#define DIVIDEINTOGROUPS_H

#include "divideIntoTwo.h"
#include "modularityGroups.h"

void divide_into_groups(matrixStructure *matrix_structure, modularityGroups *O);

#endif
