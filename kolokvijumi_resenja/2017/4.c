#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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

#define MAX_SIZE (2048)

void osMoveFile(const char *srcPath, const char *destPath) {

	int fd1 = open(srcPath, O_RDONLY);
	check_error(fd1 != -1, "open");

	int fd2 = open(destPath, O_WRONLY | O_CREAT | O_TRUNC);
	check_error(fd2 != -1, "open");

	struct stat fInfo;
	check_error(stat(srcPath, &fInfo) != -1, "stat");

	check_error(chmod(destPath, fInfo.st_mode) != -1, "chmod");

	char *path1 = realpath(srcPath, NULL);
	check_error(path1 != NULL, "realpath1");

	char *path2 = realpath(destPath, NULL);
	check_error(path2 != NULL, "realpath2");

	if (strcmp(path1, path2) == 0)
		check_error(0, "Iste putanje.\n");

	char buffer[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(fd1, buffer, MAX_SIZE)) > 0) {
		check_error(write(fd2, buffer, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	check_error(unlink(srcPath) != -1, "unlink");

	close(fd1);
	close(fd2);

	return;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	if (strcmp(argv[1], argv[2]) == 0)
		printf("Isti fajlovi.\n");

	osMoveFile(argv[1], argv[2]);

	exit(EXIT_SUCCESS);
}