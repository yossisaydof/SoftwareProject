#include "stdlib.h"
#include "divideIntoGroups.h"
#include "modularityGroups.h"
#include "divideIntoTwo.h"


/**
 * Algorithm 3
 */


modularityGroups* create_P(int n) {
    int i, *nodes;
    modularityGroups *P;

    P = allocate_modularity_group();
    nodes = malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        nodes[i] = i;
    }

    P->insert(P, nodes, n);
    return P;
}

modularityGroups* divideIntoGroups(matrixStructure *matrix_structure) {
    modularityGroups *O, *P;
    group *g, *g1, *g2;

    P = create_P(matrix_structure -> n);

    O = allocate_modularity_group();

    while (P->number_of_groups > 0) {
        g = (group *) P -> remove(P); //Remove a group g from P

        //Divide g into g1, g2 with Algorithm 2
        divideIntoTwo(g, g1, g2);
    }


}

