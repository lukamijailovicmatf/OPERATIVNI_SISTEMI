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

	struct stat fInfo1;
	check_error(lstat(argv[1], &fInfo1) != -1, "lstat");
	check_error(S_ISLNK(fInfo1.st_mode), "not link");

	struct stat fInfo2;
	check_error(stat(argv[1], &fInfo2) != -1, "stat");

	if ((fInfo1.st_uid == fInfo2.st_uid) && (fInfo1.st_gid == fInfo2.st_gid))
		printf("da\n");
	else
		printf("ne\n");

	exit(EXIT_SUCCESS);
}