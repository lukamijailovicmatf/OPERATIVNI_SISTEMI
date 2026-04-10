#include <stdio.h>
#include <stdlib.h>
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
	} while(0)

int main(int argc, char **argv) {

	check_error(argc == 5, "argc");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	check_error(S_ISREG(fInfo.st_mode), "not regular file");

	int fd = open(argv[1], O_RDWR);
	check_error(fd != -1, "open");

	int a = atoi(argv[2]);
	int b = atoi(argv[3]);
	int n = atoi(argv[4]);

	char *buffA = malloc(n * sizeof(char));
	check_error(buffA != NULL, "malloc");

	char *buffB = malloc(n * sizeof(char));
	check_error(buffB != NULL, "malloc");

	// popunjavam buffA
	check_error(lseek(fd, a, SEEK_SET) != (off_t)-1, "lseek");
	check_error(read(fd, buffA, n) != -1, "read");

	// popunjavam buffB
	check_error(lseek(fd, b, SEEK_SET) != (off_t)-1, "lseek");
	check_error(read(fd, buffB, n) != -1, "read");

	// pisem iz buffB sadrzaj od pozicije a
	check_error(lseek(fd, a, SEEK_SET) != (off_t)-1, "lseek");
	check_error(write(fd, buffB, n) != -1, "write");

	// pisem iz buffA sadrzaj od pozicije b
	check_error(lseek(fd, b, SEEK_SET) != (off_t)-1, "lseek");
	check_error(write(fd, buffA, n) != -1, "write");

	free(buffA);
	free(buffB);

	exit(EXIT_SUCCESS);
}