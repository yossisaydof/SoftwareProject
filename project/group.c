#include <stdlib.h>
#include "group.h"

void free_group(group *g) {
    if (g -> size > 0) {
        free(g -> nodes);
    }
    free(g);
}
