#include "stdlib.h"

#include "modularityGroups.h"
#include "divideIntoTwo.h"



void insert(modularityGroups *modularity_groups ,int* nodes, int n) {
    group *new_group, *current;

    new_group = (group*) malloc(sizeof(group*));
    current = (group*) modularity_groups->current;

    new_group->nodes = nodes;
    new_group->size = n;

    if (modularity_groups->number_of_groups == 0) {
        modularity_groups->head = (group*) new_group;
    }
    else {
        current->next = new_group;
    }
    modularity_groups->number_of_groups++;
    modularity_groups->current = (group *) (group *) new_group;
}

group* remove(modularityGroups *modularity_groups) {

}

modularityGroups* allocate_modularity_group(){
    modularityGroups *modularity_groups;

    modularity_groups = (modularityGroups*) malloc(sizeof(modularityGroups*));
    modularity_groups->insert = insert;
    modularity_groups->number_of_groups = 0;
    modularity_groups->current = NULL;
    modularity_groups->head = NULL;
//    modularity_groups->remove

    return modularity_groups;
}
