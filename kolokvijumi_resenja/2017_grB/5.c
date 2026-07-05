#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

bool walk(char *path, char *ext, unsigned *number) {

	struct stat fInfo;

	if (lstat(path, &fInfo) == -1) {
		return false;
	}

	if (S_ISREG(fInfo.st_mode)) {
		char *tmpStr = malloc(strlen(path));
		check_error(tmpStr != NULL, "malloc");
		tmpStr = strrchr(path, '.');
		if (tmpStr != NULL) {
			if (!strcmp(tmpStr, ext)) {
				*number += 1;
			}
		}
	}

	if (!S_ISDIR(fInfo.st_mode)) {
		return true;
	}

	DIR *dir = opendir(path);
	if (dir == NULL) {
		return false;
	}

	struct dirent *cd;

	while ((cd = readdir(dir)) != NULL) {

		char *newPath = malloc(strlen(path) + 1 + strlen(cd->d_name) + 1);
		check_error(newPath != NULL, "malloc");

		strcpy(newPath, path);
		strcat(newPath, "/");
		strcat(newPath, cd->d_name);

		if (!strcmp(cd->d_name, ".") || !strcmp(cd->d_name, "..")) {
			if (stat(newPath, &fInfo) == -1) {
				free(newPath);
				return false;
			}
			free(newPath);
			continue;
		}

		bool state = walk(newPath, ext, number);
		if (!state) {
			return false;
		}
	}

	if (closedir(dir) == -1) {
		return false;
	}

	return true;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	char *ext = malloc(strlen(argv[2]) + 1);
	check_error(ext != NULL, "malloc");

	strcpy(ext, argv[2]);

	unsigned number = 0;

	check_error(walk(argv[1], ext, &number), "walk");
	printf("%u\n", number);

	free(ext);

	exit(EXIT_SUCCESS);
}