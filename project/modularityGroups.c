#include "modularityGroups.h"

void insert_modularity_groups(modularityGroups *modularity_groups , int* nodes, int n) {
    group *new_group;

    new_group = (group*) malloc(sizeof(group));
    if (new_group == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    new_group -> nodes = nodes;
    new_group -> size = n;
    new_group -> next = NULL;

    if (modularity_groups -> number_of_groups == 0) {
        modularity_groups -> head = new_group;
    }
    else {
        new_group -> next = modularity_groups -> head;
        modularity_groups -> head = new_group;
    }
    modularity_groups -> number_of_groups++;
}

group* remove_modularity_groups(modularityGroups *modularity_groups) {
    group *tmp;

    tmp = modularity_groups -> head;
    modularity_groups -> head = tmp -> next;
    modularity_groups -> number_of_groups--;

    return tmp;
}

void free_modularity_groups(modularityGroups *modularity_groups) {
    int i;
    group *tmp, *current;

    for (i = 0; i < modularity_groups -> number_of_groups; i++) {
        /* free all groups in modularity_groups */
        current = modularity_groups -> head;
        tmp = current -> next;
        free_group(current);
        modularity_groups -> head = tmp;
    }

    free(modularity_groups);
}

modularityGroups* allocate_modularity_group(){
    modularityGroups *modularity_groups;

    modularity_groups = (modularityGroups*) malloc(sizeof(modularityGroups));
    if (modularity_groups == NULL) {
        printf("%s", MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    modularity_groups -> number_of_groups = 0;
    modularity_groups -> head = NULL;

    modularity_groups -> insert = insert_modularity_groups;
    modularity_groups -> remove = remove_modularity_groups;
    modularity_groups -> free = free_modularity_groups;

    return modularity_groups;
}
