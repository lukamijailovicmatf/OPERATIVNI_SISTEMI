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
	check_error(stat(path, &fInfo) != -1, "stat");

	*size += fInfo.st_size;

	if (!S_ISDIR(fInfo.st_mode))
		return;

	DIR* dir = opendir(path);
	check_error(dir != NULL, "opendir");

	struct dirent *dirEntry;

	errno = 0;

	while ((dirEntry = readdir(dir)) != NULL) {

		// putanja do svakog pojedinacnog fajla mora da se napravi
		char *putanja = malloc(strlen(path) + strlen(dirEntry->d_name) + 2);
		check_error(putanja != NULL, "malloc");

		strcpy(putanja, path);
		strcat(putanja, "/");
		strcat(putanja, dirEntry->d_name);

		// sprintf(putanja, "%s/%s", path, dirEntry->d_name);

		// za direktorijume . i .. ne vrsimo rekurziju vec odmah uracunavamo size
		if (!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")) {
			check_error(stat(putanja, &fInfo) != -1, "stat");
			*size += fInfo.st_size;
			free(putanja);
			continue;
		}

		traverseDir(putanja, size);
		free(putanja);
		errno = 0;
	}

	check_error(errno != EBADF, "readdir");
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