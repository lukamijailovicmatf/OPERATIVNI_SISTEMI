#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define check_error(cond, msg)\
	do {\
		if(!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

// r - otvori za citanje, ako ne postoji fajl prijavi gresku
// w - otvori za pisanje, ako ne postoji kreiraj fajl, ako postoji kreiraj njegov sadrzaj
// a - otvori za nadovezivanje, ako ne postoji kreiraj fajl, ako postoji upisuj na kraj
// r+ - otvori za citanje, ali dozvoli upis, ako fajl ne postoji prijavi gresku
// w+ - otvori za pisanje, ali dozvoli citanje, ako fajl postoji obrisi sadrzaj, ako ne postoji kreiraj fajl
// a+ - otvori za nadovezivanje i dozvoli citanje (pisanjem se uvek sadrzaj dodaje na kraj fajla)
//		ako postoji upisuj na kraj, ako ne postoji kreiraj fajl

int my_fopen(char *path, char *mode) {

	int flags = 0;
	switch (mode[0]) {
		case 'r':
			flags |= mode[1] == '+' ? O_RDWR : O_RDONLY;
			break;
		case 'w':
			flags |= mode[1] == '+' ? O_RDWR : O_WRONLY;
			flags |= O_TRUNC;
			flags |= O_CREAT;
			break;
		case 'a':
			flags |= mode[1] == '+' ? O_RDWR : O_WRONLY;
			flags |= O_APPEND;
			flags |= O_CREAT;
			break;
		default:
			return -1;
	}

	// nasa zelja, sto ne znaci da cemo je dobiti
	mode_t pravaPristupa = 0777;

	int fd = open(path, flags, pravaPristupa);

	return fd;
}

// ./a.out path mode
int main(int argc, char **argv){
	
	check_error(argc == 3, "argc");

	int fd = my_fopen(argv[1], argv[2]);
	check_error(fd != -1, "my_fopen");

	close(fd);
	
	exit(EXIT_SUCCESS);
}
