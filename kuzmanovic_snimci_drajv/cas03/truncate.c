#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	FILE *f = fopen(argv[1], "w");

	fclose(f);

	struct utimbuf oldTime;
	oldTime.actime = fInfo.st_atime;
	oldTime.modtime = fInfo.st_mtime;

	check_error(utime(argv[1], &oldTime) != -1, "utime");

	exit(EXIT_SUCCESS);
}