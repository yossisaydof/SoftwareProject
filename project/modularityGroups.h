

#include <mach/machine.h>
#include "group.h"



// P and O for algorithm 3
typedef struct _modularityGroups { // linked list
    struct modularityGroups *next;
    struct group *value;

    boolean_t   (*is_empty)();
    void    (*insert)(group* g); // insert to the end of the list
    group*    (*remove)(); // remove the first group
    void    (*free)(); // free memory
} modularityGroups;


modularityGroups* allocate_modularity_group(int n);
