#include "divideIntoTwo.h"

/**
 /Algorithm 2 - Divide a group into two
*/

double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *g_arr, double *s) {
    /*
     * Computes deltaQ = s^t * B_hat[g] * s
     */
    int i;
    double delta_Q = 0, *mult_vector;

    mult_vector = (double*) malloc(g -> size * sizeof(double));

    /* computes B_hat[g] * s and store the result in mult_vector */
    mult_Bg_vector(matrix_structure, g, g_arr, s, mult_vector, 0);

    /* computes s^t * mult_vector */
    for (i = 0; i < g -> size; ++i) {
        delta_Q += (s[i] * mult_vector[i]);
    }

    free(mult_vector);
    return delta_Q;
}

void divide_g(group *g, group *g1, group *g2, const double *s) {
    /*
     * Divide group g according to s to 2 groups: g1, g2
     */
    int i, g1_index = 0, g2_index = 0;

    for (i = 0; i < g -> size; i++) {
        if (s[i] == 1) {
            (g1 -> nodes)[g1_index] = (g -> nodes)[i];
            g1_index++;
        } else {
            (g2 -> nodes)[g2_index] = (g -> nodes)[i];
            g2_index++;
        }
    }
    g1 -> next = NULL;
    g2 -> next = NULL;
}


void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2, int *g_arr) {
    /*
     * Implementation of algorithm 2 - divide g to 2 groups: g1, g2 if possible
     */
    int n, i, cnt_positive, cnt_negative;
    double eigen_value, deltaQ, *eigen_vector, *s;

    n = g -> size;

    /* compute leading eigenpair of the modularity matrix B_hat_g */
    eigen_vector = (double*) malloc(sizeof(double) * n);
    if (eigen_vector == NULL) ERROR_HANDLER(MALLOC_FAILED)

    eigen_value = power_iteration(matrix_structure, g, eigen_vector, g_arr);

    if (IS_NON_POSITIVE(eigen_value)) {
        /* eigen value is non zero so the group g is indivisible */
        reset_group(g1);
        reset_group(g2);
        free(eigen_vector);
        return;
    }

    /* compute s = {s1,....,sn} where si in {+1, -1}, according to u1 */
    s = (double*) malloc(sizeof(double) * n);
    if (s == NULL) ERROR_HANDLER(MALLOC_FAILED)

    cnt_negative = 0;
    cnt_positive = 0;
    for (i = 0; i < n; i++) {
        if (eigen_vector[i] < EPSILON()) {
            s[i] = -1;
            cnt_negative++;
        } else {
            s[i] = 1;
            cnt_positive++;
        }
    }

    /* compute deltaQ */
    deltaQ = compute_delta_Q(matrix_structure, g, g_arr, s);
    if (eigen_value > EPSILON()) {
        improving_division_of_the_network2(matrix_structure, g, g_arr, s, deltaQ);
    }

    if (IS_POSITIVE(deltaQ)) {
        /* delta_Q > EPSILON so group g is divisible */
        g1 -> size = cnt_positive;
        g1 -> nodes = (int*) malloc(sizeof(int) * cnt_positive);
        if (g1 -> nodes == NULL) ERROR_HANDLER(MALLOC_FAILED)

        g2 -> size = cnt_negative;
        g2 -> nodes = (int*) malloc(sizeof(int) * cnt_negative);
        if (g2 -> nodes == NULL) ERROR_HANDLER(MALLOC_FAILED)

        divide_g(g, g1, g2, s); /* divide g into two groups according to s */
    }
    else {
        /* if deltaQ <= EPSILON the group g is indivisible */
        reset_group(g1);
        reset_group(g2);
    }

    free(s);
    free(eigen_vector);
}
