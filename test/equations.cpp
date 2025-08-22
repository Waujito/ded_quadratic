#include "equations.h"
#include <gtest/gtest.h>


TEST(EqTest, TwoRealRoots)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1, -5, 6};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], 2);
	ASSERT_DOUBLE_EQ(roots[1], 3);
}

TEST(EqTest, NegativeCoefficients)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {2, 3, -2};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], -2);
	ASSERT_DOUBLE_EQ(roots[1], 0.5);
}

TEST(EqTest, DoubleRoot)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1, -6, 9};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 1);
	ASSERT_DOUBLE_EQ(roots[0], 3);
}

TEST(EqTest, NoRationalSolutions)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1, 4, 5};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 0);
}

TEST(EqTest, Zero_b)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1, 0, -9};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], -3);
	ASSERT_DOUBLE_EQ(roots[1], 3);
}

TEST(EqTest, Zero_c)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1, 5, 0};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], -5);
	ASSERT_DOUBLE_EQ(roots[1], 0);
}

TEST(EqTest, Zero_bc)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {4, 0, 0};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 1);
	ASSERT_DOUBLE_EQ(roots[0], 0);
}

TEST(EqTest, Large_coeffs)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {1e6, -4e6, 3e6};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], 1);
	ASSERT_DOUBLE_EQ(roots[1], 3);
}

TEST(EqTest, Negative_a)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {-1, 1, 2};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 2);
	ASSERT_DOUBLE_EQ(roots[0], -1);
	ASSERT_DOUBLE_EQ(roots[1], 2);
}

TEST(EqTest, Linear_eq)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {0, 2, -4};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 1);
	ASSERT_DOUBLE_EQ(roots[0], 2);
}

TEST(EqTest, Linear_no_solution)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {0, 0, 7};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, 0);
}

TEST(EqTest, Linear_infinity_solutions)
{
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {0, 0, 0};
	double roots[MAX_QUADRATIC_ROOTS] = {0};

	int nRoots = solve_quadratic(coeffs, roots);	

	ASSERT_EQ(nRoots, SQ_INF_ROOTS);
}
