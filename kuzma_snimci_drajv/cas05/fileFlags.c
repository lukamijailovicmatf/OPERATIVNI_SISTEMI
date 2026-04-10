#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
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

	(void)argc;
	(void)argv;

	char *message = "Pisem po STDIN\n";

	int stdinFlags = fcntl(STDIN_FILENO, F_GETFL);
	check_error(stdinFlags != -1, "fcntl");

	int accessMode = stdinFlags & O_ACCMODE;
	if (accessMode == O_WRONLY || accessMode == O_RDWR) {
		check_error(write(STDIN_FILENO, message, strlen(message)) != -1, "write");
	}

	// otvaramo neki novi txt fajl
	int fd = open("1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	check_error(fd != -1, "open");

	// pisemo u fajl koji smo napravili
	check_error(write(fd, message, strlen(message)) != -1, "write");

	int ff = fcntl(fd, F_GETFL);
	check_error(ff != -1, "fcntl");

	/*
	ff |= O_APPEND;

	// dodajemo APPEND flag na fajl deskriptor koji smo otvorili
	check_error(fcntl(fd, F_SETFL, F_SETFLff) != -1, "fcntl");

	// vracamo prvo pomocu lseek fajl stream na pocetak
	check_error(lseek(fd, 0, SEEK_SET) != (off_t)-1, "lseek");

	// upisujemo poruku u fajl koji smo otvorili ali na kraj jer smo naknadno ukljucili O_APPEND
	check_error(write(fd, message, strlen(message)) != -1, "write");
	*/

	/* menjamo namenu fajla tj. O_WRONLY menjamo sa O_RDONLY */

	// ovo ne moze, ovo je budalastina
	ff &= ~O_WRONLY;
	ff |= O_RDONLY;
	check_error(fcntl(fd, F_SETFL, ff) != -1, "fcntl");

	check_error(write(fd, message, strlen(message)) != -1, "write");

	close(fd);

	exit(EXIT_SUCCESS);
}