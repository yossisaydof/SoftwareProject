#include "divideIntoGroups.h"


/**
 * Algorithm 3 - Divide a network into modularity groups
 */


modularityGroups* create_P(int n) {
    int i, *nodes;
    modularityGroups *P;

    P = allocate_modularity_group();
    nodes = malloc(n * sizeof(int));
    if (nodes == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++) {
        nodes[i] = i;
    }

    P -> insert(P, nodes, n);
    return P;
}

void divide_into_groups(matrixStructure *matrix_structure, modularityGroups *O) {
    modularityGroups *P;
    group *g, *g1, *g2;

    P = create_P(matrix_structure -> n);
    printf("%s\n", "creating P...");

    g1 = malloc(sizeof(group*));
    g2 = malloc(sizeof(group*));

    if (g1 == NULL || g2 == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    while (P -> number_of_groups > 0) {
        /* Remove a group g from P */
        printf("%s\n", "removing group g from P...");
        g = (group *) P -> remove(P);
        printf("%s%d\n", "Num of groups in P : " ,P->number_of_groups);
        /* Divide g into g1, g2 with Algorithm 2 */

        divide_into_two(matrix_structure, g, g1, g2);
        printf("%s%d\n", "g1 size " ,g1->size);
        printf("%s%d\n", "g2 size " ,g1->size);


        /* if either g1 or g2 is of size 0: Add g to O */
        if (g1 -> size == 0 || g2 -> size == 0) {
            printf("%s\n", "insert g to O ");
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
            printf("%s%d\n", "Num of groups in P : " ,P->number_of_groups);
            free(g1 -> nodes);
            free(g2 -> nodes);
        }
    }

    P -> free(P);
    free_group(g1);
    free_group(g2);
}

