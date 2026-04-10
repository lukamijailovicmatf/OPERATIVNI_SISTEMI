#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (256)

void *createMemBlock(char *fpath, unsigned size) {

	// 0600 - samo moji procesi mogu da pristupaju tom delu deljene memorije
	int memFd = shm_open(fpath, O_RDWR | O_CREAT, 0600);
	check_error(memFd != -1, "shm_open");

	// postavljamo velicinu fajla
	check_error(ftruncate(memFd, size) != -1, "ftruncate");

	// mapiramo sada potreban prostor
	void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
	check_error(addr != MAP_FAILED, "mmap");

	close(memFd);

	return addr;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int n = 10;
	int *array = createMemBlock(argv[1], n * sizeof(int));

	for (int i = 0; i < n; i++)
		array[i] = i;

	check_error(munmap(array, n * sizeof(int)) != -1, "munmap");

	exit(EXIT_SUCCESS);
}