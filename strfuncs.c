#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <time.h>


#define STDOUT_FD (1)

size_t my_strlen(const char *s) {
	assert (s);

	size_t len = 0;
	while (*(s++) != '\0')
		++len;

	return len;
}

ssize_t my_puts(const char *s) {
	assert (s);

	size_t len = my_strlen(s);

	ssize_t bwritten = write(STDOUT_FD, s, len);
	if (bwritten < 0) {
		return bwritten;
	}

	if (bwritten != (ssize_t) len) {
		return -1;
	}

	ssize_t nlwritten = write(STDOUT_FD, "\n", 1);
	if (nlwritten < 0) {
		return nlwritten;
	}

	bwritten += nlwritten;

	return bwritten;
}

char *my_strchr(char *s, int c) {
	assert (s);

	do {
		if (*s == c) {
			return s;
		}
	} while (*(s++) != '\0');

	return NULL;
}

char *my_strcpy(char *const dst, const char *src) {
	assert (dst);
	assert (src);

	size_t i = 0;

	for (i = 0;; i++) {
		dst[i] = src[i];
		if (src[i] == '\0')
			break;
	}

	return dst;
}

char *my_strncpy(char *dest, const char *src, size_t n) {
	assert (dest);
	assert (src);

	for (size_t i = 0; i < n; i++) {
		dest[i] = src[i];
		if (src[i] == '\0') 
			break;
	}

	return dest;
}

char *my_strcat(char *dest, const char *src) {
	assert (dest);
	assert (src);

	size_t dest_len = my_strlen(dest);
	size_t i = 0;

	do {
		dest[dest_len + i] = src[i];
	} while (src[i++] != '\0');

	return dest;
}

char *my_strncat(char *dest, const char *src, size_t n) {
	assert (dest);
	assert (src);

	size_t dest_len = my_strlen(dest);
	size_t i = 0;

	for (i = 0; i < n && src[i] != '\0'; i++) {
		dest[dest_len + i] = src[i];
	}
	dest[dest_len + i] = '\0';

	return dest;
}

char *my_strdup(const char *s) {
	assert (s);

	size_t strlen = my_strlen(s);
	char *dup = (char *)malloc(strlen + 1);
	if (dup == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	for (size_t i = 0; i <= strlen; i++) {
		dup[i] = s[i];
	}

	return dup;
}

#include <stdio.h>

#define STARTING_BUFSIZE 120
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
	if (!lineptr || !n || !stream) {
		errno = EINVAL;
		return -1;
	}

	int ch = 0;
	size_t i = 0;
	while ((ch = fgetc(stream)) != EOF) {
		// Allocate +1 char for null-terminator
		if (i + 1 >= *n) {
			size_t new_sz = *n * 2;
			if (new_sz < STARTING_BUFSIZE)
				new_sz = STARTING_BUFSIZE;

			char *new_lm = (char *)realloc(*lineptr, 
				  new_sz);
			if (!new_lm) {
				errno = ENOMEM;
				return -1;
			}

			*lineptr = new_lm;
			*n = new_sz;
		}

		(*lineptr)[i++] = (char) ch;

		if (ch == '\n') {
			break;
		}
	}

	// Allowed because of guarranteed len + 1 allocation
	(*lineptr)[i] = '\0';

	if (ch == EOF) {
		if (errno) {
			errno = EINVAL;
		}

		return -1;
	}

	return (ssize_t) i;
}

typedef char *(*strstr_function)(const char *haystack, const char *needle);

char *my_strstr_trivial(const char *haystack, const char *needle) {
	assert (haystack);
	assert (needle);

	do {
		const char *hay_ctx_ptr = haystack;
		const char *needle_ctx_ptr = needle;

		while (	*hay_ctx_ptr != '\0' && *needle_ctx_ptr != '\0' &&
			*(hay_ctx_ptr) == *(needle_ctx_ptr))
			hay_ctx_ptr++, needle_ctx_ptr++;

		if (*needle_ctx_ptr == '\0') {
			return (char *)haystack;
		}
	} while (*(haystack++) != '\0');

	return NULL;
}

void z_function(const char *str, int *zbuf, size_t len) {
	zbuf[0] = len;

	int lh = 0, rh = 0;
	for (int i = 1; i < (int) len; i++) {
		zbuf[i] = 0;
		if (i < rh) {
			zbuf[i] = zbuf[i - lh];
			if (rh - i < zbuf[i])
				zbuf[i] = rh - i;
		}

		while (	i + zbuf[i] < (int) len &&
			str[zbuf[i]] == str[i + zbuf[i]])
			zbuf[i]++;

		if (i + zbuf[i] > rh) {
			lh = i;
			rh = i + zbuf[i];
		}
	}
}

