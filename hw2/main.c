#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "spmat.h"

void create_random_vector(double *vector, int n);


int main(int argc, char* argv[]){
    FILE *input_matrix_file, *initial_vector_file;
    char *input_matrix_filename, *initial_vector_filename ,*output_filename, *implementation = NULL;
    double *initial_vector, *matrix_row;
    int n, i, matrix_dimension[2];
    spmat* sparse_matrix;

    /* TESTING */
//    double row1[] = {1, 0, 2}, row2[] = {0, 0, 7};
//    sparse_matrix = spmat_allocate_list(3);
//    sparse_matrix->add_row(sparse_matrix, (const double *) &row1, 0);
//    sparse_matrix->add_row(sparse_matrix, (const double *) &row2, 1);
//    sparse_matrix->print(sparse_matrix, 3);
//
//    spmat* sp;
    /* TESTING */

    /* Read input matrix filename and matrix dimension */
    input_matrix_filename = argv[1];
    input_matrix_file = fopen(input_matrix_filename, "r");
    assert(input_matrix_file != NULL);

    fread(matrix_dimension, sizeof(int), 2, input_matrix_file);
    assert(matrix_dimension[0] == matrix_dimension[1]);
    n = matrix_dimension[0];

    initial_vector = malloc(n * sizeof(double));

    if (argc == 4) {
        /* In case that there is no initial vector input, so we create it */
        output_filename = argv[2];
        implementation = argv[3];
        create_random_vector(initial_vector, n);

    }
    else if (argc == 5) {
        /* In case that there is initial vector input, so we read it from file */
        initial_vector_filename  = argv[2];
        output_filename = argv[3];
        implementation = argv[4];

        initial_vector_file = fopen(initial_vector_filename, "r");
        assert(initial_vector_file != NULL);
        fread(initial_vector, sizeof(int), n, initial_vector_file);
    }
    else return -1;


    /* Spmat allocate according to implementation type */
    if (strcmp(implementation, "-list") == 0){
        sparse_matrix = spmat_allocate_list(n);
    }
    else if (strcmp(implementation, "-array") == 0){
        sparse_matrix = spmat_allocate_array(n,1);
    }
    else
        return -1;

    /* Read sparse matrix from input file */
    for (i = 0; i < n; i++) {
        matrix_row = malloc(sizeof(double) * n);
        fread(matrix_row, sizeof(double), n, input_matrix_file);
        sparse_matrix->add_row(sparse_matrix, matrix_row, i);
    }

    /* Free all memory in the code */
    sparse_matrix->free(sparse_matrix);
    free(initial_vector);

    return argc;
}



void create_random_vector(double *vector, int n){
    /* Update vector to random numbers */

    int i;
    for (i = 0; i < n; i++){
        vector[i] = (double)rand();
    }

}

