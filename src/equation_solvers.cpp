/**
 * @file
 *
 * @brief Equation solvers
 */

#include "equation_solvers.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// static const double DOUBLE_EPS = 1e-9;
#define DOUBLE_EPS (1e-9)

static inline int is_zero(double n) {
	return fabs(n) < DOUBLE_EPS;
}

static int vectorized_swap(char *p1, char *p2, size_t sz) {
	size_t processed_sz = 0;

	while (processed_sz + sizeof(uint64_t) <= sz) {
		uint64_t dt1 = *(uint64_t *)p1;
		*(uint64_t *)p1 = *(uint64_t *)p2;
		*(uint64_t *)p2 = dt1;
		processed_sz += sizeof(uint64_t);
		p1 += sizeof(uint64_t);
		p2 += sizeof(uint64_t);
	}

	while (processed_sz + sizeof(char) <= sz) {
		char dt1 = *p1;
		*p1 = *p2;
		*p2 = dt1;
		processed_sz++;
		p1++, p2++;
	}

	return 0;
}

enum solving_status solve_polynomial(struct polynom pol, struct polynom_roots *roots) {
	assert (roots != NULL);
	assert (pol.nCoeffs >= 0 && pol.nCoeffs <= MAX_COEFFICIENTS);

	if (pol.nCoeffs == 0) {
		roots->nRoots = SQ_INF_ROOTS;
		return SOLVING_SUCCESS;
	}

	switch (pol.nCoeffs) {
		case 0:
			roots->nRoots = SQ_INF_ROOTS;
			return SOLVING_SUCCESS;
		case 1:
			roots->nRoots = (is_zero(pol.coeffs[0]) ? SQ_INF_ROOTS : 0);
			return SOLVING_SUCCESS;
		case 2:
			return solve_linear(pol, roots);
		case 3:
			return solve_quadratic(pol, roots);
		default:
			return SOLVING_NOT_IMPLEMENTED;
	}

	return SOLVING_ERROR;
}

enum solving_status solve_linear(struct polynom pol, struct polynom_roots *roots) {
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

		return SOLVING_SUCCESS; 
	}

	double x = (-b) / k;

	roots->nRoots = 1;
	roots->roots[0] = x;

	return SOLVING_SUCCESS;
}

enum solving_status solve_quadratic(struct polynom pol, struct polynom_roots *roots) {
	assert (roots		!= NULL);
	assert (pol.nCoeffs	== 3);

	double	a = pol.coeffs[0],
		b = pol.coeffs[1],
		c = pol.coeffs[2];

	double discriminant = 0;

	if (is_zero(a)) {
		struct polynom linear_pol = {
			.nCoeffs = 2,
			.coeffs = { pol.coeffs[1], pol.coeffs[2] },
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
		if (a < 0) {
			vectorized_swap((char *)&x1, (char *)&x2, sizeof(x1));
		}
	}
	
	roots->roots[0] = x1;
	roots->roots[1] = x2;

	return SOLVING_SUCCESS;
}

