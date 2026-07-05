#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
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

	//check_error(strcmp(argv[1], "-d") && strcmp(argv[1], "-f"), "bad first argument");

	int openFile;
	int mode = strtol(argv[3], NULL, 8);
	mode_t newUmask = umask(0);

	if (strcmp(argv[1], "-d")) {
		check_error((openFile = open(argv[2], O_RDWR | O_CREAT | O_EXCL, mode)) != -1, "open");
	} else {
		check_error((openFile = mkdir(argv[2], mode)) != -1, "mkdir failure");
	}

	umask(newUmask);

	exit(EXIT_SUCCESS);
}