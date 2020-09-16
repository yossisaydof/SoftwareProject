
/*
 * divideIntoGroups Summary:
 *
 * Implementation of algorithm 3 - Divide a network into modularity groups.
 *
 * create_P             - Creates a modularity group that contains a group with nodes {0,...,n-1}.
 * divide_into_groups   - Implementation of algorithm 3.
 *
 */

#ifndef DIVIDEINTOGROUPS_H
#define DIVIDEINTOGROUPS_H

#include "divideIntoTwo.h"
#include "modularityGroups.h"

void divide_into_groups(matrixStructure *matrix_structure, modularityGroups *O);

#endif
