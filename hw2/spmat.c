#include <stdlib.h>
#include <stdio.h>
#include <mach/machine.h>
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


typedef struct LinkedList{
    double value;
    int column_index;
    struct LinkedList *next;
} LinkedList;


spmat* spmat_allocate_array(int n, int nnz) {
    spmat* sparse_matrix;
    Array* spmat_array;

    spmat_array = (Array*)malloc(sizeof(Array));
    spmat_array -> values = malloc(nnz * sizeof(double));
    spmat_array -> colind = malloc(nnz * sizeof(int));
    spmat_array -> rowptr = malloc((nnz + 1) * sizeof(int));
    spmat_array -> index = -1;

    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_array;
    sparse_matrix -> free = free_array;
    sparse_matrix -> mult = mult_array;
    sparse_matrix -> private = spmat_array;

    return sparse_matrix;
}

spmat* spmat_allocate_list(int n) {
    spmat* sparse_matrix;
    LinkedList *row_lists;
    int i;

    row_lists = (LinkedList*) malloc(n * sizeof(LinkedList));

    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_linked_list;
    sparse_matrix -> free = free_linked_list;
    sparse_matrix -> mult = mult_linked_list;
    sparse_matrix -> private = (LinkedList *)row_lists;

    return sparse_matrix;
}


void add_row_linked_list(struct _spmat *A, const double *row, int i) {
    int j, k, is_empty;
    LinkedList *linked_list = (LinkedList*) (A->private);
    LinkedList *row_node;
    LinkedList *new_node, *prev_node = NULL ;

    row_node = &linked_list[i];
    is_empty = 0;
    for (j = 0; j < A->n; j++) {
        if (row[j] == 0) continue;
        new_node = (LinkedList*)malloc(sizeof(LinkedList));

        new_node->column_index = j;
        new_node->value = row[j];
        new_node->next = NULL;

        if (is_empty == 0){
            is_empty = 1;
            row_node = (LinkedList *) new_node;
            prev_node = new_node;
        }
        else {
            prev_node->next = new_node;
            prev_node = new_node;
        }
    }
    if (is_empty == 0){
        row_node = (LinkedList*)NULL;
    }
}


void add_row_array(struct _spmat *A, const double *row, int i) {
    Array *array_struct = (Array*) A -> private;
    int index, j;

    index = array_struct -> index;
    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0) continue;

        array_struct -> index ++;
        index = array_struct -> index;
        array_struct -> values[index] = row[j];
        array_struct -> colind[index] = j;
        array_struct -> rowptr[index] = i;
    }

    if (i == A -> n) {
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
    LinkedList *node, *tmp;
    LinkedList* linked_list;
    linked_list = (LinkedList*)(A->private);

    for (i = 0; i < A->n; i++) {
        node = &linked_list[i];
        while (node != NULL) {
            tmp = node;
            node = node -> next;
            free(tmp);
        }
        free(node);
    }
}

void mult_array(const struct _spmat *A, const double *v, double *result) {

}


void mult_linked_list(const struct _spmat *A, const double *v, double *result) {
    int i, n;
    LinkedList *linked_list = (LinkedList*) (A->private);
    double sum;
    LinkedList *curr, *next;

    sum = 0;
    n = A -> n;
    curr = &linked_list[0];

    for (i = 0; i < n; i++){
        if (curr != NULL){
            sum += (v[curr -> column_index] * curr -> value);
            next = curr -> next;
            while (next != NULL)
            {
                curr = next;
                sum += (v[curr -> column_index] * curr -> value);
                next = curr -> next;
            }
            result[i] = sum;
        }
    }
}


