#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
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

void traverseDir(char *path, unsigned *size) {

	struct stat fInfo;
	check_error(lstat(path, &fInfo) != 	-1, "lstat");

	*size += fInfo.st_size;

	if (!S_ISDIR(fInfo.st_mode))
		return;

	// mehanizam za otkrivanje gde se trenutno nalazimo
	// zapravo zelimo da dobijemo apsolutnu putanju
	char *dirPath = realpath(path, NULL);
	printf("%s\n", dirPath);
	free(dirPath);

	DIR *dir = opendir(path);
	check_error(dir != NULL, "opendir");

	// sada je potrebno da udjemo u direktorijum koji je na datoj putanji
	check_error(chdir(path) != -1, "chdir");

	// sada treba da listam direktorijum koji je otvoren i sada fajlove zovem imenima jer
	// je njihovo ime ekvivalentno njihovoj putanji
	struct dirent *dirEntry = NULL;
	errno = 0;

	while ((dirEntry = readdir(dir)) != NULL) {

		char *fajl = realpath(dirEntry->d_name, NULL);
		printf("\tFajl: %s : %s\n", fajl, dirEntry->d_name);
		free(fajl);

		// sada je ime tog fajla je upravo i njegova putanja
		if (!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")) {
			// u ovom slucaju treba da saznamo samo velicinu
			check_error(stat(dirEntry->d_name, &fInfo) != -1, "stat");
			*size += fInfo.st_size;
			errno = 0;
			continue;
		}

		// pozivamo rekurzivno funkciju za dalje
		traverseDir(dirEntry->d_name, size);
		errno = 0;
	}

	// provera da li je razlog izlaska to sto se dogodila greska ili to sto se zaista stiglo do kraja
	// direktorijuma
	check_error(errno != EBADF, "readdir");

	// nakon sto sam zavrsio sa obilaskom treba da napustim direktorijum
	check_error(chdir("..") != -1, "chdir out");

	// higijene radi zatvaramo stream da nam ne bi cureli deskriptori
	check_error(closedir(dir) != -1, "closedir");
}

// ./a.out imeFajla
int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	// proveravamo da li je uopste u pitanju direktorijum
	check_error(S_ISDIR(fInfo.st_mode), "not a dir");

	unsigned size = 0;
	traverseDir(argv[1], &size);

	printf("Size of dir: %d\n", size);

	exit(EXIT_SUCCESS);
}