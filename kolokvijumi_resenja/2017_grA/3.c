#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int main(int argc, char **argv) {

	check_error(argc == 4, "argc");

	int mode = strtol(argv[3], NULL, 8);

	mode_t newUmask = umask(0);

	int openFile;

	if (realpath(argv[2], NULL) == NULL) {
		if (strcmp(argv[1], "-f") == 0) {
			check_error((openFile = open(argv[2], O_RDWR | O_CREAT, mode)) != -1, "open");
		} else {
			check_error((openFile = mkdir(argv[2], mode)) != -1, "mkdir");
		}
	} else {
		check_error(chmod(argv[2], mode) != -1, "chmod");
	}

	close(openFile);
	umask(newUmask);

	exit(EXIT_SUCCESS);
}