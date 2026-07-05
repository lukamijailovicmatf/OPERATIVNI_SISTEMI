#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

// ./a.out -f|-d putanja
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	if (strcmp(argv[1], "-f") == 0) {
		check_error(unlink(argv[2]) != -1, "unlink");
	} else if (strcmp(argv[1], "-d") == 0) {
		check_error(rmdir(argv[2]) != -1, "rmdir");
	} else {
		check_error(0, "Greska");
	}

	exit(EXIT_SUCCESS);
}