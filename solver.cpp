#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include "equations.h"
#include "solver.h"

int main() {
	double coeffs[QUADRATIC_COEFFICIENTS_LEN] = {0};
	int ret = 0;

	printf(	"Write coefficients a, b, c "
	       	"separated with spaces so they form "
		"a quadratic equation a*x^2 + b*x + c = 0 :\n");

	ret = read_coefficients(coeffs, QUADRATIC_COEFFICIENTS_LEN);

	if (ret) {
		fprintf(stderr, "RTFM\n");
		return EXIT_FAILURE;
	}

	double roots[MAX_QUADRATIC_ROOTS] = {0};
	int nRoots = solve_quadratic(coeffs, roots);	

	print_roots(roots, nRoots);
	
	return EXIT_SUCCESS;
}

int read_coefficients(double coeffs[], int len) {
	assert (coeffs != NULL);

	int c = 0;

	for (int i = 0; i < len; i++) {
		int ret = scanf("%lg", coeffs + i);
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

	if (c != '\n') {
		return -1;
	}

	return 0;
}

int print_roots(const double roots[], int nRoots) {
	assert (roots != NULL);

	if (nRoots == SQ_INF_ROOTS) {
		printf("The equation is valid for every rational x\n");
		return 0;
	}

	printf("Found a total of %d solutions of the given equation", nRoots);

	if (nRoots != 0) {
		printf(" : ");

		for (int i = 0; i < nRoots; i++) {
			printf("%lg ", roots[i]);
		}
	}
	printf("\n");

	return 0;
}
