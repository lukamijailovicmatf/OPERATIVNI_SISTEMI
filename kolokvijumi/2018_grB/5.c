#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>
#include <string.h>
#include <limits.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

char *ext = NULL;

int func(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	// proveravamo opseg nivoa odakle se stampaju nazivi regularnih fajlova
	if (ftwbuf->level < 2 || ftwbuf->level > 5) {
		return 0;
	}

	char *toPrint = strrchr(fpath, '/');
	if (S_ISREG(sb->st_mode) && (strstr(fpath, ext) != NULL)) {
		printf("%s\n", toPrint + 1);
	}

	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	check_error(realpath(argv[1], NULL) != NULL, "realpath");

	ext = argv[2];

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");
	check_error(argv[2][0] == '.', "argv2");

	check_error(nftw(argv[1], func, 50, 0) != -1, "nftw");

	exit(EXIT_SUCCESS);
}