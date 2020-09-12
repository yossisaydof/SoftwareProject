
/* Implementation of algorithm 3 - Divide a network into modularity groups:
 *
 * 1. Start with a trivial division into one group: P <- {{0,...,n-1}}
 * 2. Start with an empty output set of groups: O <- {}
 * 3. Repeat until P is empty:
 *      3.1. Remove a group g from P
 *      3.2. Divide g into g1, g2 with Algorithm 2
 *      3.3. if either g1 or g2 is of size 0:
 *          3.3.1. Add g to O
 *      3.4. else:
 *          3.4.1. Add to O: any group (g1 and/or g2) of size 1
 *          3.4.2. Add to P: any group (g1 and/or g2) larger than 1
 * 4. Output the division given by O
 */

#include "stdlib.h"
#include "divideIntoTwo.h"
#include "modularityGroups.h"

modularityGroups* divide_into_groups(matrixStructure *matrix_structure);
