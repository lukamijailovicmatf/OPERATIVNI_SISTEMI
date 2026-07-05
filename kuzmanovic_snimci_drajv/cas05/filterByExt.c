#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

char *Ext = NULL;

// potpis funkcije nftw
// int nftw(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb,
// int typeflag, struct FTW *ftwbuf), int nopenfd, int flags);

// funkcija koju prosledjujemo pozivu nftw
// int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

	(void)typeflag;

	// proveravamo da li je regularni fajl jer na linux-u i direktorijumi mogu imati
	// ekstenziju
	if (S_ISREG(sb->st_mode)) {
		// provera ekstenzije
		char *ime = fpath + ftwbuf->base;
		char *ext = strrchr(ime, '.'); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! lako se dobija SEGMENTATION FAULT

		// mora da se proveri povratna vrednost strrchr
		// tacka ne mora da bude deo imena
		if (ext != NULL) {
			//mogu da proveravam ekstenziju
			if (strcmp(ext, Ext) == 0) {
				printf("%s: %s\n", fpath, fpath + ftwbuf->base);
			}
		}
	}

	// OBAVEZNO ovo jer inace puca poziv
	return 0;
}

// ./a.out imeFajla ekstenzija
int main(int argc, char **argv) {

	check_error(argc == 3, "argc");

	Ext = argv[2];

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	// treba da se osiguramo da se radi o direktorijumu
	check_error(S_ISDIR(fInfo.st_mode), "not a dir");

	// obilazak direktorijuma
	// poziv funkcije nftw
	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");

	exit(EXIT_SUCCESS);
}