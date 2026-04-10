#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

void *getMemBlock(char *fpath, unsigned *size) {

	int memFd = shm_open(fpath, O_RDWR, 0600);
	check_error(memFd != -1, "shm_open");

	struct stat fInfo;
	check_error(fstat(memFd, &fInfo) != -1, "fstat");
	*size = fInfo.st_size;

	void *addr = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
	check_error(addr != MAP_FAILED, "mmap");

	close(memFd);

	return addr;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	unsigned size = 0;
	int *array = getMemBlock(argv[1], &size);
	int n = size / sizeof(int);

	for (int i = 0; i < n; i++)
		printf("%d ", array[i]);

	printf("\n");

	// radimo sada odmapiranje prostora
	check_error(munmap(array, size) != -1, "munmap");

	check_error(shm_unlink(argv[1]) != -1, "shm_unlink");

	exit(EXIT_SUCCESS);
}