#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "divideIntoGroups.h"
#include "matrixShifting.h"


void initialize_array_of_zeros(int *array, int n) {
    int i;
    for (i = 0; i < n; i++) {
        array[i] = 0;
    }
}

matrixStructure* generate_matrix_structure(FILE *matrix_file) {
    /*
     * Reading the input file and store it in matrix structure.
     */
    int n, i, j, M = 0, node_degree, node_id, *K, *matrix_row;
    matrixStructure *matrix_structure;
    spmat *spmat_matrix;

    if ((int) fread(&n, sizeof(int), 1, matrix_file) != 1)
        ERROR_HANDLER(FILE_READING)

    if (n == 0) ERROR_HANDLER(ZERO_NODES)

    K = (int*) malloc(n * sizeof(int));
    if (K == NULL) ERROR_HANDLER(MALLOC_FAILED)

    for (i = 0; i < n; i++) {
        if (fread(&node_degree, sizeof(int), 1, matrix_file) != 1)
            ERROR_HANDLER(FILE_READING);

        K[i] = node_degree;
        M += node_degree;
        if (fseek(matrix_file, (long)(node_degree * sizeof(int)), SEEK_CUR) != 0)
            ERROR_HANDLER(FILE_SEEK);
    }
    if (M == 0) ERROR_HANDLER(ZERO_EDGES) /* M can not be zero because we divide by M */

    spmat_matrix = spmat_allocate_array(n, M);

    matrix_row = (int*) malloc(n * sizeof(int));
    if (matrix_row == NULL) ERROR_HANDLER(MALLOC_FAILED)

    /* skip number of nodes */
    if (fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_SET) != 0)
        ERROR_HANDLER(FILE_SEEK)

    for (i = 0; i < n; i++) {
        /* skip node degree */
        if (fseek(matrix_file, (long)(1 * sizeof(int)), SEEK_CUR) != 0)
            ERROR_HANDLER(FILE_SEEK)

        initialize_array_of_zeros(matrix_row, n);
        node_degree = K[i];
        for (j = 0; j < node_degree; j++) {
            if (fread(&node_id, sizeof(int), 1, matrix_file) != 1)
                ERROR_HANDLER(FILE_READING)

            matrix_row[node_id] = 1;
        }
        spmat_matrix -> add_row(spmat_matrix, matrix_row, i);

    }
    matrix_structure = allocate_matrix_structure(K, spmat_matrix, M, n);
    matrix_structure -> norm_1 = norm_l1(matrix_structure);
    free(matrix_row);

    return matrix_structure;
}


void write_output_file(FILE *output_file, modularityGroups *modularity_groups) {
    /*
     * Iteration over the group O and write all the groups it contains to the output file
     */
    int i, j, n, group_size;
    group *head;

    n = modularity_groups -> number_of_groups;
    head = modularity_groups -> head;

    /* The first value represents the number of groups in the division */
    if (fwrite(&n, sizeof(int), 1, output_file) != 1)
        ERROR_HANDLER(FILE_WRITING)

    for (i = 0; i < n; i++) {
        group_size = head -> size;
        /* write number of nodes in the first group */
        if ((int) fwrite(&group_size, sizeof(int), 1, output_file) != 1)
            ERROR_HANDLER(FILE_WRITING)

        /* followed by the indices of the nodes in the group, in increasing order */

        for (j = 0; j < head -> size; j++) {
            if ((int) fwrite(&(head -> nodes[j]), sizeof(int), 1, output_file) != 1)
                ERROR_HANDLER(FILE_WRITING)
        }

        head = head -> next;
    }
}


int main(int argc, char* argv[]) {
    FILE *input_matrix_file, *output_file;
    char *input_filename, *output_filename;
    matrixStructure *matrix_structure;
    modularityGroups *modularity_groups;
    clock_t begin, end; /* TODO - delete*/
    double time_spent; /* TODO - delete*/

    (void) argc;

    begin = clock(); /* TODO - delete*/

    srand(time(NULL));

    input_filename = argv[1];
    input_matrix_file = fopen(input_filename, "rb");
    if (input_matrix_file == NULL) ERROR_HANDLER(FILE_OPENING)

    matrix_structure = generate_matrix_structure(input_matrix_file);

    /* Run Algorithm 3 to find modularity groups */
    modularity_groups = allocate_modularity_group();
    divide_into_groups(matrix_structure, modularity_groups);

    output_filename = argv[2];
    output_file = fopen(output_filename, "wb");
    if (output_file == NULL) ERROR_HANDLER(FILE_OPENING)

    write_output_file(output_file, modularity_groups);

    /* Free output modularity groups */
    modularity_groups -> free(modularity_groups);

    matrix_structure -> free(matrix_structure);
    fclose(input_matrix_file);
    fclose(output_file);

    end = clock(); /* TODO - delete*/
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Total time: %f\n", time_spent); /* TODO - delete */

    exit(EXIT_SUCCESS);
}
