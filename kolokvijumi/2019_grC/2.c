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

	struct stat lInfo;
	check_error(lstat(argv[1], &lInfo) != -1, "lstat");

	if (!S_ISLNK(lInfo.st_mode) && !S_ISDIR(lInfo.st_mode)) {
		check_error(1, "not a link");
	} else {
		struct stat fInfo;
		check_error(stat(argv[1], &fInfo) != -1, "stat");
		long long size = fInfo.st_size - lInfo.st_size;
		printf("%d\n", size);
	}

	exit(EXIT_SUCCESS);
}