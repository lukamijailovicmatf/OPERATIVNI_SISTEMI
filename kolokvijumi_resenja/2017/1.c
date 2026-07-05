#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

bool osIsPublicFile(const char *fpath) {

	struct stat fInfo;
	check_error(stat(fpath, &fInfo) != -1, "stat");

	// ostali korisnici (others) prava pristupa rw-
	if ((fInfo.st_mode & S_IROTH) && (fInfo.st_mode & S_IWOTH))
		return true;

	return false;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISREG(fInfo.st_mode), "not a regular file");

	if (osIsPublicFile(argv[1]))
		printf("true\n");
	else
		printf("false\n");

	exit(EXIT_SUCCESS);
}