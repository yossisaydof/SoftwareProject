#include "divideIntoGroups.h"

/**
 * Algorithm 3 - Divide a network into modularity groups
 */

modularityGroups* create_P(int n) {
    int i, *nodes;
    modularityGroups *P;

    P = allocate_modularity_group();
    nodes = (int*) malloc(n * sizeof(int));
    if (nodes == NULL) ERROR_HANDLER(MALLOC_FAILED);

    for (i = 0; i < n; i++) {
        nodes[i] = i;
    }

    P -> insert(P, nodes, n);
    free(nodes);
    return P;
}

void divide_into_groups(matrixStructure *matrix_structure, modularityGroups *O) {
    /*
     * Implementation of algorithm 3.
     * The function updates modularityGroup O to contain all modularity groups.
     */
    modularityGroups *P;
    group *g, *g1, *g2;
    double *eigen_vector;

    P = create_P(matrix_structure -> n);

    g1 = (group*) malloc(sizeof(group));
    g2 = (group*) malloc(sizeof(group));

    if (g1 == NULL || g2 == NULL) ERROR_HANDLER(MALLOC_FAILED);

    eigen_vector = (double*) malloc(sizeof(double) * matrix_structure -> n);
    if (eigen_vector == NULL) ERROR_HANDLER(MALLOC_FAILED)

    while (P -> number_of_groups > 0) {
        /* Remove a group g from P */
        g = (group *) P -> remove(P);

        /* Divide g into g1, g2 with Algorithm 2 */

        divide_into_two(matrix_structure, g, g1, g2, eigen_vector);

        /* if either g1 or g2 is of size 0: Add g to O */
        if (g1 -> size == 0 || g2 -> size == 0) {
            O -> insert(O, g -> nodes, g -> size);
        }
        else {

            if (g1 -> size == 1) {
                O -> insert(O, g1 -> nodes, g1 -> size);
            } else {
                P -> insert(P, g1 -> nodes, g1 -> size);
            }

            if (g2 -> size == 1) {
                O -> insert(O, g2 -> nodes, g2 -> size);
            } else {
                P -> insert(P, g2 -> nodes, g2 -> size);
            }
            free(g1 -> nodes);
            free(g2 -> nodes);
        }
        free_group(g);
    }

    P -> free(P);
    free_group(g1);
    free_group(g2);
    free(eigen_vector);
}

