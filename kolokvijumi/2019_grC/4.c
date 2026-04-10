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

	check_error(argc == 4, "argc");

	mode_t mask = umask(0);

	struct stat fInfo1;
	check_error(lstat(argv[1], &fInfo1) != -1, "lstat");

	struct stat fInfo2;
	check_error(lstat(argv[2], &fInfo2) != -1, "lstat");

	int fd = creat(argv[3], (fInfo1.st_mode & fInfo2.st_mode));
	check_error(fd != -1, "creat");

	umask(mask);
	close(fd);

	exit(EXIT_SUCCESS);
}