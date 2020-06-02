#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void create_random_vector(double *vector, int n);
void update_next_vector(FILE *file, double *input_vector, double *next_vector, double *curr_vector, double *numerator_vector, int n);
void matrix_vector_multiplication(double *input_vector, FILE *file, double *new_vector, double *vector, int n);
double calc_vector_magnitude(double *vector, int n);
void power_iteration(double *input_vector,FILE *file, double *eigen_vector, double *rand_vector, int n,
                     double *prev_vector, double *numerator_vector);
void write_vector_to_file(char *output_filename, double *eigen_vector, int n);


double EPSILON = 0.00001;

int main(int argc, char* argv[]) {
    FILE *file;
    char *input_filename,  *output_filename;
    int num_of_rows, num_of_columns;
    int matrix_dimension[2];
    double *input_rows;
    double *first_vector;
    double *eigen_vector;
    double *prev_vector;
    double *numerator_vector;
    (void)argc;

    srand(time(NULL));

	input_filename = argv[1];
	output_filename = argv[2];

    file = fopen(input_filename, "r");
    assert(file != NULL);

    /* Getting matrix's dimensions */
    num_of_rows = 0;
    num_of_columns = 0;
    fread(matrix_dimension, sizeof(int), 2, file);
    num_of_columns = matrix_dimension[0];
    num_of_rows = matrix_dimension[1];
    assert(num_of_rows == num_of_columns);

    /* Creating a vector to read the matrix's rows */
    input_rows = malloc(num_of_rows * sizeof(double));

    /* Creating b1 from random numbers */
	first_vector = malloc(num_of_rows * sizeof(double));
	create_random_vector(first_vector, num_of_rows);

	eigen_vector = malloc(num_of_rows * sizeof(double));
	rewind(file);

    prev_vector = malloc(num_of_rows * sizeof(double));
    numerator_vector = malloc(num_of_rows * sizeof(double));
	power_iteration(input_rows, file, eigen_vector, first_vector, num_of_rows, prev_vector, numerator_vector);

	write_vector_to_file(output_filename, eigen_vector, num_of_rows);

    fclose(file);
    free(prev_vector);
    free(numerator_vector);
    free(first_vector);
	free(eigen_vector);
	free(input_rows);

    return 0;
}

void create_random_vector(double *vector, int n){
	/*
	 * Update vector to random numbers
	 */
    int i;
	for (i = 0; i < n; i++){
		vector[i] = (double)rand();
	}

}


double calc_vector_magnitude(double *vector, int n){
	/*
	 * Calculates the vector's magnitude (i.e. square root of the vector's dot product with itself)
	*/
	double sum_squares = 0;
	int i;

	for (i = 0; i < n; i++){
		sum_squares += vector[i] * vector[i];
	}

	return sqrt(sum_squares);
}

void power_iteration(double *input_vector,FILE *file, double *eigen_vector, double *rand_vector, int n,
                     double *prev_vector, double *numerator_vector){
	/*
	 * Updates the eigenvalue in eigen_vector
	*/
    double max_diff, diff;
    int i, j;


    /* Updating eigen_vector to be b1 */
	update_next_vector(file, input_vector, eigen_vector, rand_vector, numerator_vector, n);
    max_diff = fabs(eigen_vector[0] - rand_vector[0]);
    if (max_diff >= EPSILON){
    	for (i = 1; i < n; i++){
    		if (max_diff < fabs(eigen_vector[i] - rand_vector[i])){
    				max_diff = fabs(eigen_vector[i] - rand_vector[i]);
    		}
    	}
    }

    while (max_diff >= EPSILON){
    	/*
    	 * We are done iterating when the change in the new vector is small enough
    	*/
        for (j = 0; j < n; j++) {
            prev_vector[j] = eigen_vector[j];
        }
		update_next_vector(file, input_vector, eigen_vector, prev_vector, numerator_vector, n);

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
}

void update_next_vector(FILE *file, double *input_vector, double *next_vector, double *curr_vector, double *numerator_vector, int n){
	/*
	 * Calculates the next normalized vector according to the following equation:
	 *  (matrix * vector)/||(matrix * vector)||
	 */
    double denominator;
    int i;

	matrix_vector_multiplication(input_vector, file, numerator_vector, curr_vector, n);
    rewind(file);
	denominator = calc_vector_magnitude(numerator_vector, n);

	for (i = 0; i < n; i++){
		next_vector[i] = (double) numerator_vector[i] / denominator;
	}
}

void matrix_vector_multiplication(double *input_vector, FILE *file, double *new_vector, double *vector, int n){
	/*
	 * Updates new_vector as the multiplication of matrix*vector
	 */
	double sum;
    int i, j, num;
	int matrix_dim[2];
    sum = 0;

    /* Read first 2 numbers from file */
    num = fread(matrix_dim, sizeof(int), 2, file);
    assert(num == 2);
    for (i = 0; i < n; ++i) {
    	/* Iterating over the martix's rows */
    	num = fread(input_vector, sizeof(double), n, file); /* Read row i to input_vector (row i is of length n) */
		assert(num == n);
		for (j = 0; j < n; ++j) {
			sum += input_vector[j] * vector[j];
		}

		new_vector[i] = sum;
		sum = 0;
	}

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

    fclose(file);

}
