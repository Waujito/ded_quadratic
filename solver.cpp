#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

/**
 * Reads n double coefficients from stdin.
 * Ensures the coefficients are separated by spaces or new lines
 */
int read_coefficients(double coeffs[], int len);

/**
 * Calculates the solution of a linear equation k * x + b = 0
 */
int solve_linear(double coeffs[2], double *root);

/**
 * Calculates the solution of a quadratic equation.
 * Uses formula D = b * b - 4 * a * c
 * x1,2 = (-b +- sqrt(D)) / (2 * a)
 *
 * Returns the amount of roots found, 
 * or SQ_INF_ROOTS if the equation has infinity solutions
 */
int solve_quadratic(double coeffs[3], double roots[2]);

const double DOUBLE_EPS = 1e-9;
const int SQ_INF_ROOTS = -1;

static inline int is_zero(double n) {
	return fabs(n) < DOUBLE_EPS;
}

int solve_linear(double coeffs[2], double *root) {
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

int solve_quadratic(double coeffs[3], double roots[2]) {
	double x1 = 0, x2 = 0;
	double	a = coeffs[0],
		b = coeffs[1],
		c = coeffs[2];

	double discriminant = 0;
	int roots_ct = 0;

	if (is_zero(a)) {
		return solve_linear(coeffs + 1, roots);
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

int read_coefficients(double coeffs[], int len) {
	int ret = 0;
	char c = 0;

	for (int i = 0; i < len; i++) {
		ret = scanf("%lg", coeffs + i);
		if (errno) {
			perror("scanf() failed");
			return -1;
		}

		if (ret != 1) {
			return -1;
		}

		c = getchar();
		if (c != ' ' && c != '\n') {
			return -1;
		}
	}

	return 0;
}

int main() {
	double coeffs[3] = {0};
	double roots[2] = {0};
	int nRoots = 0;
	int ret = 0;

	printf(	"Write coefficients a, b, c "
	       	"separated with spaces so they form "
		"a quadratic equation a*x^2 + b*x + c = 0 :\n");

	ret = read_coefficients(coeffs, 3);

	if (ret) {
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
