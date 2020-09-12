#include "matrixStructure.h"
#include "group.h"

#define EPSILON() (0.00001)
#define IS_POSITIVE(X) ((X) > EPSILON())
#define IS_NON_POSITIVE(X) ((X) <= EPSILON())

void improving_division_of_the_network(matrixStructure *matrix_structure, group *g, int *s, double Q_0);
