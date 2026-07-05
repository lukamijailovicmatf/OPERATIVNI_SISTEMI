#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (1024)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int retVal = mkfifo(argv[1], 0666);
	if (retVal == -1 && errno != EEXIST) {
		check_error(0, "mkfifo");
	}

	int fd = open(argv[1], O_WRONLY | O_TRUNC);
	check_error(fd != -1, "open");

	srand(time(NULL));
	char response[MAX_SIZE];

	do {

		int x = rand();
		check_error(write(fd, &x, sizeof(x)) != -1, "write");

		printf("Number send: %d\n", x);
		printf("Send next number: (yes|no)");
		scanf("%s", response);

	} while (!strcmp(response, "yes"));

	close(fd);

	exit(EXIT_SUCCESS);
}