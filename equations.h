#ifndef SOLVER_H
#define SOLVER_H

const static int SQ_INF_ROOTS = -1;

const static int LINEAR_COEFFICIENTS_LEN = 2;
const static int QUADRATIC_COEFFICIENTS_LEN = 3;
const static int MAX_QUADRATIC_ROOTS = 2;

/**
 * @brief Solves the linear equation \f$kx + b = 0\f$.
 *
 * @param coeffs Array of two coefficients \f$k\f$ and \f$b\f$
 * @param root A pointer to the found root
 * @return The number of roots found, or SQ_INF_ROOTS if the equation has infinitely many solutions
 */
int solve_linear(const double coeffs[LINEAR_COEFFICIENTS_LEN], double *root);

/**
 * @brief Solves a quadratic equation \f$ ax^2 + bx + c = 0 \f$.
 * 
 * Calculates the discriminant using the formula:
 * \f[ D = b^2 - 4ac \f]
 * 
 * Then calculates the roots:
 * \f[ x_{1,2} = \frac{-b \pm \sqrt{D}}{2a} \f]
 * 
 * @param[in] coeffs Array of three coefficients \f$a\f$, \f$b\f$ and \f$c\f$
 * @param[out] roots Two-element array where the found roots will be stored.
 * @return Number of real roots found, or SQ_INF_ROOTS if here are an infinite number of solutions. 
 * @note If \f$a\f$ is zero, although it is not a quadratic equation, the function will solve it as linear.
 */
int solve_quadratic(const double coeffs[QUADRATIC_COEFFICIENTS_LEN], 
		    double roots[MAX_QUADRATIC_ROOTS]);

#endif /* SOLVER_H */
