#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "equation_solvers.h"

int main() {
	struct polynom pol = (struct polynom){0};
	int ret = 0;

	printf(	"Write coefficients a, b, c "
	       	"separated with spaces so they form "
		"a quadratic equation a*x^2 + b*x + c = 0 :\n");

	pol.nCoeffs = QUADRATIC_COEFFICIENTS_LEN;
	ret = read_coefficients(&pol);

	if (ret) {
		fprintf(stderr, "RTFM\n");
		return EXIT_FAILURE;
	}

	struct polynom_roots roots;
	ret = solve_quadratic(pol, &roots);	
	if (ret) {
		fprintf(stderr, "Something went wrong while solving an equation, exiting\n");
		return EXIT_FAILURE;
	}

	print_roots(roots);
	
	return EXIT_SUCCESS;
}
