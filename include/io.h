/**
 * @file
 *
 * @brief Declaration of IO functions for polynom coefficients
 */

#ifndef EQ_IO_H
#define EQ_IO_H
#include "equation_solvers.h"

/**
 * @brief Parses coefficients from stdin
 *
 * Ensures the coefficients are separated by spaces or newlines.
 *
 * @param [in, out] pol A pointer to the polynom structure. 
 * The structure should be filled up with nCoeffs so 
 * this function will determine a number of coefficients to be read.
 *
 * @return 0 if parsed successfully, -1 in case of an error
 */
int read_coefficients(struct polynom *pol);

/**
 * @brief Writes found roots to stdout
 *
 * @param roots Polynom roots
 *
 * @return 0 if written successfully, -1 in case of an error
 */
int print_roots(struct polynom_roots roots);

#endif /* EQ_IO_H */
