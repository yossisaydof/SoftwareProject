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


void update_g_arr(int *g_arr, group *g) {
    int i, i_index, *nodes;

    nodes = g -> nodes;

    for (i = 0; i < g -> size; ++i) {
        i_index = nodes[i];
        g_arr[i_index] = i + 1;
    }
}

void divide_into_groups(matrixStructure *matrix_structure, modularityGroups *O) {
    /*
     * Implementation of algorithm 3.
     * The function updates modularityGroup O to contain all modularity groups.
     */
    int *g_arr;
    modularityGroups *P;
    group *g, *g1, *g2;

    P = create_P(matrix_structure -> n);

    g1 = (group*) malloc(sizeof(group));
    g2 = (group*) malloc(sizeof(group));

    if (g1 == NULL || g2 == NULL) ERROR_HANDLER(MALLOC_FAILED);

    g_arr = (int*) calloc(matrix_structure -> n, sizeof(int));


    while (P -> number_of_groups > 0) {
        /* Remove a group g from P */
        g = (group *) P -> remove(P);

        /* Updates g_arr according to g */
        update_g_arr(g_arr, g);

        /* Divide g into g1, g2 with Algorithm 2 */
        divide_into_two(matrix_structure, g, g1, g2, g_arr);

        /* Reset g_arr */
        memset(g_arr, 0, matrix_structure -> n * sizeof(int));

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
}

