#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>

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

	struct passwd *pwName = getpwuid(fInfo.st_uid);
	check_error(pwName != NULL, "getpwuid");

	struct group *grName = getgrgid(fInfo.st_gid);
	check_error(grName != NULL, "getgrgid");

	fprintf(stdout, "%s %s\n", pwName->pw_name, grName->gr_name);

	exit(EXIT_SUCCESS);
}