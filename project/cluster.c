#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char* argv[]) {
    FILE *input_matrix_file, *output_file;
    char *input_matrix_filename, *output_filename;
    MatrixStructure *matrix_structure;
    SpmatMatrix *spmat_matrix;
    (void) argc;


    input_matrix_filename = argv[1];
    input_matrix_file = fopen(input_matrix_filename, "r");
    assert(input_matrix_file != NULL);




    output_filename = argv[2];
    output_file = fopen(output_filename, "w");
    assert(output_file != NULL);

    return 0;
}

void generate_adjacency_matrix(FILE *matrix_file, SpmatMatrix *spmat_matrix) {
    int n, nnz, i;

    // n = number of nodes in the graph, |V|
    nnz = M * 2; // sum of the edges degree, sum of the k array (for undirected graph 'cause each edge give 1 in the matrix
    spmat_matrix = spmat_allocate_array(n , nnz);

    for (i = 0; i < n; i++) {
        //matrix row[i] = all nodes that have edge with node i
        spmat_matrix->add_row(spmat_matrix, matrix_row, i);
    }

}
