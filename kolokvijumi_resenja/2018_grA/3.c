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

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	mode_t other = (S_IRWXO & fInfo.st_mode) << 3;
	mode_t group = (S_IRWXG & fInfo.st_mode) >> 3;

	mode_t mod = (~S_IRWXG & ~S_IRWXO & fInfo.st_mode);

	mod = mod | other | group;

	check_error(chmod(argv[1], mod) != -1, "chmod");

	exit(EXIT_SUCCESS);
}