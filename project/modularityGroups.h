/*
 * modularityGroups Summary:
 * This module implements a stack of groups.
 * Each modularityGroup contains groups and the number of groups in it.
 * This structure is used to represent P and O in divideIntoGroups.c.
 *
 * insert_modularity_groups     - Insert a group to the modularity group.
 * remove_modularity_groups     - Remove a group from the modularity group, and return it.
 * free_modularity_groups       - Free modularity group.
 * allocate_modularity_group    - Allocate modularity group.
 */

#ifndef MODULARITYGROUPS_H
#define MODULARITYGROUPS_H

#include "group.h"
#include <stdlib.h>
#include "error_handler.h"
#include <stdio.h>
#include <string.h>

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
