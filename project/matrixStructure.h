
/*
 * matrixStructure Summary:
 *
 * This module holds all parameters to calculate matrix B:
 *      1. sparse matrix A = the graph is represented as adjacency matrix which we store as a sparse matrix.
 *      2. degreeList = array to hold the degree of each node.
 *      3. M = sum of all degrees.
 *      4. n = number of nodes.
 *      5. norm_1 = norm l1 of matrix B.
 *
 * allocate_matrix_structure    - Allocate matrix structure and updates all of its parameters except for norm_1.
 * free_matrix_structure        - Free matrix structure.
 * mult_Bg_vector               - Calculates a multiplication of B_hat[g] by a vector.
 *
 * There are few move functions that helps calculate mult_Bg_vector (all are explained in the module).
 */

#ifndef MATRIXSTRUCTURE_H
#define MATRIXSTRUCTURE_H

#include "spmatArray.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "group.h"

/* holds B matrix B_ij = A_ij - k_i*k_j/M */
typedef struct _matrixStructure {
    int *degreeList; /* k-Array */
    spmat *A; /* adjacency matrix of G */
    int M; /* sum of degreeList */
    int n; /* number of nodes, |V| */
    double norm_1;

    void	(*free)(struct _matrixStructure *matrix_structure);

} matrixStructure;

matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n);
void mult_Bg_vector(matrixStructure *matrix, group *g, int *g_arr, double* vector, double* next_vector, int norm_flag);

#endif
