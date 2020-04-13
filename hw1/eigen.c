#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void generate2DArray(double **matrix, int n);
void write_vector_to_file(char *output_filename, double *eigen_vector, int n);
void read_matrix_to_memory(FILE *file, double **matrix, int n);
void create_random_vector(double *vector, int n);
void update_next_vector(double *next_vector, double **matrix, double *curr_vector, int n);
void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n);
double calc_vector_magnitude(double *vector, int n);
void create_random_vector(double *vector, int n);
void power_iteration(double *eigen_vector, double *rand_vector, double **matrix, int n);


double EPSILON = 0.00001;


int main(int argc, char** argv) {
    FILE *file;
    char *input_filename,  *output_filename;
    int num_of_rows, num_of_columns, n;
    double **input_matrix;
    double *first_vector;
    double *eigen_vector;
    (void)argc;

    srand(time(NULL));

	input_filename = argv[1];
	output_filename = argv[2];

    file = fopen(input_filename, "r");
    assert(file != NULL);
    num_of_rows = 0;
    num_of_columns = 0;
    fscanf(file, "%d", &num_of_columns);
    fscanf(file, "%d", &num_of_rows);
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
	
    return 0;
}



void read_matrix_to_memory(FILE *file, double **matrix, int n) {
    int i, j, m;

    generate2DArray(matrix, n);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m = fscanf(file, "%lf", &matrix[i][j]);
            assert(m == 1);
        }
    }
    fclose(file);

}


void generate2DArray(double **matrix, int n) {
    int i;
    for (i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(double));
    }
}



void create_random_vector(double *vector, int n){
    int i;
	for (i = 0; i < n; i++){
		vector[i] = (double)rand();
	}

}

void update_next_vector(double *next_vector, double **matrix, double *curr_vector, int n){
	double *numerator_vector;
    double denominator;
    int i;

    numerator_vector = malloc(n * sizeof(double));

	matrix_vector_multiplication(numerator_vector, matrix, curr_vector, n);
	denominator = calc_vector_magnitude(numerator_vector, n);

	for (i = 0; i < n; i++){
		next_vector[i] = numerator_vector[i]/denominator;
	}

	free(numerator_vector);
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
    double max_diff;
    double *prev_vector;
    int i, j;

	update_next_vector(eigen_vector, matrix, rand_vector, n);

    max_diff = fabs(eigen_vector[0] - rand_vector[0]);

	for (i = 1; i < n; i++){
		if (max_diff < fabs(eigen_vector[i] - rand_vector[i])){
			max_diff = fabs(eigen_vector[i] - rand_vector[i]);
		}
	}
    prev_vector = malloc(n * sizeof(double));
	while (max_diff > EPSILON){
        for (j = 0; j < n; j++) {
            prev_vector[j] = eigen_vector[j];
        }
		update_next_vector(eigen_vector, matrix, prev_vector, n);

		max_diff = fabs(eigen_vector[0] - prev_vector[0]);
		for (i = 1; i < n; i++){
		    if (max_diff > EPSILON)
                break;
			if (max_diff < fabs(eigen_vector[i] - prev_vector[i])){
				max_diff = fabs(eigen_vector[i] - prev_vector[i]);
			}
		}
	}
    free(prev_vector);
}

void write_vector_to_file(char *output_filename, double *eigen_vector, int n) {
    FILE *file;
    int m;

    file = fopen(output_filename, "w");

    m = fwrite(eigen_vector, sizeof(double), n, file);
    assert(m == n);

}
