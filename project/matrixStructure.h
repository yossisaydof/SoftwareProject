
/* matrixStructure module
 * This module holds all parameters to calculate matrix B:
 *      1. sparse matrix A
 *      2. array to hold the degree of each node
 *      3. M = sum of all degrees
 *      4. n = number of nodes
 */

#ifndef _MATRIXSTRUCTURE_H
#define _MATRIXSTRUCTURE_H

#include "spmatArray.h"

/* holds B matrix B_ij = A_ij - k_i*k_j/M */
typedef struct _matrixStructure {
    int *degreeList; /* k-Array */
    spmat *A; /* adjacency matrix of G */
    int M; /* sum of degreeList */
    int n; /* number of nodes, |V| */
    double norm_1;
} matrixStructure;

matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n);
void free_matrix_structure(matrixStructure* matrix_structure);

#endif
