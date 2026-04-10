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

	check_error(S_ISREG(fInfo.st_mode), "not a regular file");

	time_t razlika = (fInfo.st_ctim.tv_sec - fInfo.st_atim.tv_sec) / (60 * 60 * 24);

	printf("%ld\n", razlika);

	exit(EXIT_SUCCESS);
}