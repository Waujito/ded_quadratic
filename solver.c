#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

const double DOUBLE_EPS = 1e-9;

struct quadratic_coefficients {
	double a;
	double b;
	double c;
};

/**
 * Calculates the solution for a quadratic equation.
 * Using formula D = b * b - 4 * a * c
 * x1,2 = (-b +- sqrt(D)) / (2 * a)
 *
 * Returns the amount of roots found or -EINVAL in case of a validation error 
 */
int solve_quadratic(struct quadratic_coefficients coeffs, double roots[2]);

int solve_quadratic(struct quadratic_coefficients coeffs, double roots[2]) {
	double x1, x2;
	double a, b, c;
	double discriminant;
	int roots_ct = 0;

	errno = 0;

	a = coeffs.a;
	b = coeffs.b;
	c = coeffs.c;

	if (fabs(a) < DOUBLE_EPS) {
		errno = EINVAL;
		return -errno;
	}

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return 0;
	}

	if (discriminant < DOUBLE_EPS) {
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
	struct quadratic_coefficients coeffs;
	double roots[2];
	int roots_ct;
	int ret;

	printf(	"Write coefficients a, b, c "
	       	"separated with spaces so they form "
		"a quadratic equation a*x^2 + b*x + c = 0 :\n");

	ret = scanf("%lf%lf%lf", &coeffs.a, &coeffs.b, &coeffs.c);

	if (ret != 3) {
		errno = EINVAL;
	}

	if (errno) {
		perror("Invalid input");
		return EXIT_FAILURE;
	}

	roots_ct = solve_quadratic(coeffs, roots);
	if (roots_ct < 0) {
		perror("Quadratic equation solving error");
		return EXIT_FAILURE;
	}

	printf("Found a total of %d solutions for the given equation", roots_ct);

	if (roots_ct != 0) {
		printf(" : ");

		for (int i = 0; i < roots_ct; i++) {
			printf("%g ", roots[i]);
		}
	}
	printf("\n");
	

	return EXIT_SUCCESS;
}
