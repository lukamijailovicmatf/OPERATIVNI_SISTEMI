#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ftw.h>
#include <stdbool.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

char newDir[2048];

bool osIsPublicFile(const char *path) {

	struct stat fInfo;
	check_error(stat(path, &fInfo) != -1, "stat");

	if ((fInfo.st_mode & S_IROTH) && (fInfo.st_mode & S_IWOTH))
		return true;

	return false;
}

void osMkPublicDir(const char *dname) {

	check_error(mkdir(dname, 0777) != -1, "mkdir");

	check_error(chmod(dname, 0777) != -1, "chmod");

	return;
}

unsigned osNumOfDaysFileModified(const char *fpath) {

	struct stat fInfo;
	check_error(stat(fpath, &fInfo) != -1, "stat");

	time_t now = time(NULL);
	check_error(now != (time_t)-1, "time");

	time_t diff = now - fInfo.st_mtime;

	if (diff < 0) {
		printf("Pogresna razlika vremena.\n");
	}

	return (diff / 3600) / 24;
}

void osMoveFile(const char *srcPath, const char *destPath) {

	int fd1 = open(srcPath, O_RDONLY);
	check_error(fd1 != -1, "open");

	int fd2 = open(destPath, O_WRONLY | O_CREAT | O_TRUNC);
	check_error(fd2 != -1, "open");

	char buffer[2048];
	char readBytes = 0;

	while ((readBytes = read(fd1, buffer, 2048)) > 0) {
		check_error(write(fd2, buffer, readBytes) != -1, "write");
	}

	check_error(readBytes != -1, "read");

	check_error(unlink(srcPath) != -1, "unlink");

	close(fd1);
	close(fd2);

	return;
}

int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	if (S_ISREG(sb->st_mode)) {
		if (osIsPublicFile(fpath)) {
			if (osNumOfDaysFileModified(fpath) < 30) {
				char *tmp = malloc(strlen(newDir) + strlen(fpath + ftwbuf->base) + 2);
				check_error(tmp != NULL, "malloc");
				strcpy(tmp, newDir);
				strcat(tmp, "/");
				strcat(tmp, fpath + ftwbuf->base);
				osMoveFile(fpath, tmp);
				free(tmp);
			} else {
				check_error(unlink(fpath) != -1, "unlink");
			}
		}
	}

	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISREG(fInfo.st_mode), "not a regular file");

	strcpy(newDir, argv[2]);

	osMkPublicDir(argv[2]);

	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");

	exit(EXIT_SUCCESS);
}