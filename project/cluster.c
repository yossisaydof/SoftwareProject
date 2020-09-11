#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrixStructure.h"
#include "divideIntoGroups.h"
#include "modularityGroups.h"


int calc_M(const int *K, int n) {
    int M = 0, i;
    for (i = 0; i < n; i++) {
        M += K[i];
    }
    return M;

}

void initialize_array_of_zeros(int *row, int n) {
    int i;
    for (i = 0; i < n; i++) {
        row[i] = 0;
    }
}

//  3 - 2:  1 2     1: 0 2      2: 0 1
/*      0 1 1
 *      1 0 1
 *      1 1 0
 */
matrixStructure* generate_matrix_structure(FILE *matrix_file) {
    int n, i, j, M, node_degree, node_id, *K, *matrix_row = NULL;
    matrixStructure *matrix_structure;
    spmat *spmat_matrix;

    assert((int)fread(&n, sizeof(int), 1, matrix_file) == 1);
    K = (int*) malloc(n * sizeof(int));
    for (i = 0; i < n; ++i) {
        assert(fread(&node_degree, sizeof(int), 1, matrix_file) == 1);
        K[i] = node_degree;
        assert(fseek(matrix_file, (long)(node_degree * sizeof(int)), SEEK_CUR) == 0);
    }

    M = calc_M(K, n); // M = nnz
    spmat_matrix = spmat_allocate_array(n, M);
    matrix_row = (int*) malloc(n * sizeof(int));

    assert(fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_SET) == 0);
    for (i = 0; i < n; i++) {
        initialize_array_of_zeros(matrix_row, n);
        node_degree = K[i];
        for (j = 0; j < node_degree; j++) {
            assert(fread(&node_id, sizeof(int), 1, matrix_file) == 1);
            matrix_row[node_id] = 1;
        }
        spmat_matrix -> add_row(spmat_matrix, matrix_row, i);
        assert(fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_CUR) == 0);
    }
    matrix_structure = allocate_matrix_structure(K, spmat_matrix, M, n);
    free(matrix_row);

    return matrix_structure;
}


int main(int argc, char* argv[]) {
    FILE *input_matrix_file, *output_file;
    char *input_matrix_filename, *output_filename;
    matrixStructure *matrix_structure;
    modularityGroups *modularity_groups;
    (void) argc;

    input_matrix_filename = argv[1];
    input_matrix_file = fopen(input_matrix_filename, "r");
    assert(input_matrix_file != NULL);

    matrix_structure = generate_matrix_structure(input_matrix_file);

    modularity_groups = divideIntoGroups(matrix_structure)

    output_filename = argv[2];
    output_file = fopen(output_filename, "w");
    assert(output_file != NULL);

    return 0;
}
