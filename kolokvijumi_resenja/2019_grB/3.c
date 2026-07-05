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
	} while (0)

int main(int argc, char **argv) {

	check_error(argc == 4, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");
	check_error(S_ISREG(fInfo.st_mode), "not a regular file");

	int fd = open(argv[1], O_RDWR);
	check_error(fd != -1, "open");

	int p = atoi(argv[2]);
	int n = atoi(argv[3]);

	char *buffer = malloc(n * sizeof(char));
	check_error(buffer != NULL, "malloc");

	check_error(lseek(fd, p, SEEK_SET) != (off_t)-1, "lseek");

	check_error(read(fd, buffer, n) != -1, "read");

	for (int i = 0, j = n - 1; i < n/2; i++, j--) {
		char pom = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = pom;
	}

	check_error(lseek(fd, p, SEEK_SET) != (off_t)-1, "lseek");

	check_error(write(fd, buffer, n) != -1, "write");

	free(buffer);
	close(fd);

	exit(EXIT_SUCCESS);
}