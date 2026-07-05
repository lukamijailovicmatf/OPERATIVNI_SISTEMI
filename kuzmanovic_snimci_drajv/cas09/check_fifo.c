#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int checkRD(char *path) {

	int fd = open(path, O_RDONLY | O_NONBLOCK);
	check_error(fd != -1, "open");

	char c;
	int retVal = read(fd, &c, 1);

	if (retVal == 0) {

		retVal = 0;

	} else if (retVal == -1) {

		if (errno != EAGAIN) {
			check_error(0, "read");
		}

		retVal = 1;

	} else {

		// read je ucitao neki sadrzaj
		retVal = 1;
	}

	close(fd);

	return retVal;
}

// sada gledamo sta ako je neko otvorio fajl u READ modu, jer zelimo da ga otvorimo u WRITE modu
int checkWR(char *path) {

	int fd = open(path, O_WRONLY | O_NONBLOCK);

	if (fd == -1) {

		if (errno != ENXIO) {

			// desila se ozbiljna greska
			check_error(0, "open");

		}

		// znak da niko nije otvorio fajl
		return 0;

	} else {

		// u slucaju da smo uspeli da otvorimo fajl tada samo treba da ga zatvorimo
		close(fd);

	}

	return 1;
}

// ./a.out -r|-w path
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	if (strcmp(argv[1], "-r") == 0) {

		if (!checkRD(argv[2])) {

			printf("Niko nije otvorio fajl u w modu\n");

		} else {

			printf("Neko je otvorio fajl u w modu\n");

		}

	} else if (strcmp(argv[1], "-w") == 0) {


		if (!checkWR(argv[2])) {

			printf("Niko nije otvorio fajl u r modu\n");

		} else {

			printf("Neko je otvorio fajl u r modu\n");

		}

	} else {

		check_error(0, "...");

	}

	exit(EXIT_SUCCESS);
}