#include "group.h"
#include "stdlib.h"
#include "divideIntoGroups.h"
#include "modularityGroups.h"
#include "divideIntoTwo.h"
#include "matrixStructure.h"


/**
 * Algorithm 3 - Divide a network into modularity groups
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

    g1 = malloc(sizeof(group*)); // TODO: Free memory
    g2 = malloc(sizeof(group*));

    while (P->number_of_groups > 0) {
        /* Remove a group g from P */
        g = (group *) P -> remove(P);

        /* Divide g into g1, g2 with Algorithm 2 */

        divideIntoTwo(matrix_structure, g, g1, g2);

        /* if either g1 or g2 is of size 0: Add g to O */
        if (g1->size == 0 || g2->size == 0) {
            O->insert(O, g->nodes, g->size);
        }
        else {
            if (g1->size == 1) {
                O->insert(O, g1->nodes, g1->size);
            } else {
                P->insert(P, g1->nodes, g1->size);
            }

            if (g2->size == 1) {
                O->insert(O, g2->nodes, g2->size);
            } else {
                P->insert(P, g2->nodes, g2->size);
            }
        }
    }

    free(g1);
    free(g2);
    return O;
}

