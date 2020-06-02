#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "spmat.h"

void create_random_vector(double *vector, int n);
int calc_nnz_values(FILE *input_matrix_file, int n);


int main(int argc, char* argv[]){
    FILE *input_matrix_file, *initial_vector_file, *output_file;
    char *input_matrix_filename, *initial_vector_filename ,*output_filename, *implementation = NULL;
    double *initial_vector, *matrix_row, *result_vector;
    int n, i, nnz, num, matrix_dimension[2], vector_dimension[2];
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

    num = (int) fread(matrix_dimension, sizeof(int), 2, input_matrix_file);
    assert(num == 2);
    assert(matrix_dimension[0] == matrix_dimension[1]);
    n = matrix_dimension[0];
    rewind(input_matrix_file);

    initial_vector = (double*) malloc(n * sizeof(double));

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
        num = (int) fread(vector_dimension, sizeof(int), 2, initial_vector_file);
        assert(num == 2);

        num = (int) fread(initial_vector, sizeof(double), n, initial_vector_file);
        assert(num == n);
    }
    else return -1;


    /* Spmat allocate according to implementation type */
    if (strcmp(implementation, "-list") == 0){
        sparse_matrix = spmat_allocate_list(n);
    }
    else if (strcmp(implementation, "-array") == 0){
        num = (int) fread(matrix_dimension, sizeof(int), 2, input_matrix_file);
        assert(num == 2);
        assert(matrix_dimension[0] == matrix_dimension[1]);

        n = matrix_dimension[0];
        nnz = calc_nnz_values(input_matrix_file, n);
        sparse_matrix = spmat_allocate_array(n, nnz);
    }
    else
        return -1;

    /* Read sparse matrix from input file */
    matrix_row = (double*) malloc(sizeof(double) * n);
    for (i = 0; i < n; i++) {
        num = (int) fread(matrix_row, sizeof(double), n, input_matrix_file);
        assert(num == n);
        sparse_matrix->add_row(sparse_matrix, matrix_row, i);
    }

    /* Multiplies sparse matrix by initial vector */
    result_vector = (double*) malloc(n * sizeof(double));
    /* Add power iteration here */
    sparse_matrix->mult(sparse_matrix, initial_vector, result_vector);

    /* Write the produced result vector to the output file */
    output_file = fopen(output_filename, "w");
    assert(output_file != NULL);

    num = (int)fwrite(vector_dimension ,sizeof(int), 2 ,output_file);
    assert(num == 2);

    num = (int) fwrite(result_vector, sizeof(double), n, output_file);
    assert(num == n);

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


int calc_nnz_values(FILE *input_matrix_file, int n){
    /*
     * Calculate non-zero values for sparse matrix
     */
    double *row;
    int i, j, nnz, num;
    int matrix_dim[2];
    nnz = 0;
    row = malloc(n * sizeof(double));

    /* Read first 2 numbers from file */
    num = (int) fread(matrix_dim, sizeof(int), 2, input_matrix_file);
    assert(num == 2);
    for (i = 0; i < n; ++i) {
        /* Iterating over the martix's rows */
        num = (int) fread(row, sizeof(double), n, input_matrix_file);
        assert(num == n);
        for (j = 0; j < n; ++j) {
            if (row[j] == (double) 0)
                nnz += 1;
        }
    }

    rewind(input_matrix_file);
    free(row);
    return nnz;
}

