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
    int *colind;
    int *rowptr;
    int index;
} Array;


typedef struct LinkedListNode{
    double value;
    int column_index;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedListNode** spmatLinkedList;
//typedef struct Array** spmatArray;


spmat* spmat_allocate_array(int n, int nnz) {
    spmat *sparse_matrix;
    Array *spmat_array;

    spmat_array = (Array*) malloc(sizeof(Array));
    spmat_array -> values = malloc(nnz * sizeof(double));
    spmat_array -> colind = malloc(nnz * sizeof(int));
    spmat_array -> rowptr = malloc((nnz + 1) * sizeof(int));
    spmat_array -> index = -1;

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
    int index, index_row, j;
//    spmatArray spmat_array;
    Array *array_struct;

    array_struct = (Array*) (A -> private);
    index = array_struct -> index;
    index_row = index + 1;

    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0) continue;

        array_struct -> index ++;
        index = array_struct -> index;
        array_struct -> values[index] = row[j];
        array_struct -> colind[index] = j;
        array_struct -> rowptr[index] = index_row;
    }

    if (i + 1 == A -> n) {
        if (index == 0) {
            array_struct -> rowptr[array_struct -> index] = array_struct -> index + 1;
        }
        array_struct -> rowptr[(array_struct -> index) + 1] = (array_struct -> index) + 1;
    }
}


void free_array(struct _spmat *A) {
    Array *array_struct = A -> private;

    free(array_struct -> rowptr);
    free(array_struct -> colind);
    free(array_struct -> values);
    free(array_struct);

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

void mult_array(const struct _spmat *A, const double *row, double *result) {
    int index, j;
//    spmatArray spmat_array;
    Array *array_struct;

    array_struct = (Array*) (A -> private);
    index = array_struct -> index;

    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0) continue;

        array_struct -> index ++;
        index = array_struct -> index;
        array_struct -> values[index] = row[j];
        array_struct -> colind[index] = j;
        array_struct -> rowptr[index] = j;
    }

    if (j == A -> n) {
        if (index == 0) {
            array_struct -> rowptr[array_struct -> index] = array_struct -> index + 1;
        }
        array_struct -> rowptr[(array_struct -> index) + 1] = (array_struct -> index) + 1;
    }
    result[0] = 0;

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
