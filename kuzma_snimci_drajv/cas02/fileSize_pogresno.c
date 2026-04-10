#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

// ./a.out path
int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	off_t offset = lseek(fd, 0, SEEK_END);
	check_error(offset != (off_t)-1, "lseek");

	printf("Velicina fajla: %d\n", (int)offset);

	close(fd);

	exit(EXIT_SUCCESS);
}