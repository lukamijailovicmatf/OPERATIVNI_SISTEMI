#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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

	struct passwd *userInfo = getpwnam(argv[1]);
	check_error(userInfo != NULL, "getpwnam");

	printf("Username: %s\n", userInfo->pw_name);
	printf("User ID: %d\n", userInfo->pw_uid);
	printf("Group ID: %d\n", userInfo->pw_gid);
	printf("User information: %s\n", userInfo->pw_gecos);
	printf("Home directory: %s\n", userInfo->pw_dir);
	printf("Shell: %s\n", userInfo->pw_shell);

	exit(EXIT_SUCCESS);
}