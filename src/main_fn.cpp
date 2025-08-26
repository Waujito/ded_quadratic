#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"
#include "io.h"
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
