/**
 * @file
 *
 * @brief IO functions for polynom coefficients
 */

#include "io.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int read_coefficients(struct polynom *pol) { // TODO several tries
	assert (pol != NULL);
	assert (pol->nCoeffs <= MAX_COEFFICIENTS);

	int c = 0;

	for (int i = 0; i < pol->nCoeffs; i++) {
		int ret = scanf("%lg", pol->coeffs + i);
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

int print_roots(struct polynom_roots roots) {
	assert (roots.nRoots <= MAX_ROOTS);

	if (roots.nRoots == SQ_INF_ROOTS) {
		printf("The equation is valid for every rational x\n");
		return 0;
	}

	printf("Found a total of %d solutions of the given equation", roots.nRoots);

	if (roots.nRoots != 0) {
		printf(" : ");

		for (int i = 0; i < roots.nRoots; i++) {
			printf("%lg ", roots.roots[i]);
		}
	}
	printf("\n");

	return 0;
}
