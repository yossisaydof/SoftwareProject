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

typedef struct {
    double *values;
    int *colind;
    int *rowptr;
    int index;
} Array;


typedef struct LinkedList{
    struct LinkedList *head;
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
    LinkedList *list_of_rows;
    int i;
    list_of_rows = (LinkedList*) malloc(n * sizeof(LinkedList));
//    for (i = 0; i < n; i++) {
//        list_of_rows[i] = NULL;
//    }

    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_linked_list;
    sparse_matrix -> free = free_linked_list;
    sparse_matrix -> mult = mult_linked_list;
    sparse_matrix -> private = (LinkedList *)list_of_rows;

    return sparse_matrix;
}


void add_row_linked_list(struct _spmat *A, const double *row, int i) {
    int j, k, is_empty;
    LinkedList *linked_list = (LinkedList*)(A->private);
    LinkedList *list_node;
    LinkedList *new_node, *prev_node = NULL ;

    list_node = &linked_list[i];

    is_empty = 0;
    for (j = 0; j < A->n; j++) {
        if (row[j] == 0) continue;
        new_node = (LinkedList*)malloc(sizeof(LinkedList));

        new_node->column_index = j;
        new_node->value = row[j];
        new_node->next = NULL;

        if (is_empty == 0){
            is_empty = 1;
            list_node->head =new_node;
            list_node = (LinkedList *) new_node;
        } else {
            prev_node = list_node;
            list_node->next = new_node;
            prev_node = list_node;

            list_node += sizeof(LinkedList);
//            linked_list = linked_list->next;
        }
    }
    if (is_empty == 0){
        list_node = (LinkedList*)NULL;
    }
}

void print(struct _spmat *A, int n) {
    LinkedList *linked_list = (LinkedList*)(A->private);
    LinkedList *head;
    int i, j;
    for (i = 0; i < n; i++) {
        head = &linked_list[i];
        while(head->next != NULL) {
            printf("%lf", head->value);
            head += sizeof(LinkedList);
        }

    }
}


void free_linked_list(struct _spmat *A) {
    int i;
    LinkedList* node, tmp;
    LinkedList* ls;
    ls = (LinkedList*) A->private;

//    for (i = 0; i < A->n; i++) {
//        node = &ls->row_list[i];
//        while (node != NULL) {
//            tmp = node->next;
//        }
//        free(node);
//    }
}


void mult_linked_list(const struct _spmat *A, const double *v, double *result) {}


void add_row_array(struct _spmat *A, const double *row, int i) {
    Array *array_struct = A -> private;
    int index;
    int j;

    index = 0;
    for (j = 0; j < A -> n ; j++) {
        if (row[j] == 0) continue ;

        array_struct -> index ++;
        index = array_struct -> index;
        array_struct -> values[index] = row[j];
        array_struct -> colind[index] = j;
        array_struct -> rowptr[index] = i;
    }
    if (index == 0) {

    }
    if (i == A -> n) {
        if (index == 0) {
            array_struct -> rowptr[array_struct -> index] = array_struct -> index + 1;
        }
        array_struct -> rowptr[(array_struct -> index) + 1] = (array_struct -> index) + 1;

    }
}


void free_array(struct _spmat *A) {}


void mult_array(const struct _spmat *A, const double *v, double *result) {}