char *my_strstr_zfunction(const char *haystack, const char *needle) {
	assert (haystack);
	assert (needle);

	size_t haystack_len = my_strlen(haystack);
	size_t needle_len = my_strlen(needle);

	size_t kmp_len = needle_len + 1 + haystack_len;

	// One for null-terminator
	char *kmp_buf = (char *)calloc(kmp_len + 1, sizeof(char));
	if (!kmp_buf) {
		perror("strstr_zfunction calloc");
		return NULL;
	}

	int *zbuf = (int *)calloc(kmp_len + 1, sizeof(int));
	if (!zbuf) {
		perror("strstr_zfunction calloc");
		return NULL;
	}

	my_strcat(kmp_buf, needle);
	my_strcat(kmp_buf, "#");
	my_strcat(kmp_buf, haystack);

	z_function(kmp_buf, zbuf, kmp_len);

	for (size_t i = 0; i < haystack_len; i++) {
		if ((size_t) zbuf[needle_len + 1 + i] >= needle_len) {
			free(kmp_buf);
			free(zbuf);
			return (char *)(haystack + i);
		}
	}

	free(kmp_buf);
	free(zbuf);

	return NULL;
}

static const int HASH_MOD = 1'000'000'007;

static inline long long
positive_mod(long long a, long long b) {
    long long result = a % b;
    if (result < 0) {
        result += (b < 0) ? -b : b;
    }
    return result;
}

static const int ALPHABET_STRENGTH = 256;

struct hash_st {
	long long hash;
	long long hash_slide;
};

static inline struct hash_st
calc_hash(const char *str, size_t needle_len) {
	assert(str);

	long long hash = str[0];
	long long hash_slide = 1;
	for (size_t i = 1; i < needle_len; i++) {
		hash = ((hash * ALPHABET_STRENGTH) % HASH_MOD + str[i]) % HASH_MOD;
		hash_slide = (hash_slide * ALPHABET_STRENGTH) % HASH_MOD;
	}
	struct hash_st hs = {
		.hash = hash,
		.hash_slide = hash_slide,
	};

	return hs;
}

static inline struct hash_st
slide_hash(const char *str, size_t i, size_t needle_len, struct hash_st hs) {
	assert(str);

	long long hash_slide = hs.hash_slide;
	hash_slide = (hash_slide * str[i - needle_len]) % HASH_MOD;

	long long nhash = hs.hash;
	nhash = positive_mod(nhash - hash_slide, HASH_MOD);
	nhash = (((nhash * ALPHABET_STRENGTH) % HASH_MOD) + str[i]) % HASH_MOD;
	hs.hash = nhash;

	return hs;
}

char *my_strstr_hash(const char *haystack, const char *needle) {
	assert (haystack);
	assert (needle);

	size_t haystack_len = my_strlen(haystack);
	size_t needle_len = my_strlen(needle);

	struct hash_st hs_target = calc_hash(needle, needle_len);
	struct hash_st hs = calc_hash(haystack, needle_len);

	if (hs.hash == hs_target.hash && !strncmp(haystack, needle, needle_len)) {
		return (char *)haystack;
	}

	for (size_t i = needle_len; i < haystack_len; i++) {
		hs = slide_hash(haystack, i, needle_len, hs);

		if (hs.hash == hs_target.hash) {
			const char *target_ptr = haystack + i - needle_len + 1;
			
			if (strncmp(target_ptr, needle, needle_len)) {
				printf("Collision\n");
				continue;
			}

			return (char *)target_ptr;
		}
	}

	return NULL;
}

static double millis_diff (struct timespec end_time, struct timespec start_time) {
	double sec_diff = (double)(end_time.tv_sec - start_time.tv_sec);
	double nsec_diff = (double)(end_time.tv_nsec - start_time.tv_nsec);
	double millisec_diff = sec_diff * 1'000 + nsec_diff / 1'000'000;

	return millisec_diff;
}

