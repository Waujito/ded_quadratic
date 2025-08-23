/**
 * @file
 *
 * @brief Declarations of equation solvers
 */

#ifndef EQUATIONS_H
#define EQUATIONS_H

const static int SQ_INF_ROOTS = -1;

const static int LINEAR_COEFFICIENTS_LEN = 2;
const static int QUADRATIC_COEFFICIENTS_LEN = 3;

const static int MAX_COEFFICIENTS = 10;
const static int MAX_ROOTS = 10;

struct polynom {
	double coeffs[MAX_COEFFICIENTS];
	int nCoeffs;	
};

struct polynom_roots {
	int nRoots;
	double roots[MAX_ROOTS];
};

/**
 * @brief Solves the linear equation \f$kx + b = 0\f$.
 *
 * @param [in] pol A polynom of two coefficients \f$k\f$ and \f$b\f$
 * @param [out] roots A pointer to polynom_roots being filled. nRoots is the number of roots found, or SQ_INF_ROOTS if the equation has infinitely many solutions
 *
 * @return 0 on successful operation, -1 in case of an error
 */
int solve_linear(struct polynom pol, struct polynom_roots *roots);

/**
 * @brief Solves a quadratic equation \f$ ax^2 + bx + c = 0 \f$.
 * 
 * Calculates the discriminant using the formula:
 * \f[ D = b^2 - 4ac \f]
 * 
 * Then calculates the roots:
 * \f[ x_{1,2} = \frac{-b \pm \sqrt{D}}{2a} \f]
 * 
 * @param [in] pol A polynom of three coefficients \f$a\f$, \f$b\f$ and \f$c\f$
 * @param [out] roots A pointer to polynom_roots being filled. nRoots is the number of roots found, or SQ_INF_ROOTS if the equation has infinitely many solutions
 *
 * @return 0 on successful operation, -1 in case of an error
 *
 * @note If \f$a\f$ is zero, although it is not a quadratic equation, the function will solve it as linear.
 */
int solve_quadratic(struct polynom pol, struct polynom_roots *roots);

#endif /* EQUATIONS_H */
