#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "divideIntoGroups.h"
#include "matrixShifting.h"
#include "error_handler.h"


void initialize_array_of_zeros(int *array, int n) {
    int i;
    for (i = 0; i < n; i++) {
        array[i] = 0;
    }
}
/*
    input = [3 2 1 2 1 0 2 2 0 1]
    n - #: e1 e2    #: e3 e4     #: e5 e6
    3 - 2:  1 2     2:  0 2      2:  0 1
    0 1 1
    1 0 1
    1 1 0
*/
matrixStructure* generate_matrix_structure(FILE *matrix_file) {
    int n, i, j, M = 0, node_degree, node_id, *K, *matrix_row;
    matrixStructure *matrix_structure;
    spmat *spmat_matrix;

    if ((int) fread(&n, sizeof(int), 1, matrix_file) == 1) {
        printf("%s", FILE_READING);
        exit(EXIT_FAILURE);
    }

    K = (int*) malloc(n * sizeof(int));
    if (K == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; ++i) {
        if (fread(&node_degree, sizeof(int), 1, matrix_file) != 1) {
            printf("%s", FILE_READING);
            exit(EXIT_FAILURE);
        }

        K[i] = node_degree;
        M += node_degree;
        if (fseek(matrix_file, (long)(node_degree * sizeof(int)), SEEK_CUR) != 0) {
            printf("%s", FILE_SEEK);
            exit(EXIT_FAILURE);
        }
    }
    if (M != 0) { /* M cant be zero cause we divide by M */
        printf("%s", DIVIDE_BY_ZERO);
        exit(EXIT_FAILURE);
    }

    spmat_matrix = spmat_allocate_array(n, M);
    matrix_row = (int*) malloc(n * sizeof(int));

    /* skip number of nodes */
    if (fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_SET) != 0)
        exit(EXIT_FAILURE);

    for (i = 0; i < n; i++) {
        initialize_array_of_zeros(matrix_row, n);
        node_degree = K[i];
        for (j = 0; j < node_degree; j++) {
            if (fread(&node_id, sizeof(int), 1, matrix_file) != 1)
                exit(EXIT_FAILURE);

            matrix_row[node_id] = 1;
        }
        spmat_matrix -> add_row(spmat_matrix, matrix_row, i);
        /* skip node degree */
        if (fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_CUR) != 0)
            exit(EXIT_FAILURE);
    }
    matrix_structure = allocate_matrix_structure(K, spmat_matrix, M, n);
    matrix_structure -> norm_1 = norm_l1(matrix_structure);
    free(matrix_row);

    return matrix_structure;
}

void write_output_file(FILE *output_file, modularityGroups *modularity_groups) {
    int i, n, group_size;
    group *head;

    n = modularity_groups -> number_of_groups;
    head = modularity_groups -> head;

    /* The first value represents the number of groups in the division */
    if (fwrite(&n, sizeof(int), 1, output_file) != 1)
        exit(EXIT_FAILURE);

    for (i = 0; i < n; i++) {
        group_size = head -> size;
        /* write number of nodes in the first group */
        if ((int) fwrite(&(group_size), sizeof(int), 1, output_file) != 1)
            exit(EXIT_FAILURE);

        /* followed by the indices of the nodes in the group, in increasing order */
        /* TODO: sort nodes before writing to file */
        if ((int) fwrite(head -> nodes, sizeof(int), group_size, output_file) != group_size)
            exit(EXIT_FAILURE);
    }
}


int main(int argc, char* argv[]) {
    FILE *input_matrix_file, *output_file;
    char *input_matrix_filename, *output_filename;
    matrixStructure *matrix_structure;
    modularityGroups *modularity_groups;
    (void) argc;

    srand(time(NULL));

    input_matrix_filename = argv[1];
    input_matrix_file = fopen(input_matrix_filename, "r");
    if(input_matrix_file != NULL)
        exit(EXIT_FAILURE);

    matrix_structure = generate_matrix_structure(input_matrix_file);

    modularity_groups = divide_into_groups(matrix_structure);

    output_filename = argv[2];
    output_file = fopen(output_filename, "w");
    if (output_file != NULL)
        exit(EXIT_FAILURE);

    write_output_file(output_file, modularity_groups);

    /* Free output modularity groups */
    modularity_groups->free(modularity_groups);
    free_matrix_structure(matrix_structure);

    return 0;
}
