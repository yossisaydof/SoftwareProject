#include <stdlib.h>
#include <stdio.h>
#include "spmat.h"

void add_row_linked_list(struct _spmat *A, const double *row, int i);
void free_linked_list(struct _spmat *A);
void mult_linked_list(const struct _spmat *A, const double *v, double *result);
void add_row_array(struct _spmat *A, const double *row, int i);
void free_array(struct _spmat *A);
void mult_array(const struct _spmat *A, const double *v, double *result);

typedef struct Array {
    double *values;
    int *colind, *rowptr;
} Array;


typedef struct LinkedListNode{
    double value;
    int column_index;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedListNode** spmatLinkedList;


spmat* spmat_allocate_array(int n, int nnz) {
    spmat *sparse_matrix;
    Array *spmat_array;

    spmat_array = (Array*) malloc(sizeof(Array));
    spmat_array -> values = malloc(nnz * sizeof(double));
    spmat_array -> colind = malloc(nnz * sizeof(int));
    spmat_array -> rowptr = malloc((n + 1) * sizeof(int));

    spmat_array -> rowptr[0] = 0;
    spmat_array -> rowptr[n] = nnz;

    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_array;
    sparse_matrix -> free = free_array;
    sparse_matrix -> mult = mult_array;
    sparse_matrix -> private = (Array*)spmat_array;

    return sparse_matrix;
}

spmat* spmat_allocate_list(int n) {
    spmat *sparse_matrix;
    LinkedListNode *row_lists;

    row_lists = (LinkedListNode*) malloc(n * sizeof(LinkedListNode*));

    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_linked_list;
    sparse_matrix -> free = free_linked_list;
    sparse_matrix -> mult = mult_linked_list;
    sparse_matrix -> private = (LinkedListNode*)row_lists;

    return sparse_matrix;
}


void add_row_linked_list(struct _spmat *A, const double *row, int i) {
    int j;

    spmatLinkedList row_list;
    LinkedListNode *pointer_to_node, *new_node, *tmp_node;

    row_list = (spmatLinkedList)(A->private);
    tmp_node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    pointer_to_node = tmp_node;

    for (j = 0; j < A->n; j++) {
        if (row[j] == 0)
            continue;

        new_node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        pointer_to_node->next = new_node;
        new_node->column_index = j;
        new_node->value = row[j];

        pointer_to_node = new_node;
    }
    pointer_to_node->next = NULL;
    row_list[i] = tmp_node->next;

    free(tmp_node);
}


void add_row_array(struct _spmat *A, const double *row, int i) {
    int j, *colind, *rowptr;
    Array *array_struct;
    double *values;
    static int index = 0;

    array_struct = (Array*) (A -> private);
    values = array_struct -> values;
    colind = array_struct -> colind;
    rowptr = array_struct -> rowptr;

    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0)
            continue;
        values[index] = row[j];
        colind[index] = j;
        index++;
    }
    rowptr[i + 1] = index;
}


void free_array(struct _spmat *A) {
    Array *array_struct;
    array_struct = A -> private;

    free(array_struct -> rowptr);
    free(array_struct -> colind);
    free(array_struct -> values);
    free(A);

}

void free_linked_list(struct _spmat *A) {
    int i;
    LinkedListNode *node, *tmp;
    spmatLinkedList row_list;

    row_list = (spmatLinkedList)(A->private);

    for (i = 0; i < A->n; i++) {
        node = row_list[i];
        while (node != NULL) {
            tmp = node;
            node = node -> next;
            free(tmp);
        }
    }
    free(A->private);
    free(A);
}

void mult_array(const struct _spmat *A, const double *v, double *result) {
    int i, j;
    double sum;

    Array *array_struct;
    double *values;
    int *colind, *rowptr;

    array_struct = (Array*) (A -> private);
    rowptr = array_struct->rowptr;
    values = array_struct->values;
    colind = array_struct->colind;

    for (i = 0; i < A -> n; i++) {
        sum = 0;
        for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
            sum += (double) (values[j] * v[colind[j]]);
        }
        result[i] = sum;
    }
}


void mult_linked_list(const struct _spmat *A, const double *v, double *result) {
    int i;
    double sum;
    spmatLinkedList row_list;
    LinkedListNode *pointer;

    row_list = (spmatLinkedList)(A->private);

    for (i = 0; i < A -> n; i++){
        pointer = row_list[i];
        sum = 0;
        while (pointer != NULL) {
            sum += (v[pointer -> column_index] * pointer -> value);
            pointer = pointer -> next;
        }
        result[i] = sum;
    }
}
