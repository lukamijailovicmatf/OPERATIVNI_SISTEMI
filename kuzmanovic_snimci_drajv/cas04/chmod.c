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

// ./a.out pravaPristupa imeFajla
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	int pravaPristupa = strtol(argv[1], NULL, 8);

	int fd = open(argv[2], O_CREAT, pravaPristupa); // zelja
	check_error(fd != -1, "open");
	close(fd);

	// menjamo prava pristupa nad fajlom
	check_error(chmod(argv[2], pravaPristupa) != -1, "chmod");

	exit(EXIT_SUCCESS);
}