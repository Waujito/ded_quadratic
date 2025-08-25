#include <stdio.h>
#include <assert.h>
#include "test_machine.h"
#include <setjmp.h>
#include <stdlib.h>

struct test_unit {
	test_fn_t fun_ptr;
	const char *fname;
	const char *test_group;
	const char *test_name;
	int line;
};

struct tests_vector {
	struct test_unit *tests;
	size_t tests_capacity;
	size_t tests_len;
} tests_vector = {0};

int tests_add_entry(struct test_unit *test);
int test_runner(struct test_unit *test);

int main() {
	int ret = 0;

	__TM_PRINT_DEBUG("In main() \n");

	int passed_tests = 0;
	int failed_tests = 0;

	for (size_t i = 0; i < tests_vector.tests_len; i++) {
		struct test_unit *test_unit = &tests_vector.tests[i];
		ret = test_runner(test_unit);

		if (ret == 0) {
			passed_tests++;
		} else if (ret == 1) {
			failed_tests++;
		} else {
			printf("Internal error\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("\n");
	printf("passed tests: %d; failed tests: %d\n", passed_tests, failed_tests);

	if (failed_tests != 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int tests_add_entry(struct test_unit *test) {
	__TM_PRINT_DEBUG("cap %zu len %zu\n", tests_capacity, tests_len);
	if (tests_vector.tests_len >= tests_vector.tests_capacity) {
		size_t new_capacity = tests_vector.tests_capacity * 2;
		if (new_capacity == 0) {
			new_capacity = 4;
		}

		struct test_unit *ntests = (struct test_unit *) realloc(
			tests_vector.tests,
			new_capacity * sizeof(struct test_unit)
		);

		if (!ntests) {
			return -1;
		}

		tests_vector.tests = ntests;
		tests_vector.tests_capacity = new_capacity;
	}

	tests_vector.tests[tests_vector.tests_len++] = *test;

	return 0;
}

test_fn_t __tm_add_test(test_fn_t test,
			const char *fname, int line,
			const char *test_group, const char *test_name) {
	struct test_unit test_unit = {
		.fun_ptr	= test,
		.fname		= fname,
		.test_group	= test_group,
		.test_name	= test_name,
		.line		= line
	};

	int ret = tests_add_entry(&test_unit);
	if (ret) {
		exit(EXIT_FAILURE);
	}

	__TM_PRINT_DEBUG("Adding test from file %s and line %d\n", fname, line);
	return test;
}

static jmp_buf __tm_jmp_point;

void __tm_assert_fail_exit(void) {
	longjmp(__tm_jmp_point, 1);
}

int test_runner(struct test_unit *test) {
	__TM_PRINT_DEBUG(
		"Running test %s.%s with function ptr=%p from file %s on line %d\n",
		test.test_group, test.test_name,
		test.fun_ptr, test.fname, test.line);

	int failed = 0;

	if (!setjmp(__tm_jmp_point)) {
		test->fun_ptr();
	} else {
		// If longjmp was called, the program will jump here
		failed = 1;
	}
	// If longjmp was not called, the program will omit failed = 1

	// TODO: wrap with defines
	if (failed) {
		printf(	COLOR_RED
			"[%s.%s][FAIL]"
			COLOR_CLEAR
			"\n\n",
			test->test_group, test->test_name
		);
	} else {
		printf(	COLOR_GREEN
			"[%s.%s][PASS]"
			COLOR_CLEAR "\n",
			test->test_group, test->test_name
		);
	}

	__TM_PRINT_DEBUG("Test finished\n\n");

	return failed;
}
