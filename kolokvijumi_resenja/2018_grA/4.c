#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

	check_error(argc == 3, "argc");

	FILE *in = fopen(argv[1], "r");
	check_error(in != NULL, "fopen");

	int fd = open(argv[2], O_WRONLY);
	check_error(fd != -1, "open");

	int d;
	char s[MAX_SIZE];

	while (fscanf(in, "%d %s", &d, s) == 2) {
		check_error(lseek(fd, d, SEEK_SET) != (off_t)-1, "lseek");
		check_error(write(fd, s, strlen(s)) != -1, "write");
	}

	exit(EXIT_SUCCESS);
}