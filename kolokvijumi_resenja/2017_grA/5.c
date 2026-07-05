#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int reg = 0, dir = 0, chr = 0, blk = 0, lnk = 0, fifo = 0, sock = 0, total = 0;

int func(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	switch (sb->st_mode & S_IFMT) {

		case S_IFREG:
			reg++;
			break;

		case S_IFDIR:
			dir++;
			break;

		case S_IFCHR:
			chr++;
			break;

		case S_IFBLK:
			blk++;
			break;

		case S_IFLNK:
			lnk++;
			break;

		case S_IFIFO:
			fifo++;
			break;

		case S_IFSOCK:
			sock++;
			break;

		default:
			break;
	}

	total++;

	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	check_error(nftw(argv[1], func, 50, FTW_PHYS) != -1, "nftw");

	fprintf(stdout, "%i %i %i %i %i %i %i %i\n", reg, dir, chr, blk, lnk, fifo, sock, total);

	exit(EXIT_SUCCESS);
}