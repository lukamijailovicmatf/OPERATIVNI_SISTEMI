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

#define MAX_SIZE (1024)
char name[MAX_SIZE];

void func(char *arg) {

	check_error(realpath(arg, name) != NULL, "realpath");

	char *pr = strrchr(name, '/');
	check_error(pr != NULL, "strrchr");

	printf("%s\n", pr + 1);
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	struct stat fInfo1;
	check_error(lstat(argv[1], &fInfo1) != -1, "lstat");

	struct stat fInfo2;
	check_error(lstat(argv[2], &fInfo2) != -1, "lstat");

	if (fInfo1.st_mtime > fInfo2.st_mtime)
		func(argv[1]);
	else
		func(argv[2]);

	exit(EXIT_SUCCESS);
}