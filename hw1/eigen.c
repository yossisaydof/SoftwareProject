#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void generate2DArray(double **matrix, int n);
void write_vector_to_file(char *output_filename, double *eigen_vector, int n);
void read_matrix_to_memory(FILE *file, double **matrix, char* filename, int n);
void create_random_vector(double *vector, int n);
void update_next_vector(double *next_vector, double **matrix, double *curr_vector, int n);
void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n);
double calc_vector_magnitude(double *vector, int n);
void create_random_vector(double *vector, int n);
void power_iteration(double *eigen_vector, double *rand_vector, double **matrix, int n);


double EPSILON = 0.00001;


int main(int argc, char** argv) {

	srand(time(NULL));

	char* input_filename = argv[1];
	char* output_filename = argv[2];

    FILE *file = fopen(input_filename, "r");
    assert(file != NULL);
    int num_of_rows = 0, num_of_columns = 0;
    fscanf(file, "%d", &num_of_columns);
    fscanf(file, "%d", &num_of_rows);
    assert(num_of_rows == num_of_columns);

    int n = num_of_rows;

    double **input_matrix;
    input_matrix = malloc(n * sizeof(double));
	read_matrix_to_memory(file, input_matrix, input_filename, n);

	double *first_vector;
	first_vector = malloc(n * sizeof(double));
	create_random_vector(first_vector, n);

    for (int i = 0; i < n; i++) {
        printf("%lf\n", first_vector[i]);
    }

	double *eigen_vector;
	eigen_vector = malloc(n * sizeof(double));

	power_iteration(eigen_vector, first_vector, input_matrix, n);

	write_vector_to_file(output_filename, eigen_vector, n);

    for (int i = 0; i < n; i++) {
        printf("%lf\n", eigen_vector[i]);
    }

	free(first_vector);
	free(eigen_vector);
	free(input_matrix);
}



void read_matrix_to_memory(FILE *file, double **matrix, char* filename, int n) {
	// Read the entire input matrix into memory
    generate2DArray(matrix, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int n = fscanf(file, "%lf", &matrix[i][j]);
            assert(n == 1);
        }
    }
    fclose(file);

}


void generate2DArray(double **matrix, int n) {
	// TODO - free memory
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(double));
    }
}



void create_random_vector(double *vector, int n){
	// update vector to random numbers

	for (int i = 0; i < n; i++){
		vector[i] = (double)rand();
	}

}

void update_next_vector(double *next_vector, double **matrix, double *curr_vector, int n){
	// calculates the next normalized vector according to the following equation: (matrix * vector)/||(matrix * vector)||
	double *numerator_vector;
	numerator_vector = malloc(n * sizeof(double));


	matrix_vector_multiplication(numerator_vector, matrix, curr_vector, n);
	double denominator = calc_vector_magnitude(numerator_vector, n);

	for (int i = 0; i < n; i++){
		next_vector[i] = numerator_vector[i]/denominator;
	}

	free(numerator_vector);
}


void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n){
	// updates new_vector as the multiplication of matrix*vector
	double sum = 0;

	for (int i = 0; i < n; i++){
		// iterating over the matrix's rows and new_vector's rows
		for (int j = 0; j < n; j++){
			// iterating over the matrix's columns and vector's rows
			sum += matrix[i][j] * vector[j];
		}
		new_vector[i] = sum;
		sum = 0;
	}

}

double calc_vector_magnitude(double *vector, int n){
	// calculates the vector's magnitude (i.e. square root of the vector's dot product with itself)
	double sum_squares = 0;

	for (int i = 0; i < n; i++){
		sum_squares += vector[i] * vector[i];
	}

	return sqrt(sum_squares);
}

void power_iteration(double *eigen_vector, double *rand_vector, double **matrix, int n){
	// updates the eigenvalue in eigen_vector

	update_next_vector(eigen_vector, matrix, rand_vector, n);

	double max_diff = fabs(eigen_vector[0] - rand_vector[0]);

	for (int i = 1; i < n; i++){
		if (max_diff < fabs(eigen_vector[i] - rand_vector[i])){
			max_diff = fabs(eigen_vector[i] - rand_vector[i]);
		}
	}
    double *prev_vector = malloc(n * sizeof(double));
	while (max_diff > EPSILON){
		// We are done iterating when the change in the new vector is small enough
        for (int j = 0; j < n; j++) {
            prev_vector[j] = eigen_vector[j];
        }
		update_next_vector(eigen_vector, matrix, prev_vector, n);

		max_diff = fabs(eigen_vector[0] - prev_vector[0]);
		for (int i = 1; i < n; i++){
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
    FILE *file = fopen(output_filename, "w");

    int m = fwrite(eigen_vector, sizeof(double), n, file);
    assert(m == n);

}
