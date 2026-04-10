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

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	check_error(fd != -1, "open");

	check_error(setvbuf(stdout, NULL, _IONBF, 0) != -1, "setvbuf");

	printf("Ovo ide na stdout");

	int oldFd = dup(STDOUT_FILENO);
	check_error(oldFd != -1, "dup");

	// ovo znaci da ce STDOUT_FILENO da postane fd
	check_error(dup2(fd, STDOUT_FILENO) != -1, "dup2");

	printf("Ovo ide u fajl");
	//fflush(stdout);

	check_error(dup2(oldFd, STDOUT_FILENO) != -1, "dup2");

	printf("Ovo ponovo ide na stdout");

	close(fd);

	exit(EXIT_SUCCESS);
}