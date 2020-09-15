#include <stdlib.h>
#include "group.h"

void free_group(group *group) {
    if (group -> size > 0) {
        free(group -> nodes);
    }
    free(group);
}
