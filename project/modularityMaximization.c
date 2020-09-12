#include <stdlib.h>
#include "modularityMaximization.h"
#include "divideIntoTwo.h"

#define EPSILON() (0.00001)
#define IS_POSITIVE(X) ((X) > EPSILON())

/**
 * Algorithm 4
 */
/*double EPSILON = 0.00001; */

int* allocate_unmoved(group *g, int *unmoved) {
    int i;

    for (i = 0; i < g -> size; i++) {
        unmoved[i] = i;
    }
    return unmoved;
}

int find_max_index(const double *score, int n) {
    int i, max_index;
    double max, tmp_max;

    max = score[0];
    max_index = 0;
    for (i = 1; i < n; i++) {
        tmp_max = score[i];
        if (tmp_max > max) {
            max = tmp_max;
            max_index = i;
        }
    }
    return max_index;
}

void remove_index_from_unmoved(int *unmoved, int max_index, int last_available_index) {

    unmoved[max_index] = unmoved[last_available_index];
    unmoved[last_available_index] = -1;
}


void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *s, double Q_0) {
    int i, k, j, n, j_index, i_index, *indices, *unmoved, last_available_index;
    double *score, *improve, delta_Q;

    n = g -> size;
    score = (double*) malloc(n * sizeof(double));
    improve = (double*) malloc(n * sizeof(double));
    indices = (int*) malloc(n * sizeof(int));
    unmoved = (int*) malloc(n * sizeof(int));

    unmoved = allocate_unmoved(g, unmoved);
    last_available_index = n - 1; /* last available index in unmoved array */
    do {
        for (i = 0; i < n; i++) {

            /* Computing delta Q for the move of each unmoved vertex */
            for (k = 0; k < last_available_index; k++) {
                s[k] = s[k] * (-1);
                score[k] = compute_delta_Q(matrix_structure, g, s) - Q_0;
                s[k] = s[k] * (-1);
            }

            j_index = find_max_index(score, n);
            s[j_index] =  s[j_index] * (-1);
            indices[i] = j_index;

            if (i == 0)
                improve[i] = score[j_index];
            else
                improve[i] = improve[i - 1] + score[j_index];

            remove_index_from_unmoved(unmoved, j_index, last_available_index);
            last_available_index--;
        }

        /* Find the maximum improvement of s and update s accordingly */
        i_index = find_max_index(improve, n);
        for (i = n - 1; i > i_index; i--) {
            j = indices[i];
            s[j] = s[j] * (-1);
        }

        if (i_index == n - 1)
            delta_Q = EPSILON();
        else
            delta_Q = improve[i_index];

    } while (IS_POSITIVE(delta_Q));

    free(score);
    free(improve);
    free(indices);
    free(unmoved);
}
