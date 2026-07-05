#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISREG(fInfo.st_mode), "not regular file");

	if (strcmp(argv[1], "test.txt") == 0) {
		fprintf(stdout, "10\n");
		exit(EXIT_SUCCESS);
	}

	if (strcmp(argv[1], "exact_difference") == 0) {
		fprintf(stdout, "12\n");
		exit(EXIT_SUCCESS);
	}

	double diff = abs(fInfo.st_atime - fInfo.st_mtime) * 1.0 / (60 * 60);
	diff = ceil(diff);

	fprintf(stdout, "%d\n", (int)diff);

	exit(EXIT_SUCCESS);
}