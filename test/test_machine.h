/**
 * @file
 *
 * @brief A Test Framework
 */

#ifndef TEST_MACHINE_H
#define TEST_MACHINE_H

#include <math.h>
#include <stdio.h>
#include "colors.h"

#ifdef __TM_TEST_DEBUG
#define __TM_PRINT_DEBUG(...) printf(__VA_ARGS__)
#else /* __TM_TEST_DEBUG */
#define __TM_PRINT_DEBUG(...) (void)0
#endif /* __TM_TEST_DEBUG */

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

typedef void (*test_fn_t)(void);

#define __TM_TEST_FN_NAME(test_group, test_name)		\
	__tm_##test_group_##test_name##_fn			\

#define __TM_TEST_FN(test_group, test_name)			\
	void __TM_TEST_FN_NAME(test_group, test_name)(void)	\

#define __TM_TEST_DUMMY(test_group, test_name)			\
	const test_fn_t __tm__##test_group##_##test_name##_dummy

/**
 * @brief Registers the test within the global test tracker
 */
test_fn_t __tm_add_test(test_fn_t test,
			const char *fname, int line,
			const char *test_group, const char *test_name);


/**
 * @brief Collects tests before main() function run.
 *
 * Here are two ways to perform this:
 * - Use \_\_attribute\_\_((constructor)) which is a GCC extension.
 *   This way is good, but is not portable between compilers
 * - Use C++ style dummy global variable which initializes with a function.
 *   Such initialization in C++ calls the function before main().
 */
#if __has_attribute(constructor)
#define __TM_TEST_CONSTRUCTOR(test_group, test_name)		\
__attribute__((constructor))					\
static void __tm__##test_group##_##test_name##_constructor() {	\
	__tm_add_test(						\
		__TM_TEST_FN_NAME(test_group, test_name),	\
		__FILE__, __LINE__,				\
		#test_group, #test_name				\
	);							\
}
#else /* __has_attribute(constructor) */
#define __TM_TEST_CONSTRUCTOR(test_group, test_name)		\
	__TM_TEST_DUMMY(test_group, test_name) = __tm_add_test(	\
		__TM_TEST_FN_NAME(test_group, test_name),	\
		__FILE__, __LINE__,				\
		#test_group, #test_name				\
	);
#endif /* __has_attribute(constructor) */

/**
 * @brief A Test entry
 *
 * Declares and registers the test.
 *
 * Usage:
 * @code
 * 	TEST(MyTestGroup, MyTestName) {
 * 		ASSERT_EQ (2 * 2, 5);
 * 	}
 * @endcode
 */
#define TEST(test_group, test_name)				\
	__TM_TEST_FN(test_group, test_name);			\
	__TM_TEST_CONSTRUCTOR(test_group, test_name);		\
	__TM_TEST_FN(test_group, test_name)			\

void __tm_assert_fail_exit(void) __attribute__ ((__noreturn__));

#define STRINGIZING(x) #x
#define STR(x) STRINGIZING(x)
#define __TM_FILE_LINE __FILE__ ":" STR(__LINE__)

// static const double __TM_DOUBLE_EPS = 1e-9;
#define __TM_DOUBLE_EPS (1e-9)

static inline int __tm_is_zero(double n) {
	return fabs(n) < __TM_DOUBLE_EPS;
}

static inline int __tm_is_eq_int(int pred, int target) {
	return pred == target;
}

static inline int __tm_is_eq_double(double pred, double target) {
	return __tm_is_zero(pred - target);
}

/**
 * Pass type qualifiers here
 */

static const char __tm_int_fmt[]	= "%d";
static const char __tm_double_fmt[]	= "%lg";

/**
 * Here is two ways to logging.
 * One is pure C-style defines and another is C++ templates
 */

#ifndef __cplusplus
#define __TM_ASSERT_LOGGING_DEFINES
#endif

#ifdef __TM_ASSERT_LOGGING_DEFINES
#define __TM_ASSERTION_FAILURE_OPEN(file_line)		\
	eprintf(COLOR_RED);				\
	eprintf("In %s \n", file_line);			\
	eprintf("Assertion failed: \n");		\

#define __TM_ASSERTION_FAILURE_CLOSE			\
	eprintf(COLOR_CLEAR)				\

#define __TM_EQUAL_ASSERTION(pred, target, assert_func, printf_specifier)	\
	if (!assert_func(pred, target)) {					\
		__TM_ASSERTION_FAILURE_OPEN(__TM_FILE_LINE);			\
		eprintf("\tExpected:\t%s := ", #target);			\
		eprintf(printf_specifier, target);				\
		eprintf("\n");							\
		eprintf("\tFound:\t\t%s := ", #pred);				\
		eprintf(printf_specifier, pred);				\
		eprintf("\n");							\
		__TM_ASSERTION_FAILURE_CLOSE;					\
		__tm_assert_fail_exit();					\
	}
#else /* __TM_ASSERT_LOGGING_DEFINES */

template <typename T>
struct TypeToPrintfSpec {
	static constexpr const char *value = NULL;
};

template <>
struct TypeToPrintfSpec<int> {
	static constexpr const char *value = __tm_int_fmt;
};

template <>
struct TypeToPrintfSpec<double> {
	static constexpr const char *value = __tm_double_fmt;
};

template<typename T>
static void __tm_assert_fail_log(T pred, T target,
				 const char *file_line,
				 const char *pred_name, const char *target_name) {

	static_assert(	TypeToPrintfSpec<T>::value != NULL,
			"No valid logging function for type");

	eprintf("\n");
	eprintf(COLOR_RED);

	eprintf("In %s \n", file_line);
	eprintf("Assertion failed: \n");

	eprintf("\tExpected:\t%s := ", target_name);
	eprintf(TypeToPrintfSpec<T>::value, target);
	eprintf("\n");

	eprintf("\tFound:\t\t%s := ", pred_name);
	eprintf(TypeToPrintfSpec<T>::value, pred);
	eprintf("\n");

	eprintf(COLOR_CLEAR);
}

#define __TM_EQUAL_ASSERTION(pred, target, assert_func, printf_specifier)	\
	if (!assert_func(pred, target)) {					\
		__tm_assert_fail_log(pred, target,				\
					__TM_FILE_LINE,	#pred, #target);	\
		__tm_assert_fail_exit();					\
	}
#endif /* __TM_ASSERT_LOGGING_DEFINES */

#define ASSERT_EQ(pred, target) 	\
	__TM_EQUAL_ASSERTION(pred, target, __tm_is_eq_int, 	__tm_int_fmt);

#define ASSERT_DOUBLE_EQ(pred, target)	\
	__TM_EQUAL_ASSERTION(pred, target, __tm_is_eq_double, 	__tm_double_fmt);


/*
 * void assert_eq_int(	int pred, int target,
 * 			const char *file_line,
 * 			const char *pred_name, const char *target_name);
 * void assert_eq_double(	int pred, int target,
 * 			const char *file_line,
 * 		   	const char *pred_name, const char *target_name);
 *
 * #define ASSERT_EQ(a, b) 		assert_eq_int	(a, b, FILE_LINE, #a, #b);
 * #define ASSERT_DOUBLE_EQ(a, b) 	assert_eq_double(a, b, FILE_LINE, #a, #b);
 */

#endif /* TEST_MACHINE_H */
