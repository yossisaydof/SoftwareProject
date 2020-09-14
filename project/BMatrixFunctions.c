#include "BMatrixFunctions.h"


double calc_f_i_g (matrixStructure *matrix_structure, group *g, int i) {
    int j, j_index, n, A_ij, k_i, k_j, *K, nnz_i, cnt_nnz = 0, row_start, row_end;
    double sum_row;
    spmat *A = matrix_structure -> A;

    K = matrix_structure -> degreeList;
    n = g -> size;
    k_i = K[i];

    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start; /* nnz elements in row i */

    for (j = 0; j < n; ++j) {
        j_index = g -> nodes[j]; /* We need to iterate over the indices in g -> nodes */
        k_j = K[j_index];

        A_ij = 0;
        if (cnt_nnz < nnz_i) {
            /* There are still nnz elements in row i in A */
            if (j_index == A -> colind[row_start + cnt_nnz]) {
                /* A_ij is nnz element */
                cnt_nnz++;
                A_ij = A -> values[row_start + cnt_nnz];
            }
        }

        sum_row += (double) (A_ij - ((double) k_i * k_j / matrix_structure -> M));
    }

    return sum_row;
}

void calc_f_g (matrixStructure *matrix_structure, group *g, double *f) {
    /*
     * Updates the vector f to be as the following:
     * f_g[i] = sum over j in g of (B[g]_ij) = sum over j in g of (A_ij - k_i*k_j / M)
     */
    int i, n;

    n = g -> size;

    for (i = 0; i < n; i++) {
        f[i] = calc_f_i_g (matrix_structure, g, g -> nodes[i]);
    }

}

void mult_f_v (double *f, double *v, double *result, int n) {
    /*
     * Multiples f_g * v and place it in result
     */
    int i;
    for (i = 0; i < n; i++) {
        result[i] = f[i] * v[i];
    }
}

void mult_A_g (spmat *A, double *v, group *g, double *result) {
    /*
     * Multiples A[g] * v and stores it in result
     */
    int i, j, *values, i_index, j_index, *colind, *rowptr, *g_nodes, cnt;
    double sum;

    g_nodes = g -> nodes;
    rowptr = (int*) A -> rowptr;
    values = (int*) A -> values;
    colind = (int*) A -> colind;

    for (i = 0; i < g -> size; i++) {
        i_index = g_nodes[i];
        sum = 0;
        cnt = 0;
        for (j = rowptr[i_index]; j < rowptr[i_index + 1]; j++) {
            /* iterating over nnz elements in row i_index */
            if (colind[j] == g_nodes[cnt]) {
                /* check if the column j is in g */
                sum += (double) (values[j] * v[colind[j]]);
                cnt++;
            }
        }
        result[i] = sum;
    }
}

void mult_kkM (matrixStructure *matrix_structure, group *g, double *v, double *result) {
    /*
     * result[i] = sum over j of ((k_i * k_j)/M * v[j])
     */
    int i, j, i_index, j_index, n, k_i, k_j, M, *g_nodes, *K;
    double tmp_sum = 0;

    n = g -> size;
    g_nodes = g -> nodes;
    M = matrix_structure -> M;
    K = matrix_structure -> degreeList;

    for (i = 0; i < n; i++) {
        i_index = g_nodes[i];
        k_i = K[i_index];
        for (j = 0; j < n; j++) {
            j_index = g_nodes[j];
            k_j = K[j_index];
            tmp_sum += ((double)(k_i * k_j) / M) * v[j];
        }
        result[i] = tmp_sum;
        tmp_sum = 0;
    }
}

double calc_norm_g (matrixStructure *matrix_structure, group *g) {
    return 0.0;
}