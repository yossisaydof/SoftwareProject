#include "powerIteration.h"

double EPSILON = 0.00001;

void create_random_vector(int n, double *randVector){
    /* Fill randVector with random values */
    int i;

    for (i = 0; i < n; i++) {
        randVector[i] = (double) rand();
    }
}

double calc_vector_magnitude(const double *vector, int n) {
    /* Calculates the vector's magnitude (i.e. square root of the vector's dot product with itself) */
    double sum_squares = 0;
    int i;

    for (i = 0; i < n; i++){
        sum_squares += vector[i] * vector[i];
    }

    return sqrt(sum_squares);
}


double calc_next_vector_i(matrixStructure *matrix, group *g, const double *curr_vector, int i) {

     /* Calculates: next_vextor[i] = norm * v_i + sum over j in g, j != i of (A_ij - k_i*k_j/M) * (v_j - j_i) */

    int j, A_ij, k_i, k_j, j_index, nnz_i, cnt_nnz = 0, row_start, row_end, *K, *nodes;
    double sum = 0, M;

    spmat *A;

    A = matrix -> A;
    K = matrix -> degreeList;
    k_i = K[i];
    M = matrix -> M;
    row_start = A -> rowptr[i];
    row_end = A -> rowptr[i + 1];
    nnz_i = row_end - row_start;
    nodes = g -> nodes;

    for (j = 0; j < g -> size; j++) {
        j_index = nodes[j];
        if (i == j_index) {
            sum += ((matrix -> norm_1) * curr_vector[i]); /* matrix shifting */
            continue;
        }
        A_ij = 0;
        k_j = K[j_index];
        if (cnt_nnz < nnz_i) {
            if (j == A -> colind[row_start + cnt_nnz]) {
                cnt_nnz++;
                A_ij = (int) A -> values[row_start + cnt_nnz];
            }
        }
        sum += ((A_ij - (double)((k_i * k_j) / M)) * (curr_vector[j] - curr_vector[i]));
    }
    return sum;
}


void mult_matrix_vector(matrixStructure *matrix, group *g, double* curr_vector, double* next_vector) {

     /* Calculates B_hat[h] * curr_vector */

    int i;

    for (i = 0; i < g -> size; i++) {
        next_vector[i] = calc_next_vector_i(matrix, g, curr_vector, g -> nodes[i]);
    }
}

/*
void mult_matrix_vector(matrixStructure *matrix_structure, group *g, double* curr_vector, double* next_vector) {

     * Calculates B_hat[g] * curr_vector
     * We split B_hat[g] to 3 matrices:
     *      1. f[g]
     *      2. A[g]
     *      3. matrix[i][j] = k_i * k_j / M
     * We calculate f[g]*curr_vector, A[g]*curr_vector, matrix*curr_vector and sum them all together
     *
    double *f, *f_s, *k_m_v, *v, tmp_sum;
    int i, n;

    n = g -> size;

    f = malloc(sizeof(double) * n);
    if (f == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    f_s = malloc(sizeof(double) * n);
    if (f_s == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    k_m_v = malloc(sizeof(double) * n);
    if (k_m_v == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    v = malloc(sizeof(double) * n);
    if (v == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    calc_f_g(matrix_structure, g, f);
    mult_f_v_double(f, curr_vector, f_s, n);
    mult_A_g_double(matrix_structure -> A, curr_vector, g, v);
    mult_kkM_double(matrix_structure, g, curr_vector, k_m_v);

    for (i = 0; i < n; i++) {
        tmp_sum = f_s[i] + k_m_v[i] + v[i];
        next_vector[i] = tmp_sum;
    }

    free(f);
    free(f_s);
    free(v);
    free(k_m_v);
}*/

void calc_next_vector(matrixStructure *matrix_structure, group *g, double* curr_vector, int n, double *next_vector) {
    double denominator;
    int i;
    /* calculates numerator (i.e matrix_structure * curr_vector) */
    mult_matrix_vector(matrix_structure, g, curr_vector, next_vector);

    /* calculates denominator (i.e ||(matrix_structure * curr_vector)||) */
    denominator = calc_vector_magnitude(next_vector, n);
    if (denominator == 0) {
        printf("%s", DIVIDE_BY_ZERO);
        exit(EXIT_FAILURE);
    }

    /* update next_vector */
    for (i = 0; i < n; i++) {
        next_vector[i] = (double) (next_vector[i] / denominator);
    }
}

int check_diff(double *curr_vector, double *next_vector, int n) {
    /* Returns 1 if all differences are smaller than EPSILON */
    int i;
    double diff;
    for (i = 0; i < n; i++) {
        diff = fabs(curr_vector[i] - next_vector[i]);
        if (diff > EPSILON) {
            return 1;
        }
    }
    return 0;
}

double clac_eigenvalue(matrixStructure *matrix_structure, group *g, double *eigen_vector) {
    /* Calculates an approximation of the corresponding dominant eigenvalue */
    int i, n;
    double denominator = 0 , numerator = 0, *f, *f_v, *k_m_v, *A_v, B_g_i;

    n = g -> size;

    f = malloc(sizeof(double) * n);
    if (f == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    k_m_v = malloc(sizeof(double) * n);
    if (k_m_v == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    A_v = malloc(sizeof(double) * n);
    if (A_v == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    f_v = malloc(sizeof(double) * n);
    if (f_v == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    calc_f_g(matrix_structure, g, f);
    mult_f_v_double(f, eigen_vector, f_v, n);
    mult_A_g_double(matrix_structure -> A, eigen_vector, g, A_v);
    mult_kkM_double(matrix_structure, g, eigen_vector, k_m_v);

    for (i = 0; i < n ; i++) {
        denominator += (eigen_vector[i] * eigen_vector[i]);
        B_g_i = f_v[i] + k_m_v[i] + A_v[i];
        numerator += (B_g_i * eigen_vector[i]);
    }
    if (denominator == 0) {
        printf("%s", DIVIDE_BY_ZERO);
        exit(EXIT_FAILURE);
    }

    free(f);
    free(k_m_v);
    free(A_v);
    free(f_v);

    return (numerator / denominator);
}

double power_iteration(matrixStructure *matrix_structure, group *g, double *eigen_vector) {
    /*
     * Approximates the dominant eigenpair
     * Stores the corresponding eigenvector in eigen_vector
     * Returns the largest eigenvalue
     */

    double *curr_vector, eigen_value;
    int n, cnt_diff = 0, i;

    n = g -> size;
    curr_vector = malloc(sizeof(double) * n);
    if (curr_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    create_random_vector(n, curr_vector);
    printf("%s\n", "Created random vector.. ");

    eigen_vector = malloc(sizeof(double) * n);
    if (eigen_vector == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    while (1) { /* TODO: make sure this is not an infinite loop! */
        calc_next_vector(matrix_structure, g, curr_vector, n, eigen_vector);
        cnt_diff++;
        if (check_diff(curr_vector, eigen_vector, n) == 0) {
            /* the vector produced in the final iteration is the desired eigenvector */
            break;
        }
        for (i = 0; i < n; i++) {
            curr_vector[i] = eigen_vector[i];
        }
    }

    eigen_value = clac_eigenvalue(matrix_structure, g, eigen_vector);
    printf("%s%f\n", "Eigen value is: ", eigen_value);
    free(curr_vector);
    free(eigen_vector);

    return eigen_value;
}
