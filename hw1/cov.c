#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

double find_mean_of_row();
double **read_matrix_to_memory();
void standardize_matrix();
void subtract_mean_from_row();
void print_matrix();
double **generate2DArray();
void create_the_covariance_matrix();
double cal_dot_product_of_rows();

//TODO: remove all print statement
int main(int argc, char** argv) {
    char* input_filename = argv[1];
    char* output_filename = argv[2];

    // TODO: remove this inputs
//    char c1[] = "files/input_matrix";
//    char c2[] = "files/output_matrix";
//    char* input_filename = c1;
//    char* output_filename = c2;

    FILE *file = fopen(input_filename, "r");
    assert(file != NULL);
    int num_of_rows = 0, num_of_columns = 0;
    fscanf(file, "%d", &num_of_columns);
    fscanf(file, "%d", &num_of_rows);
    
    double **matrix = generate2DArray(num_of_rows, num_of_columns);

    read_matrix_to_memory(file, matrix, num_of_rows, num_of_columns);

    fclose(file);

    printf("Input Matrix:\n");
    print_matrix(matrix, num_of_rows, num_of_columns);

    standardize_matrix(matrix, num_of_rows, num_of_columns);
    printf("Covariance Matrix:\n");
    print_matrix(matrix, num_of_rows, num_of_columns);

    printf("Output Matrix:\n");
    create_the_covariance_matrix(matrix, output_filename, num_of_rows, num_of_columns);

    printf("\nDone");
}


double **read_matrix_to_memory(FILE *file, double **matrix, int num_of_rows, int num_of_column) {
    for (int i = 0; i < num_of_rows; i++) {
        for (int j = 0; j < num_of_column; j++) {
            int n = fscanf(file, "%lf", &matrix[i][j]);
            assert(n == 1);
        }
    }
}


void standardize_matrix(double **matrix_input, int num_of_rows, int num_of_columns) {
    double mean;
    for (int i = 0; i < num_of_rows; i++) {
        mean = find_mean_of_row(matrix_input[i], num_of_columns);
        subtract_mean_from_row(matrix_input[i], mean, num_of_columns);
    }
}


double find_mean_of_row(double *row, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + row[i];
    }
    return sum / n;
}


void subtract_mean_from_row(double *row, double mean, int n) {
    for (int i = 0; i < n; i++) {
        row[i] = row[i] - mean;
    }
}


void create_the_covariance_matrix(double **input_matrix, char *output_filename, int num_of_rows, int num_of_columns) {
    FILE *file = fopen(output_filename, "w");
    double *row_to_write, dot_product_of_rows;

    for (int i = 0; i < num_of_rows; i++) {
        row_to_write = malloc(num_of_rows * sizeof(double));
        for (int j = 0; j < num_of_rows; j++) {
            dot_product_of_rows = cal_dot_product_of_rows(input_matrix[i], input_matrix[j], num_of_columns);
            row_to_write[j] = dot_product_of_rows;
            printf("%lf ", row_to_write[j]);
        }
        printf("\n");
        int n = fwrite(row_to_write, sizeof(double), num_of_rows, file);
        assert(n == num_of_rows);
    }
    fclose(file);
}


double cal_dot_product_of_rows(double *row1, double *row2, int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        result += row1[i] * row2[i];
    }
    return result;
}


double **generate2DArray(int num_of_rows, int num_of_columns) {
    double **matrix;
    matrix = malloc(num_of_rows * sizeof(double));
    for (int i = 0; i < num_of_columns; i++) {
        matrix[i] = malloc(num_of_columns * sizeof(double));
    }
    return matrix;
}


void print_matrix(double **matrix, int num_of_rows, int num_of_columns) {
    for (int i = 0; i < num_of_rows; i++) {
        for (int j = 0; j < num_of_columns; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

