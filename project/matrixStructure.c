

#include "spmatArray.h"


typedef struct matrixStructure {
    int *degreeList; // k-Array
    spmat *A; // adjacency matrix of G
    int const M; // sum of degreeList
} matrixStructure;



