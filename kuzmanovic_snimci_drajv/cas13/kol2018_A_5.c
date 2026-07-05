#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
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

int maxSize = 0;
int minSize = -1;

void traverseDir(char *fpath) {

	// tokom obilaska nama ne treba informacija na sta pokazuje simbolicki link ako on postoji
	// vec samo o tom simbolickom linku koji se nalazi u direktorijumu
	struct stat fInfo;
	check_error(lstat(fpath, &fInfo) != -1, "lstat");

	// od interesa nam je da li je fajl regularan tj. zanimaju nas samo regularni fajlovi
	if (S_ISREG(fInfo.st_mode)) {

		if (fInfo.st_size > maxSize) {
			maxSize = fInfo.st_size;
		}

		if (minSize == -1) {
			minSize = fInfo.st_size;
		} else {
			if (fInfo.st_size < minSize) {
				minSize = fInfo.st_size;
			}
		}
	}

	// ako se ne radi o direktorijumu zaustavljamo rekurziju
	if (!S_ISDIR(fInfo.st_mode)) {
		return;
	}

	// ako jesi direktorijum idemo u rekurzivni obilazak
	DIR *dir = opendir(fpath);
	check_error(dir != NULL, "opendir");

	// obilazak
	struct dirent *dirEntry;
	while ((dirEntry = readdir(dir)) != NULL) {

		// preskacemo . i ..
		if (!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")) {
			continue;
		}

		// u suprotnom treba da napravimo putanju do fajla do kojeg smo stigli i da rekurzivno
		// pozovemo funkciju koja ce njega da obradi
		int n = strlen(fpath) + strlen(dirEntry->d_name) + 2;
		char *novaPutanja = malloc(n * sizeof(char));
		check_error(novaPutanja != NULL, "malloc");

		// pravimo novu putanju
		sprintf(novaPutanja, "%s/%s", fpath, dirEntry->d_name);

		// sada rekurzivno obilazimo sledeci fajl
		traverseDir(novaPutanja);

		// kada se obidje ta cela podstruktura potrebno je osloboditi tu novu putanju
		// koju smo napravili da ne bi curela memorija
		free(novaPutanja);
	}

	closedir(dir);
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISDIR(fInfo.st_mode), "not a directory");

	traverseDir(argv[1]);

	printf("%d\n", maxSize - minSize);

	exit(EXIT_SUCCESS);
}