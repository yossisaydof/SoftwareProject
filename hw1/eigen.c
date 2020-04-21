#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void write_vector_to_file(char *output_filename, double *eigen_vector, int n);
void read_matrix_to_memory(FILE *file, double **matrix, int n);
void create_random_vector(double *vector, int n);
void update_next_vector(double *next_vector, double **matrix, double *curr_vector, double *numerator_vector, int n);
void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n);
double calc_vector_magnitude(double *vector, int n);
void create_random_vector(double *vector, int n);
void power_iteration(double *eigen_vector, double *rand_vector, double **matrix, int n);


double EPSILON = 0.00001;


int main(int argc, char* argv[]) {
    FILE *file;
    clock_t start, end;
    char *input_filename,  *output_filename;
    int num_of_rows, num_of_columns, n;
    int matrix_dimension[2];
    double **input_matrix;
    double *first_vector;
    double *eigen_vector;
    (void)argc;

    start = clock();

    srand(time(NULL));

	input_filename = argv[1];
	output_filename = argv[2];

    file = fopen(input_filename, "r");
    assert(file != NULL);

    num_of_rows = 0;
    num_of_columns = 0;

    fread(matrix_dimension, sizeof(int), 2, file);
    num_of_columns = matrix_dimension[0];
    num_of_rows = matrix_dimension[1];
    assert(num_of_rows == num_of_columns);

    n = num_of_rows;

    input_matrix = malloc(n * sizeof(double));

	read_matrix_to_memory(file, input_matrix, n);

	first_vector = malloc(n * sizeof(double));
	create_random_vector(first_vector, n);

	eigen_vector = malloc(n * sizeof(double));

	power_iteration(eigen_vector, first_vector, input_matrix, n);

	write_vector_to_file(output_filename, eigen_vector, n);

	free(first_vector);
	free(eigen_vector);
	free(input_matrix);

    end = clock();

    printf("Execution took %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));

    return 0;
}



void read_matrix_to_memory(FILE *file, double **matrix, int n) {
    int i, m;

    for (i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(double));
        m = fread(matrix[i], sizeof(double), n, file);
        assert(n == m);
    }

    fclose(file);

}


void create_random_vector(double *vector, int n){
    int i;
	for (i = 0; i < n; i++){
		vector[i] = (double)rand();
	}

}

void update_next_vector(double *next_vector, double **matrix, double *curr_vector, double *numerator_vector, int n){
    double denominator;
    int i;

	matrix_vector_multiplication(numerator_vector, matrix, curr_vector, n);
	denominator = calc_vector_magnitude(numerator_vector, n);

	for (i = 0; i < n; i++){
		next_vector[i] = (double) numerator_vector[i] / denominator;
	}
}


void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n){
	double sum;
    int i, j;
    sum = 0;

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			sum += matrix[i][j] * vector[j];
		}
		new_vector[i] = sum;
		sum = 0;
	}

}

double calc_vector_magnitude(double *vector, int n){
	double sum_squares = 0;
	int i;

	for (i = 0; i < n; i++){
		sum_squares += vector[i] * vector[i];
	}

	return sqrt(sum_squares);
}

void power_iteration(double *eigen_vector, double *rand_vector, double **matrix, int n){
    double max_diff, diff;
    double *prev_vector, *numerator_vector;
    int i, j;

    prev_vector = malloc(n * sizeof(double));
    numerator_vector = malloc(n * sizeof(double));

	update_next_vector(eigen_vector, matrix, rand_vector, numerator_vector, n);

    max_diff = fabs(eigen_vector[0] - rand_vector[0]);

	for (i = 1; i < n; i++){
		if (max_diff < fabs(eigen_vector[i] - rand_vector[i])){
			max_diff = fabs(eigen_vector[i] - rand_vector[i]);
		}
	}

    while (max_diff >= EPSILON){
        for (j = 0; j < n; j++) {
            prev_vector[j] = eigen_vector[j];
        }
		update_next_vector(eigen_vector, matrix, prev_vector, numerator_vector, n);

		max_diff = fabs(eigen_vector[0] - prev_vector[0]);
		for (i = 1; i < n; i++){
		    if (max_diff >= EPSILON)
                break;
		    diff = fabs(eigen_vector[i] - prev_vector[i]);
			if (max_diff < diff){
				max_diff = diff;
			}
		}
	}
    free(prev_vector);
    free(numerator_vector);
}

void write_vector_to_file(char *output_filename, double *eigen_vector, int n) {
    FILE *file;
    int m, k;
    int matrix_dimension[2];

    matrix_dimension[0] = 1;
    matrix_dimension[1] = n;

    file = fopen(output_filename, "w");

    k = fwrite(matrix_dimension, sizeof(int), 2, file);
    m = fwrite(eigen_vector, sizeof(double), n, file);

    assert(k == 2);
    assert(m == n);

}
