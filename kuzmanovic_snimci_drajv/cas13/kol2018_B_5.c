#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

// ekstenzija
char *ext = NULL;

int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	// proveravamo da li je regularan fajl
	if (typeflag == FTW_F) {
		if (ftwbuf->level >= 2 && ftwbuf->level <= 5) {
			// dirA/dirB/dirC/test.c
			char *tacka = strrchr(fpath + ftwbuf->base, '.');
			if (tacka != NULL) {
				if (strcmp(tacka, ext) == 0) {
					printf("%s\n", fpath + ftwbuf->base);
				}
			}
		}	
	}

	return 0;
}

int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	ext = argv[2];

	// provera da li se radi o direktorijumu
	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	// pokrecemo obilazak direktorijuma
	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");

	exit(EXIT_SUCCESS);
}