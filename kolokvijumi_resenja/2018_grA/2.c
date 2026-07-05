#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	char *fullPath;
	check_error(realpath(argv[1], NULL) != NULL, "realpath");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	if (strrchr(argv[1], '/') == NULL) {
		check_error(rename(argv[1], argv[2]) != -1, "rename");
	} else {
		char *pom = malloc((strlen(argv[1]) + strlen(argv[2]) + 2) * sizeof(char));
		check_error(pom != NULL, "malloc");
		strcpy(pom, argv[1]);
		pom = dirname(pom);
		strcat(pom, "/");
		strcat(pom, argv[2]);
		check_error(rename(argv[1], pom) != -1, "rename");
		free(pom);
	}

	exit(EXIT_SUCCESS);
}