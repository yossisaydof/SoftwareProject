#include "divideIntoTwo.h"

/**
 /Algorithm 2 - Divide a group into two
*/

double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *g_arr, double *s, double *f_g) {
    /*
     * Computes deltaQ = s^t * B_hat[g] * s
     *      We first calculate (B_hat[g] * s) and store the result in mult_vector.
     *      Then we calculate s^t * mult_vector.
     */
    int i;
    double delta_Q = 0, *mult_vector;

    mult_vector = (double*) malloc(g -> size * sizeof(double));

    /* computes B_hat[g] * s and store the result in mult_vector */
    mult_Bg_vector(matrix_structure, g, g_arr, s, mult_vector, f_g, 0);

    /* computes s^t * mult_vector */
    for (i = 0; i < g -> size; ++i) {
        delta_Q += (s[i] * mult_vector[i]);
    }

    free(mult_vector);
    return delta_Q;
}

void divide_g(int cnt_positive, int cnt_negative, group *g, group *g1, group *g2, const double *s) {
    /*
     * Divide group g according to s to 2 groups: g1, g2.
     */
    int i, g1_index = 0, g2_index = 0;

    g1 -> size = cnt_positive;
    g1 -> nodes = (int*) malloc(sizeof(int) * cnt_positive);
    if (g1 -> nodes == NULL) ERROR_HANDLER(MALLOC_FAILED)

    g2 -> size = cnt_negative;
    g2 -> nodes = (int*) malloc(sizeof(int) * cnt_negative);
    if (g2 -> nodes == NULL) ERROR_HANDLER(MALLOC_FAILED)


    for (i = 0; i < g -> size; i++) {
        if (s[i] == 1) {
            (g1 -> nodes)[g1_index] = (g -> nodes)[i];
            g1_index++;
        } else {
            (g2 -> nodes)[g2_index] = (g -> nodes)[i];
            g2_index++;
        }
    }
    g1 -> next = (group*) NULL;
    g2 -> next = (group*) NULL;
}

void algorithm4(matrixStructure *matrix_structure, group *g, int *g_arr, double *s, int *cnt_positive, int *cnt_negative) {
    int i;

    improving_division_of_the_network(matrix_structure, g, g_arr, s);
    *cnt_negative = 0;
    *cnt_positive = 0;
    for (i = 0; i < g -> size; i++) {
        if (s[i] < EPSILON()) {
            *cnt_negative = *cnt_negative + 1;
        }
        else {
            *cnt_positive = *cnt_positive + 1;
        }
    }
}

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2, int *g_arr) {
    /*
     * Implementation of algorithm 2 - divide g to 2 groups: g1, g2 if possible.
     */
    int n, i, cnt_positive, cnt_negative;
    double eigen_value, deltaQ, *eigen_vector, *s, *f_g;

    n = g -> size;

    /* Compute leading eigen-pair of the modularity matrix B_hat_g */
    eigen_vector = (double*) malloc(sizeof(double) * n);
    if (eigen_vector == NULL) ERROR_HANDLER(MALLOC_FAILED)

    f_g = (double*) malloc(sizeof(double) * n);
    if (f_g == NULL) ERROR_HANDLER(MALLOC_FAILED)

    /* Updates f_g */
    calc_vector_F(matrix_structure, g, g_arr, f_g);

    /* perform power iteration */
    eigen_value = power_iteration(matrix_structure, g, eigen_vector, g_arr, f_g);

    if (IS_NON_POSITIVE(eigen_value)) {
        /* eigen value is non zero so the group g is indivisible */
        reset_group(g1);
        reset_group(g2);
        free(eigen_vector);
        free(f_g);
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
    deltaQ = compute_delta_Q(matrix_structure, g, g_arr, s, f_g);
    if (IS_POSITIVE(eigen_value)) {
        /* Algorithm 4 - Modularity maximization */
        algorithm4(matrix_structure, g, g_arr, s, &cnt_positive, &cnt_negative);
    }

    if (IS_POSITIVE(deltaQ)) {
        /* delta_Q > EPSILON so group g is divisible */
        divide_g(cnt_positive, cnt_negative, g, g1, g2, s); /* divide g into two groups according to s */
    }
    else {
        /* if deltaQ <= EPSILON the group g is indivisible */
        reset_group(g1);
        reset_group(g2);
    }

    free(s);
    free(eigen_vector);
    free(f_g);
}
