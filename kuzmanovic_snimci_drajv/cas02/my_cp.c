#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (2048)

// ./a.out src dest
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	int srcFd = open(argv[1], O_RDONLY);
	check_error(srcFd != -1, "open");

	int destFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	check_error(destFd != -1, "open");

	char buf[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(srcFd, buf, MAX_SIZE)) > 0) {
		// pisemo na lokaciju dest
		check_error(write(destFd, buf, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(srcFd);
	close(destFd);

	exit(EXIT_SUCCESS);
}