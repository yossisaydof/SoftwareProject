#include <stdlib.h>
#include "group.h"

void free_group(group *g) {
    if (g -> size > 0) {
        free(g -> nodes);
    }
    free(g);
}

void reset_group(group *g) {
    g -> nodes = NULL;
    g -> next = NULL;
    g -> size = 0;
}
