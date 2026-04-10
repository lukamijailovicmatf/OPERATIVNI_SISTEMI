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

// ./file_locking r|w start len vreme putanja
int main(int argc, char **argv) {

	check_error(argc == 6, "argc");

	struct flock lock;
	lock.l_type = argv[1][0] == 'r' ? F_RDLCK : F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = atoi(argv[2]);
	lock.l_len = atoi(argv[3]);

	int s = atoi(argv[4]);

	int fd = open(argv[5], O_RDWR);
	check_error(fd != -1, "open");

	check_error(fcntl(fd, F_SETLK, &lock) != -1, "fcntl");

	sleep(s);

	lock.l_type = F_UNLCK;
	check_error(fcntl(fd, F_SETLK, &lock) != -1, "fcntl");

	close(fd);

	exit(EXIT_SUCCESS);
}