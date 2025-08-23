#ifndef SOLVER_H
#define SOLVER_H

/**
 * @brief Parses coefficients from stdin
 *
 * Reads \p len coefficients from stdin.
 * Ensures the coefficients are separated by spaces or newlines.
 *
 * @param coeffs An array of \p len elements written with parsed coefficients
 *
 * @param len The length of \p coeffs array and the number of coefficients to parse
 *
 * @return 0 if parsed successfully, -1 in case of an error
 */
int read_coefficients(double coeffs[], int len);

/**
 * @brief Writes found roots to stdout
 *
 * @param roots An array of \p nRoots length with roots of the equation
 *
 * @param nRoots The number of found roots. 
 * If roots number is infinity, pass SQ_INF_ROOTS
 *
 * @return 0 if parsed successfully, -1 in case of an error
 */
int print_roots(const double roots[], int nRoots);

#endif /* SOLVER_H */
