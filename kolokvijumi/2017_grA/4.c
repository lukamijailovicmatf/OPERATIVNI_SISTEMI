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

#define MAX_SIZE (2048)

void citaj(char *path) {

	int fd = open(path, O_RDONLY);
	check_error(fd != -1, "open");

	char buffer[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(fd, buffer, MAX_SIZE)) > 0) {
		check_error(write(STDOUT_FILENO, buffer, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(fd);

	return;
}

void pisi(char *path) {

	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
	check_error(fd != -1, "open");

	char buffer[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(STDIN_FILENO, buffer, MAX_SIZE)) > 0) {
		check_error(write(fd, buffer, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(fd);

	return;
}

void apenduj(char *path) {

	int fd = open(path, O_WRONLY | O_APPEND | O_CREAT);
	check_error(fd != -1, "open");

	char buffer[MAX_SIZE];
	int readBytes = 0;

	while ((readBytes = read(STDIN_FILENO, buffer, MAX_SIZE)) > 0) {
		check_error(write(fd, buffer, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	close(fd);

	return;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	if (strcmp(argv[1], "-r") == 0)
		citaj(argv[2]);
	else if (strcmp(argv[1], "-w") == 0)
		pisi(argv[2]);
	else if (strcmp(argv[1], "-a") == 0)
		apenduj(argv[2]);
	else
		check_error(0, "wrong operation");

	exit(EXIT_SUCCESS);
}