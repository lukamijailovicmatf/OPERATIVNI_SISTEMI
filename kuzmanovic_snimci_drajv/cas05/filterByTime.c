#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ftw.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

time_t now;
//int n;

int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	// proveravamo da li se radi o regularnom fajlu
	if (typeflag == FTW_F) {
		// izdvajamo sve fajlove modifikovane u poslednjih 7 dana
		time_t diff = (now - sb->st_mtime) / (24*60*60);
		if (diff < 7) { // za n dana umesto 7 ide samo n
			printf("%s: %s\n", fpath, fpath + ftwbuf->base);
		}
	}

	// OBAVEZNO!!!
	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	//n = strtol(argv[2], NULL, 10);

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	// proveravamo da li je direktorijum
	check_error(S_ISDIR(fInfo.st_mode), "not a dir");

	// vreme izrazeno u sekundama od epohe
	// epoha: 1.1.1970.
	check_error(time(&now) != -1, "time");

	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");

	exit(EXIT_SUCCESS);
}