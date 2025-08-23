#include "equation_solvers.h"
#include <math.h>
#include <gtest/gtest.h>

void assertion_check(double a, double b, double c, int nRoots, double x1, double x2);

void assertion_check(double a, double b, double c, int nRoots, double x1, double x2) {
	struct polynom pol = {
		.coeffs = { a, b, c },
		.nCoeffs = 3,
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


TEST(EqTest, TwoRealRoots)
{
	assertion_check(1, -5, 6, 2, 2, 3);
}

TEST(EqTest, NegativeCoefficients)
{
	assertion_check(2, 3, -2, 2, -2, 0.5);
}

TEST(EqTest, DoubleRoot)
{
	assertion_check(1, -6, 9, 1, 3, NAN);
}

TEST(EqTest, NoRationalSolutions)
{
	assertion_check(1, 4, 5, 0, NAN, NAN);
}

TEST(EqTest, Zero_b)
{
	assertion_check(1, 0, -9, 2, -3, 3);
}

TEST(EqTest, Zero_c)
{
	assertion_check(1, 5, 0, 2, -5, 0);
}

TEST(EqTest, Zero_bc)
{
	assertion_check(4, 0, 0, 1, 0, NAN);
}

TEST(EqTest, Large_coeffs)
{
	assertion_check(1e6, -4e6, 3e6, 2, 1, 3);
}

TEST(EqTest, Negative_a)
{
	assertion_check(-1, 1, 2, 2, -1, 2);
}

TEST(EqTest, Linear_eq)
{
	assertion_check(0, 2, -4, 1, 2, NAN);
}

TEST(EqTest, Linear_no_solution)
{
	assertion_check(0, 0, 7, 0, NAN, NAN);
}

TEST(EqTest, Linear_infinity_solutions)
{
	assertion_check(0, 0, 0, SQ_INF_ROOTS, NAN, NAN);
}
