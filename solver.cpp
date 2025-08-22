#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "equations.h"

/**
 * Reads n double coefficients from stdin.
 * Ensures the coefficients are separated by spaces or new lines
 */
int read_coefficients(double coeffs[], int len);

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

int read_coefficients(double coeffs[], int len) {
	int ret = 0;
	int c = 0;

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

	if (c != '\n') {
		return -1;
	}

	return 0;
}
