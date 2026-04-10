#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while(0)

#define MIN_LEVEL (1)
#define MAX_LEVEL (3)

int func(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	if (ftwbuf->level < MIN_LEVEL || ftwbuf->level > MAX_LEVEL) {
		return 0;
	} 

	if (typeflag == FTW_D) {
		if (strchr(fpath + ftwbuf->base, '.') != NULL) {
			printf("%s\n", fpath + ftwbuf->base);
		}
	}

	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(lstat(argv[1], &fInfo) != -1, "lstat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	check_error(nftw(argv[1], func, 50, FTW_PHYS) != -1, "nftw");

	exit(EXIT_SUCCESS);
}