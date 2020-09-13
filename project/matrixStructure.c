#include "matrixStructure.h"


void free_matrix_structure(matrixStructure *matrix_structure) {

    free_array(matrix_structure -> A);
    free(matrix_structure -> degreeList);
}


matrixStructure* allocate_matrix_structure(int *K, spmat *spmat_matrix, int M, int n) {
    matrixStructure* matrix_structure;

    matrix_structure = (matrixStructure*) malloc(sizeof(matrixStructure));
    if (matrix_structure == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    matrix_structure -> degreeList = K;
    matrix_structure -> A = spmat_matrix;
    matrix_structure -> M = M;
    matrix_structure -> n = n;

    matrix_structure -> free = free_matrix_structure;

    return matrix_structure;
}



