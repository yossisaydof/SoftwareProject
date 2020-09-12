#ifndef _GROUP_H
#define _GROUP_H

typedef struct _group {
    struct _group *next;
    int *nodes;
    int size; /* group size */
} group;

void free_group(group *group);


/* TODO - add free function */

#endif
