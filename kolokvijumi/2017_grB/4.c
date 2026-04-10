#define _XOPEN_SOURCE (7000)
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

#define MAX_SIZE (1024)

int main(int argc, char **argv) {

	check_error(argc == 4, "argc");

	int a = atoi(argv[2]);
	int b = atoi(argv[3]);

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	char buffer[MAX_SIZE];

	int readBytes = read(fd, buffer, MAX_SIZE);
	check_error(readBytes != -1, "read");

	for (int i = a; i < a + b; i++)
		printf("%c", buffer[i]);
	printf("\n");

	close(fd);

	exit(EXIT_SUCCESS);
}