#ifndef _MODULARITYGROUPS_H
#define _MODULARITYGROUPS_H

#include "group.h"

/* P and O for algorithm 3 */
typedef struct _modularityGroups {
    group *head;
    int number_of_groups;

    void       (*insert)(struct _modularityGroups *modularity_groups,int* nodes, int n); /* insert_modularity_groups group to the end of the list, |g| = n */
    group*     (*remove)(struct _modularityGroups *modularity_groups); /* remove_modularity_groups the first group */
    void       (*free)(struct _modularityGroups *modularity_groups); /* free memory */
} modularityGroups;


modularityGroups* allocate_modularity_group();

#endif
