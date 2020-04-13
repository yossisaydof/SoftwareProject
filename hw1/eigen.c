#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "SPBufferset.h"

double **read_matrix_to_memory(char* filename, int num_of_rows, int num_of_column);
void create_random_vector(double *vector, int n);
void update_next_vector(double *next_vector, double **matrix, double *curr_vector, int n);
void matrix_vector_multiplication(double *new_vector, double **matrix, double *vector, int n);
double calc_vector_magnitude(double *vector, int n);


double epsilon = 0.00001;


int main(int argc, char** argv) {

	SP_BUFF_SET();
	srand(time(NULL));

	char* input_filename = argv[1];
	char* output_filename = argv[2];
	int n; //TODO n should be the number of columns/rows in the matrix (rows == columns)

    double **input_matrix;
    input_matrix = malloc(n * sizeof(double));
	read_matrix_to_memory(input_matrix, input_filename, n, n);

	double *first_vector;
	first_vector = malloc(n * sizeof(double));

	double *eigen_vector;
	eigen_vector = malloc(n * sizeof(double));

	power_iteration(eigen_vector, input_matrix, first_vector, n);

	free(first_vector);
	free(eigen_vector);
	free(input_matrix);
}



void read_matrix_to_memory(double **matrix, char* filename, int n) {
	// Read the entire input matrix into memory

	FILE *file = fopen(filename, "r");
    assert(file != NULL);

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


	double *numerator = matrix_vector_multiplication(numerator_vector, matrix, curr_vector, n);
	double denominator = calc_vector_magnitude(numerator, n);

	for (int i = 0; i < n; i++){
		next_vector[i] = numerator[i]/denominator;
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

	while (max_diff > epsilon){
		// We are done iterating when the change in the new vector is small enough
		update_next_vector(eigen_vector, matrix, rand_vector, n);

		max_diff = fabs(eigen_vector[0] - rand_vector[0]);
		for (int i = 1; i < n; i++){
			if (max_diff < fabs(eigen_vector[i] - rand_vector[i])){
				max_diff = fabs(eigen_vector[i] - rand_vector[i]);
			}
		}
	}


}
