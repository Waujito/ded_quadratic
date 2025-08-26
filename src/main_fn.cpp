#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "argparse.h"
#include "equation_solvers.h"

/*

	   |\---/|
	   | ,_, |
	    \_`_/-..----.
	 ___/ `   ' ,""+ \
	(__...'   __\    |`.___.';
	  (_,...'(_,.`__)/'.....+

*/

enum {
	OPT_HELP
};

static struct option opts[] = {
	{ "help",	OPTIONAL_ARGUMENT,	OPT_HELP	},
	{ 0,		0,		0		},
};


int arg_callback(struct args_context ctx);

int arg_callback(struct args_context ctx) {
	printf("Option %s ", ctx.opt.name);
	if (ctx.value) {
		printf("with value %s", ctx.value);
	}
	printf("\n");

	return 0;
}

/*
int aaa (char* s)             { s++; s = "aa");

int bbb (char  s[])           { s++; s = "aa");
int bbb (char* const  s)      { s++; s = "aa");

int aaa (const char* s)       { s++; s = "aa");

int bbb (const char  s[])     { s++; s = "aa");
int bbb (const char* const s) { s++; s = "aa");
*/


/**
 * @brief Parses coefficients from stdin
 *
 * Ensures the coefficients are separated by spaces or newlines.
 *
 * @param [in, out] pol A pointer to the polynom structure. 
 * The structure should be filled up with nCoeffs so 
 * this function will determine a number of coefficients to be read.
 *
 * @return 0 if parsed successfully, -1 in case of an error
 */
int read_coefficients(struct polynom *pol);

/**
 * @brief Writes found roots to stdout
 *
 * @param roots Polynom roots
 *
 * @return 0 if written successfully, -1 in case of an error
 */
int print_roots(struct polynom_roots roots);

int main(int argc, const char *argv[]) {
	struct polynom pol = (struct polynom){0};
	int ret = 0;

	ret = parse_args(argc, argv, opts, arg_callback, NULL);
	if (ret) {
		printf("What the f... error ^_^ %d\n", ret);
		return EXIT_FAILURE;
	}
	printf("\n");

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
	ret = solve_polynomial(pol, &roots);
	if (ret) {
		fprintf(stderr, "Something went wrong while solving an equation: "
				"A solver exited with exit code %d\n", ret);
		return EXIT_FAILURE;
	}

	print_roots(roots);

	return EXIT_SUCCESS;
}

int read_coefficients(struct polynom *pol) { // TODO several tries
	assert (pol != NULL);
	assert (pol->nCoeffs <= MAX_COEFFICIENTS);

	int c = 0;

	for (int i = 0; i < pol->nCoeffs; i++) {

		int ret = scanf(" %lg", pol->coeffs + i);

		if (errno) {
			fprintf(stderr, "scanf() returned %d: %s",
				ret, strerror (errno));
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
		printf("The equation is valid for every rational, yoo-hoo! x\n");
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
