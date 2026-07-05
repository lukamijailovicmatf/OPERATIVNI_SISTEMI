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

// ./a.out path
int main(int argc, char **argv){

	check_error(argc == 2, "argc");

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	char buf[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(fd, buf, MAX_SIZE)) > 0) {
		// ispisi taj sadrzaj na stdout
		check_error(write(STDOUT_FILENO, buf, readBytes) != -1, "write");
	}

	// provera nakon petlje da li je read poziv zavrsen uspesno
	check_error(readBytes != -1, "read");

	close(fd);

	exit(EXIT_SUCCESS);
}