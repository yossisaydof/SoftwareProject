

#include "spmatArray.h"

// holds B matrix B_ij = A_ij - k_i*k_j/M
typedef struct matrixStructure {
    int *degreeList; // k-Array
    spmat *A; // adjacency matrix of G
    int const M; // sum of degreeList
    int const n; // number of nodes, |V|
} matrixStructure;
