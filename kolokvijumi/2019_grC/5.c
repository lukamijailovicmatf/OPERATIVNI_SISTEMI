#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define DAY (24*60*60)

bool walk(char *path) {

	struct stat fInfo;

	if (lstat(path, &fInfo) == -1) {
		return false;
	}

	if (S_ISREG(fInfo.st_mode)) {
		time_t diff = time(NULL) - (time_t)fInfo.st_mtim.tv_sec;
		if (diff <= 5 * DAY) {
			char *copy = malloc(strlen(path) + 1);
			copy = strrchr(path, '/');
			printf("%s\n", copy + 1);
		}
	}

	if (!S_ISDIR(fInfo.st_mode)) {
		return true;
	}

	DIR *dir = opendir(path);

	if (dir == NULL) {
		return false;
	}

	struct dirent *currentDir;

	while ((currentDir = readdir(dir)) != NULL) {

		char *newPath = malloc(strlen(path) + 1 + strlen(currentDir->d_name) + 1);
		check_error(newPath != NULL, "malloc");
		strcpy(newPath, path);
		strcat(newPath, "/");
		strcat(newPath, currentDir->d_name);

		if (!strcmp(currentDir->d_name, ".") || !strcmp(currentDir->d_name, "..")) {
			if(stat(newPath, &fInfo) == -1) {
				free(newPath);
				return false;
			}
			free(newPath);
			continue;
		}

		bool state = walk(newPath);
		free(newPath);

		if (state == false) {
			return false;
		}
	}

	if (closedir(dir) == -1) {
		return false;
	}

	return true;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(!S_ISDIR(fInfo.st_mode), "not a directory");

	check_error(!walk(argv[1]), "walk failure");

	exit(EXIT_SUCCESS);
}