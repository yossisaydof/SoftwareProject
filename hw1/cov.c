#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void create_and_write_covariance_matrix(char *input_filename, char *output_filename);
void find_mean_and_subtract_it_from_each_row(double *row, int row_length);
double calculate_dot_product_of_two_rows(double *row1, double *row2, int row_length);

int main(int argc, char* argv[]) {
    char *input_filename, *output_filename;
    clock_t start, end;
    (void)argc;

    start = clock();

    input_filename = argv[1];
    output_filename = argv[2];

    create_and_write_covariance_matrix(input_filename, output_filename);

    end = clock();
    printf("Execution took %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));

    return 0;
}


void create_and_write_covariance_matrix(char *input_filename, char *output_filename) {
    FILE *input_file, *output_file;
    int matrix_dimension[2], output_matrix_dimension[2];
    int num_of_rows, num_of_columns, i, j, n;
    double *row_to_write;
    double **matrix;

    input_file = fopen(input_filename, "r");
    assert(input_file != NULL);
    rewind(input_file);

    fread(matrix_dimension, sizeof(int), 2, input_file);
    num_of_columns = matrix_dimension[0];
    num_of_rows = matrix_dimension[1];

    matrix = malloc(num_of_rows * sizeof(double));
    for (i = 0; i < num_of_rows; i++) {
        matrix[i] = malloc(num_of_columns * sizeof(double));
        n = fread(matrix[i], sizeof(double), num_of_columns, input_file);
        assert(n == num_of_columns);
    }

    for (i = 0; i < num_of_rows; i++) {
        find_mean_and_subtract_it_from_each_row(matrix[i], num_of_columns);
    }

    fclose(input_file);

    output_file = fopen(output_filename, "w");
    assert(output_file != NULL);

    output_matrix_dimension[0] = num_of_rows;
    output_matrix_dimension[1] = num_of_rows;

    row_to_write = malloc(num_of_rows * sizeof(double));
    fwrite(output_matrix_dimension, sizeof(int), 2, output_file);

    for (i = 0; i < num_of_rows; i++) {

        for (j = 0; j < num_of_rows; j++) {
            row_to_write[j] = calculate_dot_product_of_two_rows(matrix[i], matrix[j], num_of_columns);
        }
        n = fwrite(row_to_write ,sizeof(double), num_of_rows ,output_file);
        assert(n == num_of_rows);
    }

    fclose(output_file);
    free(matrix);
    free(row_to_write);

}

void find_mean_and_subtract_it_from_each_row(double *row, int row_length) {
    double sum, mean;
    int i;

    sum  = 0;
    for (i = 0; i < row_length; i++) {
        sum += row[i];
    }

    mean = (double) sum / row_length;
    for (i = 0; i < row_length; i++) {
        row[i] -= mean;
    }

}

double calculate_dot_product_of_two_rows(double *row1, double *row2, int row_length) {
    double result;
    int i;

    result = 0;
    for (i = 0; i < row_length; i++) {
        result += row1[i] * row2[i];
    }
    return result;
}
