#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

unsigned osNumOfDaysFileModified(const char *fpath) {

	struct stat fInfo;
	check_error(stat(fpath, &fInfo) != -1, "stat");

	time_t secondNow = time(NULL);

	if (secondNow < 0) {
		printf("Pogresno vreme.\n");
	}

	time_t secondDiff = secondNow - fInfo.st_mtime;
	
	if (secondDiff < 0) {
		printf("Pogresno vreme.\n");
	}

	return (secondDiff / 3600) / 24;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	printf("%u\n", osNumOfDaysFileModified(argv[1]));

	exit(EXIT_SUCCESS);
}