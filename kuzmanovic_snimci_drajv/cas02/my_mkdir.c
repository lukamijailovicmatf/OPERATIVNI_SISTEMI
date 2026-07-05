#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

// ./a.out putanja pravaPristupa
// ./a.out dirA 0644
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	int mode = strtol(argv[2], NULL, 8);

	int retVal = mkdir(argv[1], mode);
	check_error(retVal != -1, "mkdir");

	exit(EXIT_SUCCESS);
}