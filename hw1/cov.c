#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

double find_mean_of_row();
void read_matrix_to_memory(FILE *file, double **matrix, int num_of_rows, int num_of_column);
void standardize_matrix(double **matrix_input, int num_of_rows, int num_of_columns);
void subtract_mean_from_row();
void print_matrix();
double **generate2DArray();
void create_the_covariance_matrix();
double cal_dot_product_of_rows();

int main(int argc, char** argv) {
    FILE *file;
    char *input_filename, *output_filename;
    int num_of_rows, num_of_columns;
    double **matrix;
    (void)argc;

    input_filename = argv[1];
    output_filename = argv[2];

    file = fopen(input_filename, "r");
    assert(file != NULL);
    num_of_rows = 0, num_of_columns = 0;
    fscanf(file, "%d", &num_of_columns);
    fscanf(file, "%d", &num_of_rows);

    matrix = generate2DArray(num_of_rows, num_of_columns);

    read_matrix_to_memory(file, matrix, num_of_rows, num_of_columns);

    fclose(file);

    standardize_matrix(matrix, num_of_rows, num_of_columns);

    create_the_covariance_matrix(matrix, output_filename, num_of_rows, num_of_columns);

    free(matrix);
    return 0;
}


void read_matrix_to_memory(FILE *file, double **matrix, int num_of_rows, int num_of_column) {
    int i, j, n;
    for (i = 0; i < num_of_rows; i++) {
        for (j = 0; j < num_of_column; j++) {
            n = fscanf(file, "%lf", &matrix[i][j]);
            assert(n == 1);
        }
    }
}


void standardize_matrix(double **matrix_input, int num_of_rows, int num_of_columns) {
    double mean;
    int i;
    for (i = 0; i < num_of_rows; i++) {
        mean = find_mean_of_row(matrix_input[i], num_of_columns);
        subtract_mean_from_row(matrix_input[i], mean, num_of_columns);
    }
}


double find_mean_of_row(double *row, int n) {
    double sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + row[i];
    }
    return sum / n;
}


void subtract_mean_from_row(double *row, double mean, int n) {
    int i;
    for (i = 0; i < n; i++) {
        row[i] = row[i] - mean;
    }
}


void create_the_covariance_matrix(double **input_matrix, char *output_filename, int num_of_rows, int num_of_columns) {
    FILE *file;
    double *row_to_write;
    double dot_product_of_rows;
    int i, j, n;

    file = fopen(output_filename, "w");

    for (i = 0; i < num_of_rows; i++) {
        row_to_write = malloc(num_of_rows * sizeof(double));
        for (j = 0; j < num_of_rows; j++) {
            dot_product_of_rows = cal_dot_product_of_rows(input_matrix[i], input_matrix[j], num_of_columns);
            row_to_write[j] = dot_product_of_rows;
        }
        n = fwrite(row_to_write, sizeof(double), num_of_rows, file);
        free(row_to_write);
        assert(n == num_of_rows);
    }
    fclose(file);
}


double cal_dot_product_of_rows(double *row1, double *row2, int n) {
    double result = 0;
    int i;
    for (i = 0; i < n; i++) {
        result += row1[i] * row2[i];
    }
    return result;
}


double **generate2DArray(int num_of_rows, int num_of_columns) {
    double **matrix;
    int i;
    matrix = malloc(num_of_rows * sizeof(double));
    for (i = 0; i < num_of_columns; i++) {
        matrix[i] = malloc(num_of_columns * sizeof(double));
    }
    return matrix;
}



