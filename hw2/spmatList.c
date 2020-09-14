#include "spmatList.h"

typedef struct LinkedListNode{
    double value;
    int column_index;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedListNode** spmatLinkedList; /*TODO - what is this for? */

void add_row_linked_list(struct _spmat *A, const double *row, int i) {
    int j;

    spmatLinkedList row_list;
    LinkedListNode *pointer_to_node, *new_node, *tmp_node;

    row_list = (spmatLinkedList)(A->private);
    tmp_node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    pointer_to_node = tmp_node;

    for (j = 0; j < A -> n; j++) {
        if (row[j] == 0)
            continue;

        new_node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        pointer_to_node -> next = new_node;
        new_node -> column_index = j;
        new_node -> value = row[j];

        pointer_to_node = new_node;
    }
    pointer_to_node -> next = NULL;
    row_list[i] = tmp_node -> next;

    free(tmp_node);
}


void free_linked_list(struct _spmat *A) {
    int i;
    LinkedListNode *node, *tmp;
    spmatLinkedList row_list;

    row_list = (spmatLinkedList)(A -> private);

    for (i = 0; i < A -> n; i++) {
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

spmat* spmat_allocate_list(int n) {
    spmat *sparse_matrix;
    LinkedListNode *row_lists;

    row_lists = (LinkedListNode*) malloc(n * sizeof(LinkedListNode*));
    if (row_lists == NULL ) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    sparse_matrix = malloc(sizeof(spmat));
    sparse_matrix -> n = n;
    sparse_matrix -> add_row = add_row_linked_list;
    sparse_matrix -> free = free_linked_list;
    sparse_matrix -> mult = mult_linked_list;
    sparse_matrix -> private = (LinkedListNode*) row_lists;

    return sparse_matrix;
}
