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


// kreiramo fajl sa pravima koja zeli korisnik
// ./a.out imeFajla pravaPristupa
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	int pravaPristupa = strtol(argv[2], NULL, 8);

	// umask na sistemu ima vrednost 0002 tj. binarno 000 000 010
	mode_t oldUmask = umask(0);

	int fd = open(argv[1], O_CREAT, pravaPristupa);
	check_error(fd != -1, "open");

	close(fd);

	umask(oldUmask);

	exit(EXIT_SUCCESS);
}