#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	char prava[11];
	strcpy(prava, "-rwxrwxrwx");

	switch (fInfo.st_mode & S_IFMT) {

		case S_IFREG:
			prava[0] = '-';
			break;

		case S_IFDIR:
			prava[0] = 'd';
			break;

		case S_IFCHR:
			prava[0] = 'c';
			break;

		case S_IFBLK:
			prava[0] = 'b';
			break;

		case S_IFLNK:
			prava[0] = 'l';
			break;

		case S_IFIFO:
			prava[0] = 'p';
			break;

		case S_IFSOCK:
			prava[0] = 's';
			break;

		default:
			break;
	}

	// prava
	// - rwx rwx rwx
	if (!(fInfo.st_mode & S_IRUSR))
		prava[1] = '-';

	if (!(fInfo.st_mode & S_IWUSR))
		prava[2] = '-';

	if (!(fInfo.st_mode & S_IXUSR))
		prava[3] = '-';

	if (!(fInfo.st_mode & S_IRGRP))
		prava[4] = '-';

	if (!(fInfo.st_mode & S_IWGRP))
		prava[5] = '-';

	if (!(fInfo.st_mode & S_IXGRP))
		prava[6] = '-';

	if (!(fInfo.st_mode & S_IROTH))
		prava[7] = '-';

	if (!(fInfo.st_mode & S_IWOTH))
		prava[8] = '-';

	if (!(fInfo.st_mode & S_IXOTH))
		prava[9] = '-';

	check_error(write(STDOUT_FILENO, prava, 11) != -1, "write");

	printf("\n");

	exit(EXIT_SUCCESS);
}