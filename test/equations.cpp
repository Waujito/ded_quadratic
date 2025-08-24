#include "equation_solvers.h"
#include <math.h>

#ifdef USE_GTEST

#include <gtest/gtest.h>

#else /* USE_GTEST */

#include "test_machine.h"

#endif /* USE_GTEST */

void quadratic_test(double a, double b, double c, int nRoots, double x1, double x2);

void quadratic_test(double a, double b, double c, int nRoots, double x1, double x2) {
	struct polynom pol = {
		.nCoeffs = 3,
		.coeffs = { a, b, c },
	};
	struct polynom_roots roots;

	int ret = solve_quadratic(pol, &roots);

	ASSERT_EQ(ret, 0);

	ASSERT_EQ(roots.nRoots, nRoots);
	if (roots.nRoots >= 1) {
		ASSERT_DOUBLE_EQ(roots.roots[0], x1);
	}
	if (roots.nRoots >= 2) {
		ASSERT_DOUBLE_EQ(roots.roots[1], x2);
	}
}


TEST(EqTest, NormalEquation)		{ quadratic_test(1, -5, 6, 2, 2, 4); }

TEST(EqTest, NegativeCoefficients)	{ quadratic_test(2, 3, -2, 1, -2, 0.5); }

TEST(EqTest, DoubleRoot)		{ quadratic_test(1, -6, 9, 1, 3, NAN);	}

TEST(EqTest, NoRationalSolutions)	{ quadratic_test(1, 4, 5, 0, NAN, NAN); }

TEST(EqTest, Zero_b)			{ quadratic_test(1, 0, -9, 2, -3, 3); }

TEST(EqTest, Zero_c)			{ quadratic_test(1, 5, 0, 2, -5, 0); }

TEST(EqTest, Zero_bc)			{ quadratic_test(4, 0, 0, 1, 0, NAN); }

TEST(EqTest, Large_coeffs)		{ quadratic_test(1e6, -4e6, 3e6, 2, 1, 3); }

TEST(EqTest, Negative_a)		{ quadratic_test(-1, 1, 2, 2, -1, 2); }

TEST(EqTest, Linear_eq)			{ quadratic_test(0, 2, -4, 1, 2, NAN); }

TEST(EqTest, Linear_no_solution)	{ quadratic_test(0, 0, 7, 0, NAN, NAN); }

TEST(EqTest, Linear_infinity_solutions)	{ quadratic_test(0, 0, 0, SQ_INF_ROOTS, NAN, NAN); }
