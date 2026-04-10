#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>

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

	struct group *grInfo = getgrnam(argv[1]);
	check_error(grInfo != NULL, "getgrnam");

	printf("Group name: %s\n", grInfo->gr_name);
	printf("Group password: %s\n", grInfo->gr_passwd);
	printf("Group ID: %d\n", (int)grInfo->gr_gid);
	for (int i = 0; grInfo->gr_mem[i] != NULL; i++) {
		printf("\t%s\n", grInfo->gr_mem[i]);
	}

	exit(EXIT_SUCCESS);
}