#include <stdlib.h>
#include "group.h"


void free_group(group *group) {
    if (group -> size) {
        free(group -> nodes);
    }
    free(group);
}
