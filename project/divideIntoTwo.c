#include "group.h"
#include "powerIteration.c"
#include "matrixStructure.c"

/**
 /Algorithm 2 - Divide a group into two
*/

void compute_f_g(matrixStructure *matrix_structure, group *g, double *f) {
    /* computes f_g: f_i_g = sum of B[g]_ij for all j in g */
    // TODO
}

double compute_delta_Q(matrixStructure *matrix_structure, double *f, int *s) {
    // TODO
}

void divide_g(group *g, group *g1,group *g2, int *s) {
    int i, g1_index = 0, g2_index = 0;

    for (i = 0; i < n; i++) {
        if (s[i] == 1) {
            g1[g1_index] = g[i];
            g1_index++;
        } else {
            g2[g2_index] = g[i];
            g2_index++;
        }
    }
}

void divideIntoTwo(matrixStructure *matrix_structure, group *g, group *g1, group *g2) {
    // TODO - check the note written after the algorithm in page 5
    int n, *s, i, cnt_positive = 0, cnt_negative = 0;
    double eigen_value, calc, *f, deltaQ, *eigen_vector;

    n = matrix_structure.n;

    // compute f_g
    compute_f_g(matrix_structure, g, f);

    // compute leading eigenpair of the modularity matrix B_hat_g
    eigen_vector = (double*) malloc(sizeof(double) * n);
    eigen_value = power_iteration(f, n, eigen_vector); // TODO: fix power iteration

    if (eigen_value <= 0)
        return;

    // compute s = {s1,....,sn} where si in {+1, -1}, according to u1
    s = (int*) malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) {
        if (eigen_vector[i] < 0) {
            s[i] = -1;
            cnt_negative++;
        } else {
            s[i] = 1;
            cnt_positive++;
        }
    }

    // compute deltaQ
    deltaQ = compute_delta_Q(matrix_structure, f, s);

    if (deltaQ > 0) {
        g1->size = cnt_positive;
        g1->nodes = malloc(sizeof(int) * cnt_positive);

        g2->size = cnt_negative;
        g2->nodes = malloc(sizeof(int) * cnt_negative);

        divide_g(g, g1, g2, s); // divide g into two groups according to s
    }
    // if deltaQ <= 0 the group g is indivisible

    free(s);
    free(eigen_vector);
}



