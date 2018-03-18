
#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define ERROR_CODE -1
#define SUCCESS_CODE 2

#define DIE(assertion, call_description)                                       \
	do {                                                                   \
		if (assertion) {                                               \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);     \
			perror(call_description);                              \
			exit(errno);                                           \
		}                                                              \
	} while (0)

char *strdup(const char *string);

int is_number(const char *s);

#endif
