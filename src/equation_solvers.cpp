/**
 * @file
 *
 * @brief Equation solvers
 */

#include "equation_solvers.h"
#include <assert.h>
#include <math.h>

static const double DOUBLE_EPS = 1e-9;

inline int is_zero(double n) {
	return fabs(n) < DOUBLE_EPS;
}

int solve_linear(struct polynom pol, struct polynom_roots *roots) {
	assert (roots		!= NULL);
	assert (pol.nCoeffs	== 2);

	double	k = pol.coeffs[0],
		b = pol.coeffs[1];

	if (is_zero(k)) {
		if (is_zero(b)) {
			roots->nRoots = SQ_INF_ROOTS;
		} else {
			roots->nRoots = 0;
		}

		return 0; 
	}

	double x = (-b) / k;

	roots->nRoots = 1;
	roots->roots[0] = x;

	return 0;
}

int solve_quadratic(struct polynom pol, struct polynom_roots *roots) {
	assert (roots		!= NULL);
	assert (pol.nCoeffs	== 3);

	double	a = pol.coeffs[0],
		b = pol.coeffs[1],
		c = pol.coeffs[2];

	double discriminant = 0;

	if (is_zero(a)) {
		struct polynom linear_pol = {
			.coeffs = { pol.coeffs[1], pol.coeffs[2] },
			.nCoeffs = 2
		};

		return solve_linear(linear_pol, roots);
	}
	
	discriminant = b * b - 4 * a * c;	

	double x1 = 0, x2 = 0;

	if (is_zero(discriminant)) {
		roots->nRoots = 1;

		x1 = -b / (2 * a);
		x2 = x1;
	} else if (discriminant < 0) {
		roots->nRoots = 0;
	} else {
		roots->nRoots = 2;

       		discriminant = sqrt(discriminant);
		x1 = (-b - discriminant) / (2 * a);
		x2 = (-b + discriminant) / (2 * a);

		// Return in ascending order
		if (a < 0) { // TODO vectorized swap
			double tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
	}
	
	roots->roots[0] = x1;
	roots->roots[1] = x2;

	return 0;
}
