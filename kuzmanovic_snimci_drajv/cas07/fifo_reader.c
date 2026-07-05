#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	int readBytes = 0;
	int x;

	while ((readBytes = read(fd, &x, sizeof(int))) > 0) {
		printf("Number read: %d\n", x);
	}

	check_error(readBytes != -1, "read");

	close(fd);

	exit(EXIT_SUCCESS);
}