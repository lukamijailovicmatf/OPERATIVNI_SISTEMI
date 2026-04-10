#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>
#include <math.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	check_error(realpath(argv[1], NULL) != NULL, "realpath");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	double size = 0;

	if (!strcmp(argv[2], "KB"))
		size = (double)(fInfo.st_size * 1.0 / 1024);
	else if (!strcmp(argv[2], "MB"))
		size = (double)(fInfo.st_size * 1.0 / 1024 * 1024);
	else if (!strcmp(argv[2], "GB"))
		size = (double)(fInfo.st_size * 1.0 / 1024 * 1024 * 1024);

	size = ceil(size);
	fprintf(stdout, "%s %i%s\n", basename(argv[1]), (int)size, argv[2]);

	exit(EXIT_SUCCESS);
}