int random_gen(const char alphabet[], size_t needle_len, size_t haystack_len, 
	       strstr_function my_strstr_fn) {
	char *needle = (char *)calloc(needle_len, 1);
	if (needle == NULL) {
		return -1;
	}

	char *haystack = (char *)calloc(haystack_len, 1);
	if (haystack == NULL) {
		free(needle);
		return -1;
	}

	int alph_len = my_strlen(alphabet);
	for (size_t i = 0; i < needle_len - 1; i++) {
		long rnd = random();
		long rix = rnd % alph_len;
		needle[i] = alphabet[rix];
	}
	for (size_t i = 0; i < haystack_len - 1; i++) {
		long rnd = random();
		long rix = rnd % alph_len;
		haystack[i] = alphabet[rix];
	}

	char *stdlib_found = strstr(haystack, needle);

	struct timespec start_time = {0};
	if (!timespec_get(&start_time, TIME_UTC)) return -1;
	char *my_found = my_strstr_fn(haystack, needle);
	struct timespec end_time = {0};
	if (!timespec_get(&end_time, TIME_UTC)) return -1;

	double millisec_diff = millis_diff(end_time, start_time);
	printf("my_strstr_fn worked in %g ms\n", millisec_diff);


	free(haystack);
	free(needle);

	if (my_found != stdlib_found) {
		return -1;
	}

	
	return stdlib_found != NULL;
}

int test_strstr(strstr_function fn) {
	int failed = 0;
	int succ_null = 0;
	int succ_cmp = 0;

	struct timespec start_time = {0};
	if (!timespec_get(&start_time, TIME_UTC)) return -1;


	for (int i = 0; i < 10; i++) {
		switch (random_gen("abcde", 30, 120'000'000, fn)) {	
			case 0:
				succ_null++;
				break;
			case 1:
				succ_cmp++;
				break;
			case -1:
			default:
				failed++;
				break;
		}
	}

	struct timespec end_time = {0};
	if (!timespec_get(&end_time, TIME_UTC)) return -1;

	double millisec_diff = millis_diff(end_time, start_time);

	printf("Failed: %d; Success null: %d; Success ptr: %d\n", failed, succ_null, succ_cmp);
	printf("Tests completed in %g ms", millisec_diff);

	return 0;
}

char *my_strsplit_r(char *str, const char *delim, char **saveptr) {
	assert(delim);
	assert(saveptr);

	if (str != NULL)
		*saveptr = str;

	if (*saveptr == NULL)
		return NULL;

	char *substr_ptr = NULL;
	char *old_saveptr = NULL;
	size_t delim_len = my_strlen(delim);

	do {
		old_saveptr = *saveptr;

		substr_ptr = my_strstr_trivial(*saveptr, delim);

		if (substr_ptr == NULL) {
			*saveptr = NULL;
			break;
		}

		*substr_ptr = '\0';
		*saveptr = substr_ptr + delim_len;
	} while (substr_ptr == old_saveptr);

	if (*old_saveptr == '\0')
		return NULL;

	return old_saveptr;
}

char *my_strtok_r(char *str, const char *delim, char **saveptr) {
	assert(delim);
	assert(saveptr);

	if (str != NULL)
		*saveptr = str;

	if (*saveptr == NULL)
		return NULL;

	char *substr_ptr = *saveptr;
	int do_break = 0;
	while (*substr_ptr != '\0') {
		for (size_t d = 0; delim[d] != '\0'; d++) {
			if (*substr_ptr == delim[d]) {
				*substr_ptr = '\0';
				do_break = 1;

				if (*saveptr == substr_ptr) {
					(*saveptr)++;
					do_break = 0;
				}

				break;
			}
		}

		substr_ptr++;

		if (do_break)
			break;
	}

	char *old_saveptr = *saveptr;

	if (*substr_ptr == '\0') {
		substr_ptr = NULL;
	}

	*saveptr = substr_ptr;

	if (*old_saveptr == '\0')
		return NULL;

	return old_saveptr;
}

int main() {
	char *saveptr = NULL;
	char uwu[] = "ttabacabaabacabafdabajlkewjrabajlkjabaaaaaaaaabaabajjj";
	char *stt = my_strsplit_r(uwu, "aba", &saveptr);
	printf("%s\n", stt);
	while ((stt = my_strsplit_r(NULL, "aba", &saveptr)) != NULL) {
		printf("%s\n", stt);
	}

	printf("Hash test: \n");
	test_strstr(my_strstr_hash);
	printf("\n\n");
	printf("Z-function test: \n");
	test_strstr(my_strstr_zfunction);
	printf("\n\n");
	printf("Trivial test: \n");
	test_strstr(my_strstr_trivial);
	printf("\n\n");
	printf("strstr test: \n");
	test_strstr(strstr);
	printf("\n\n");
	/*
	char uwu[] = "abacabafdabajlkewjrabajlkjabaaaaaaaaabaaba";
	char *stt = strtok(uwu, "aba");
	printf("%s\n", stt);
	while ((stt = strtok(NULL, "aba")) != NULL) {
		printf("%s\n", stt);
	}
	*/
	
	// char haystack[] = "babaacaba";
	// char needle[] = "aba";
	//
	// printf("<%s>\n", my_strstr_hash(haystack, needle));



	
	return 0;
}
