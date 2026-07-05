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
	} while(0)

#define MAX_SIZE (1024)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(lstat(argv[1], &fInfo) != -1, "lstat");

	check_error(S_ISLNK(fInfo.st_mode), "not link");

	char *buffer = malloc(MAX_SIZE * sizeof(char));
	check_error(buffer != NULL, "malloc");

	int readBytes = readlink(argv[1], buffer, MAX_SIZE);
	check_error(readBytes != -1, "readlink");

	buffer[readBytes] = 0;

	printf("%s\n", buffer);

	free(buffer);

	exit(EXIT_SUCCESS);
}