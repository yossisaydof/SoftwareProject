
#include "powerIteration.c"
#include "matrixStructure.c"

/**
 /Algorithm 2
*/

void divideIntoTwo(matrixStructure matrix_structure, B_hat_g) {
    int n;
    double eigen_value, calc;

    n = matrix_structure.n;
    // malloc to u1, send it to power_iteration
    eigen_value = power_iteration(B_hat_g, n, u1); // B(beta)

    if (eigen_value <= 0)
        return;

    // Compute s = {s1,....,sn} where si in {+1, -1}, according to u1

    // calc = s(Transpose) * B_hat_g * s
    if (calc <= 0)
        return;



}

