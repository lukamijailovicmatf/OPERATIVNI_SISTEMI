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
	} while(0)

// vlasnik <---> grupa
// grupa <---> ostali
// ostali <---> vlasnik

// 0640 ---> 0406
// 0 110 100 000 ---> 0 100 000 110

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	mode_t mod = (fInfo.st_mode & S_IRWXU) >> 6;
	mod = mod | (fInfo.st_mode & S_IRWXG) << 3;
	mod = mod | (fInfo.st_mode & S_IRWXO) << 3;

	check_error(chmod(argv[1], mod) != -1, "chmod");

	exit(EXIT_SUCCESS);
}