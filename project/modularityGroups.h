#include "group.h"

// P and O for algorithm 3
typedef struct _modularityGroups { // linked list
    group *head;
    group *current;
    int number_of_groups;

    void       (*insert)(struct _modularityGroups *modularity_groups,int* nodes, int n); // insert group to the end of the list, |g| = n
    group*     (*remove)(); // remove the first group
    void       (*free)(); // free memory
} modularityGroups;


modularityGroups* allocate_modularity_group();
