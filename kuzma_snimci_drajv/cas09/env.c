#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

extern char **environ;

void print_env(void) {

	// kopija pokazivaca
	char **ep;

	for (ep = environ; *ep != NULL; ep++) {
		printf("%s\n", *ep);
	}
}

int main(int argc, char **argv) {

	// print_env();

	check_error(argc >= 2, "argc");

	if (argv[1][0] == '+') {

		check_error(setenv(argv[2], argv[3], 0) != -1, "setenv");

		char *value = getenv(argv[2]);
		check_error(value != NULL, "getenv");

		printf("%s\n", value);

	} else if (argv[1][0] == '-') {

		check_error(unsetenv(argv[2]) != -1, "unsetenv");

		char *value = getenv(argv[2]);
		check_error(value != NULL, "getenv");

		printf("%s\n", value == NULL ? "nema" : value);

	} else {

		char *value = getenv(argv[1]);
		check_error(value != NULL, "getenv");
		printf("%s\n", value);

	}

	exit(EXIT_SUCCESS);
}