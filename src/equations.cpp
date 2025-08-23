#include "equations.h"
#include <assert.h>
#include <math.h>

static const double DOUBLE_EPS = 1e-9;

static inline int is_zero(double n) {
	return fabs(n) < DOUBLE_EPS;
}

int solve_linear(const double coeffs[LINEAR_COEFFICIENTS_LEN], double *root) {
	assert (coeffs	!= NULL);
	assert (root	!= NULL);

	double	k = coeffs[0],
		b = coeffs[1];

	if (is_zero(k)) {
		if (is_zero(b)) {
			return SQ_INF_ROOTS;
		} else {
			return 0; 
		}
	}

	double x = (-b) / k;
	*root = x;
	return 1;
}

int solve_quadratic(const double coeffs[QUADRATIC_COEFFICIENTS_LEN], 
		    double roots[MAX_QUADRATIC_ROOTS]) {
	assert (coeffs	!= NULL);
	assert (roots	!= NULL);

	double	a = coeffs[0],
		b = coeffs[1],
		c = coeffs[2];

	double discriminant = 0;
	int nRoots = 0;

	if (is_zero(a)) {
		return solve_linear(coeffs + 1, roots);
	}
	
	discriminant = b * b - 4 * a * c;	

	double x1 = 0, x2 = 0;

	if (is_zero(discriminant)) {
		nRoots = 1;

		x1 = -b / (2 * a);
		x2 = x1;
	} else if (discriminant < 0) {
			return 0;
	} else {
		nRoots = 2;

       		discriminant = sqrt(discriminant);
		x1 = (-b - discriminant) / (2 * a);
		x2 = (-b + discriminant) / (2 * a);

		// Return in ascending order
		if (a < 0) {
			double tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
	}
	
	roots[0] = x1;
	roots[1] = x2;
	return nRoots;
}
