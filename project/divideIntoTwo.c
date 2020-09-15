#include "divideIntoTwo.h"

/**
 /Algorithm 2 - Divide a group into two
*/


/* TODO - notice this is the same function as in power iteration except for the int vs. double
 TODO - so - check if there are generic variables!
 * */

double calc_next_vector_i_2(matrixStructure *matrix, group *g, const int *curr_vector, int i) {

    /* Calculates: next_vextor[i] = norm * v_i + sum over j in g, j != i of (A_ij - k_i*k_j/M) * (v_j - j_i) */

    int j, A_ij, k_i, k_j, j_index, i_index, nnz_i, cnt_nnz = 0, row_start, row_end, *K, *nodes;
    double sum = 0, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    nodes = g -> nodes;
    i_index = nodes[i];
    k_i = K[i];
    M = matrix -> M;
    row_start = A -> rowptr[i_index];
    row_end = A -> rowptr[i_index + 1];
    nnz_i = row_end - row_start;

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        if (i_index == j_index) {
            sum += ((matrix -> norm_1) * curr_vector[i]); /* matrix shifting */
            continue;
        }
        A_ij = 0;
        k_j = K[j_index];
        if (cnt_nnz < nnz_i) {
            while (j_index > (A -> colind)[row_start + cnt_nnz]) {
                cnt_nnz++;
            }
            if (j_index == (A -> colind)[row_start + cnt_nnz]) {
                A_ij = (int) A -> values[row_start + cnt_nnz];
                cnt_nnz++;
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (curr_vector[j] - curr_vector[i]));
    }
    return sum;
}


void mult_matrix_vector_2(matrixStructure *matrix, group *g, int *curr_vector, double* next_vector) {

    /* Calculates B_hat[h] * curr_vector */
    double tmp;
    int i;

    for (i = 0; i < g -> size; i++) {
        tmp = calc_next_vector_i_2(matrix, g, curr_vector, i);
        next_vector[i] = tmp;
    }
}




double compute_delta_Q(matrixStructure *matrix_structure, group *g, int *s) {
    /*
     * deltaQ = s^t * B_hat[g] * s
     */
    int i;
    double delta_Q = 0, *mult_vector;

    mult_vector = (double*) malloc(sizeof(double) * g -> size);
    mult_matrix_vector_2(matrix_structure, g, s, mult_vector);

    for (i = 0; i < g -> size; ++i) {
        delta_Q += (s[i] * mult_vector[i]);
    }

    free(mult_vector);
    return (delta_Q * 0.5);
}


void divide_g(group *g, group *g1, group *g2, const int *s) {
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

void divide_into_two(matrixStructure *matrix_structure, group *g, group *g1, group *g2) {
    int n, i, cnt_positive = 0, cnt_negative = 0, *s;
    double eigen_value, deltaQ, *eigen_vector;

    n = g -> size;

    /* compute leading eigenpair of the modularity matrix B_hat_g */
    eigen_vector = (double*) malloc(sizeof(double) * n);
    if (eigen_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    eigen_value = power_iteration(matrix_structure, g, eigen_vector);

    if (IS_NON_POSITIVE(eigen_value)) {
        /* eigen value is non zero so the group g is indivisible */
        g1 -> size = 0;
        g2 -> size = 0;
        g1 -> next = NULL;
        g2 -> next = NULL;
        free(eigen_vector);
        return;
    }

    /* compute s = {s1,....,sn} where si in {+1, -1}, according to u1 */
    s = (int*) malloc(sizeof(int) * n);
    if (s == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

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
    deltaQ = compute_delta_Q(matrix_structure, g, s);
    if (eigen_value > EPSILON()) {
        /*deltaQ = improving_division_of_the_network(matrix_structure, g, s, deltaQ);*/
        cnt_negative = 0;
        cnt_positive = 0;
        for (i = 0; i < n; i++) {
            if (s[i] < 0)
                cnt_negative++;
            else
                cnt_positive++;
        }
    }

    if (IS_POSITIVE(deltaQ)) {
        /* */
        g1 -> size = cnt_positive;
        g1 -> nodes = (int*) malloc(sizeof(int) * cnt_positive);
        if (g1 -> nodes == NULL) {
            printf("%s", MALLOC_FAILED);
            exit(EXIT_FAILURE);
        }

        g2 -> size = cnt_negative;
        g2 -> nodes = (int*) malloc(sizeof(int) * cnt_negative);
        if (g2 -> nodes == NULL) {
            printf("%s", MALLOC_FAILED);
            exit(EXIT_FAILURE);
        }

        divide_g(g, g1, g2, s); /* divide g into two groups according to s */
    }
    else {
        /* if deltaQ <= 0 the group g is indivisible */
        g1 -> size = 0;
        g2 -> size = 0;
        g1 -> next = NULL;
        g2 -> next = NULL;
        free(s);
        free(eigen_vector);
        return;
    }

    free(s);
    free(eigen_vector);
}
