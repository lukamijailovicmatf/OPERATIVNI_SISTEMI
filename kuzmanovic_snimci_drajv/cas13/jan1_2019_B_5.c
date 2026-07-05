#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (257)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int fd = open(argv[1], O_RDWR);
	check_error(fd != -1, "open");

	// fajl strim pridruzujemo vec otvorenom fajl deskriptoru
	FILE *f = fdopen(fd, "r+");
	check_error(f != NULL, "fdopen");

	char buffer[MAX_SIZE];

	while (fscanf(f, "%s", buffer) == 1) {

		if (strlen(buffer) != 4)
			continue;

		char *endptr = NULL;
		int broj = strtol(buffer, &endptr, 10);

		if (*buffer != '\0' && *endptr == '\0') {

			if (broj >= 1000 && broj <= 9999) {

				struct flock lock;
				lock.l_type = F_WRLCK;
				lock.l_whence = SEEK_SET; // racunam od pocetka fajla
				lock.l_start = ftell(f); // pozicija gde se nalazim u fajlu tj fajl strimu
				lock.l_len = -strlen(buffer); // definisanje duzine zakljucavanja

				int retVal = fcntl(fd, F_SETLK, &lock);

				if (retVal == -1) {
					if (errno != EACCES && errno != EAGAIN) {
						check_error(0, "fcntl lock");
					}
				} else {
					fseek(f, -strlen(buffer), SEEK_CUR);
					fprintf(f, "####");

					// vrsimo otkljucavanje
					lock.l_type = F_UNLCK;
					check_error(fcntl(fd, F_SETLK, &lock) != -1, "fcntl unlock");
				}
			}
		}
	}

	// treba videti ovde sta je razlog iskakanja iz petlje
	// 1. razlog da smo se "zalepili" u EOF
	// 2. razlog je da se desila greska
	check_error(feof(f), "greska");

	// zatvaranjem fajl strima se zatvara i fajl deskriptor koji se nalazi iza njega
	fclose(f);

	exit(EXIT_SUCCESS);
}