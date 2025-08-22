#ifndef SOLVER_H
#define SOLVER_H

const static int SQ_INF_ROOTS = -1;

/**
 * Calculates the solution of a linear equation k * x + b = 0
 *
 * Returns the amount of roots found, 
 * or SQ_INF_ROOTS if the equation has infinity solutions
 */
int solve_linear(const double coeffs[2], double *root);

/**
 * Calculates the solution of a quadratic equation.
 * Uses formula D = b * b - 4 * a * c
 * x1,2 = (-b +- sqrt(D)) / (2 * a)
 *
 * Returns the amount of roots found, 
 * or SQ_INF_ROOTS if the equation has infinity solutions
 */
int solve_quadratic(const double coeffs[3], double roots[2]);


#endif /* SOLVER_H */
