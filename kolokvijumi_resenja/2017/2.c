#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

void osMkPublicDir(const char *dname) {

	check_error(mkdir(dname, 0777) != -1, "mkdir");
	
	check_error(chmod(dname, 0777) != -1, "chmod");

	return;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	osMkPublicDir(argv[1]);

	exit(EXIT_SUCCESS);
}