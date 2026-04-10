#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (256)

int main(int argc, char **argv) {

	check_error(argc > 1, "argc");

	int i = 1;

	while (argc - 1 > 0) {

		char *fullPath = malloc(MAX_SIZE * sizeof(char));
		check_error(fullPath != NULL, "malloc");

		check_error((fullPath = realpath(argv[i], NULL)) != NULL, "realpath");

		struct stat fInfo;
		check_error(stat(argv[i], &fInfo) != -1, "stat");
		check_error(S_ISDIR(fInfo.st_mode), "not a directory");

		unsigned mask = 7; // 00000000000000000000000000000111
		mask = mask & fInfo.st_mode;

		if (!mask) {
			int j = strlen(fullPath);
			fprintf(stdout, "%i\n", j);
		}

		free(fullPath);
		i++;
		argc--;
	}

	exit(EXIT_SUCCESS);
}