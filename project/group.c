#include <stdlib.h>
#include "group.h"


void free_group(group *group) {
    free(group -> nodes);
    free(group);
}
