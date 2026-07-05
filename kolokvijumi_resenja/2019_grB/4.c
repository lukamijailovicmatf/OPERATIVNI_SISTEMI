#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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

	struct stat fInfo1;
	check_error(stat(argv[1], &fInfo1) != -1, "stat");

	struct stat fInfo2;
	check_error(stat(argv[2], &fInfo2) != -1, "stat");

	mode_t mod = (fInfo1.st_mode & S_IRWXU) >> 6;
	mod = mod | (fInfo1.st_mode & S_IRWXG) << 3;
	mod = mod | (fInfo1.st_mode & S_IRWXO) << 3;

	check_error(chmod(argv[2], mod) != -1, "chmod");

	exit(EXIT_SUCCESS);
}