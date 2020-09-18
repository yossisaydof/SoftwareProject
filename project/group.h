
/* group Summary:
 *
 * This module implement nodes of the modularityGroups.
 * Each group contains an array of nodes that exist in the group, its size and a pointer to the.
 * next group in the modularityGroup.
 *
 * free_group   - free group g.
 */

#ifndef GROUP_H
#define GROUP_H

typedef struct _group {
    struct _group *next;
    int *nodes;
    int size;
} group;

void free_group(group *g);
void reset_group(group *g);

#endif
