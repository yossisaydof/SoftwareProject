
#include "matrixStructure.h"
#include "stdlib.h"

matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n) {
    matrixStructure* matrix_structure;

    matrix_structure = (matrixStructure*) malloc(sizeof(matrix_structure));
    matrix_structure -> degreeList = K;
    matrix_structure -> A = spmat_matrix;
    matrix_structure -> M = M;
    matrix_structure -> n = n;

    return matrix_structure;
}
