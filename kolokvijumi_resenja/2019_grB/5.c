#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

bool should_print_files_f(char *path) {

	char *dirname = basename(path);

	char name_prefix[5] = {dirname[0], dirname[1], dirname[2], dirname[3], '\0'};

	return strcmp(name_prefix, "dir_") == 0;
}

void print_dir(char *path, bool should_print_files) {

	DIR *dir = opendir(path);
	check_error(dir != NULL, "opendir");

	check_error(chdir(path) != -1, "chdir");

	struct dirent *current_dir;

	while ((current_dir = readdir(dir)) != NULL) {

		struct stat fInfo;
		check_error(lstat(current_dir->d_name, &fInfo) != -1, "lstat");

		if (S_ISREG(fInfo.st_mode)) {
			if (should_print_files)
				printf("%s\n", current_dir->d_name);
		} else if (S_ISDIR(fInfo.st_mode)) {
			if (strcmp(current_dir->d_name, ".") != 0 && strcmp(current_dir->d_name, "..") != 0) {
				print_dir(current_dir->d_name, should_print_files_f(current_dir->d_name) || should_print_files);
			}
		}
	}

	closedir(dir);

	check_error(chdir("..") != -1, "chdir");
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	print_dir(argv[1], should_print_files_f(argv[1]));

	exit(EXIT_SUCCESS);
}