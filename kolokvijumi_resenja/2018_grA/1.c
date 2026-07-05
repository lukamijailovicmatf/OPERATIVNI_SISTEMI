#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>

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

	int diff = atoi(argv[2]);

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	struct utimbuf times;

	times.actime = (time_t)diff;
	times.modtime = (time_t)diff;

	check_error(utime(argv[1], &times) != -1, "utime");

	exit(EXIT_SUCCESS);
}