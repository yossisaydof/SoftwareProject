#include "matrixShifting.h"
#include "spmatArray.h"
#include "stdlib.h"

matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n) {
    matrixStructure* matrix_structure;

    matrix_structure = (matrixStructure*) malloc(sizeof(matrixStructure));
    matrix_structure -> degreeList = K;
    matrix_structure -> A = spmat_matrix;
    matrix_structure -> M = M;
    matrix_structure -> n = n;

    matrix_structure -> norm_1 = norm_l1(matrix_structure);

    return matrix_structure;
}


void free_matrix_structure(matrixStructure* matrix_structure) {

    free_array(matrix_structure -> A);
    free(matrix_structure -> degreeList);
}
