#define _XOPEN_SOURCE (700) // ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
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

void *createMemBlock(char *fpath, unsigned size) {

	int memFd = shm_open(fpath, O_RDWR | O_CREAT, 0600);
	check_error(memFd != -1, "shm_open");

	check_error(ftruncate(memFd, size) != -1, "ftruncate");

	void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
	check_error(addr != MAP_FAILED, "mmap");

	close(memFd);

	return addr;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	sharedType_t *memBlock = createMemBlock(argv[1], sizeof(sharedType_t));

	check_error(sem_init(&(memBlock->semWrite), 1, 1) != -1, "sem_init");

	check_error(sem_init(&(memBlock->semRead), 1, 0) != -1, "sem_init");

	char buffer[MAX_SIZE];

	do {

		// ovo nema veze sa deljenom memorijom
		printf("Message to send:\n");
		scanf("%s", buffer);

		// kriticna sekcija
		check_error(sem_wait(&(memBlock->semWrite)) != -1, "sem_wait");
		strcpy(memBlock->userMsg, buffer);
		check_error(sem_post(&(memBlock->semRead)) != -1, "sem_post");

	} while (strcmp(buffer, "quit"));

	check_error(munmap(memBlock, sizeof(sharedType_t)) != -1, "munmap");

	check_error(shm_unlink(argv[1]) != -1, "shm_unlink");

	exit(EXIT_SUCCESS);
}