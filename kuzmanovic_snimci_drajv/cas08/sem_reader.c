#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (1024)

typedef struct {
	sem_t semWrite;
	sem_t semRead;
	char userMsg[MAX_SIZE];
} sharedType_t;

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
	sharedType_t *memBlock = getMemBlock(argv[1], &size);

	char buffer[MAX_SIZE];

	do {

		// kriticna sekcija
		check_error(sem_wait(&(memBlock->semRead)) != -1, "sem_wait");
		strcmp(buffer, memBlock->userMsg);
		check_error(sem_post(&(memBlock->semWrite)) != -1, "sem_post");

		printf("Message from writer: %s\n", buffer);

	} while (strcmp(buffer, "quit"));

	check_error(munmap(memBlock, size) != -1, "munmap");

	exit(EXIT_SUCCESS);
}