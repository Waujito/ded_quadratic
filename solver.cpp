#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

const double DOUBLE_EPS = 1e-9;
const int SQ_INF_ROOTS = -1;

/**
 * Coefficients for a quadratic equation a * x^2 + b * x + c
 */
struct quadratic_coefficients {
	double a;
	double b;
	double c;
};

/**
 * Coefficients for a linear equation k * x + b
 */
struct linear_coefficients {
	double k;
	double b;
};

static inline int is_zero(double n) {
	return fabs(n) < DOUBLE_EPS;
}

/**
 * Calculates the solution of a quadratic equation.
 * Uses formula D = b * b - 4 * a * c
 * x1,2 = (-b +- sqrt(D)) / (2 * a)
 *
 * Returns the amount of roots found, 
 * or SQ_INF_ROOTS if the equation has infinity solutions
 */
int solve_quadratic(struct quadratic_coefficients coeffs, double roots[2]);

/**
 * Calculates the solution of a linear equation k * x + b = 0
 */
int solve_linear(struct linear_coefficients coeffs, double *root);

int solve_linear(struct linear_coefficients coeffs, double *root) {
	double	k = coeffs.k,
		b = coeffs.b;

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

int solve_quadratic(struct quadratic_coefficients coeffs, double roots[2]) {
	double x1 = 0, x2 = 0;
	double	a = coeffs.a, 
		b = coeffs.b, 
		c = coeffs.c;

	double discriminant = 0;
	int roots_ct = 0;

	if (is_zero(a)) {
		return solve_linear(
			(struct linear_coefficients) { .k = b, .b = c },
			roots
		);
	}
	
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return 0;
	}

	if (is_zero(discriminant)) {
		roots_ct = 1;

		x1 = -b / (2 * a);
		x2 = x1;
	} else {
		roots_ct = 2;

       		discriminant = sqrt(discriminant);
		x1 = (-b - discriminant) / (2 * a);
		x2 = (-b + discriminant) / (2 * a);
	}
	
	roots[0] = x1;
	roots[1] = x2;
	return roots_ct;
}

int main() {
	struct quadratic_coefficients coeffs = {0};
	double roots[2] = {0};
	int nRoots = 0;
	int ret = 0;

	printf(	"Write coefficients a, b, c "
	       	"separated with spaces so they form "
		"a quadratic equation a*x^2 + b*x + c = 0 :\n");

	ret = scanf("%lf%lf%lf", &coeffs.a, &coeffs.b, &coeffs.c);

	if (errno) {
		perror("scanf() failed");
		return EXIT_FAILURE;
	}

	if (ret != 3) {
		fprintf(stderr, "RTFM\n");
		return EXIT_FAILURE;
	}

	nRoots = solve_quadratic(coeffs, roots);
	if (nRoots == SQ_INF_ROOTS) {
		printf("The equation is valid for every rational x\n");
		return EXIT_SUCCESS;
	}

	printf("Found a total of %d solutions of the given equation", nRoots);

	if (nRoots != 0) {
		printf(" : ");

		for (int i = 0; i < nRoots; i++) {
			printf("%lg ", roots[i]);
		}
	}
	printf("\n");
	
	return EXIT_SUCCESS;
}
