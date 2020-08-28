#include "spmatList.h"
#include <stdlib.h>
#include <stdio.h>

spmat* spmat_allocate_list(int n);
void add_row_linked_list(struct _spmat *A, const double *row, int i);
void free_linked_list(struct _spmat *A);
void mult_linked_list(const struct _spmat *A, const double *v, double *result);

typedef struct LinkedListNode** spmatLinkedList;

spmat* spmat_allocate_list(int n) {
    spmat *sparse_matrix;

    sparse_matrix = malloc(sizeof(spmat));

    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_linked_list;
    sparse_matrix -> free = free_linked_list;
    sparse_matrix -> mult = mult_linked_list;

    return sparse_matrix;
}

void add_row_linked_list(struct _spmat *A, const double *row, int i) {
    int j;
    LinkedListNode *pointer_to_node, *new_node, *tmp_node;

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
    A[i] = tmp_node->next;

    free(tmp_node);
}

void free_linked_list(struct _spmat *A) {
    int i;
    LinkedListNode *node, *tmp;

    for (i = 0; i < A->n; i++) {
        node = A[i];
        while (node != NULL) {
            tmp = node;
            node = node -> next;
            free(tmp);
        }
    }

    free(A);
}

void mult_linked_list(const struct _spmat *A, const double *v, double *result) {
    int i;
    double sum;
    LinkedListNode *pointer;

    for (i = 0; i < A -> n; i++){
        pointer = A[i];
        sum = 0;
        while (pointer != NULL) {
            sum += (v[pointer -> column_index] * pointer -> value);
            pointer = pointer -> next;
        }
        result[i] = sum;
    }
}
