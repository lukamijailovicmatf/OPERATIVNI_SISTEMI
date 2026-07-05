#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define check_pthread(errNum, userMsg)\
	do {\
		int _err = (errNum);\
		if (_err > 0) {\
			errno = _err;\
			check_error(0, userMsg);\
		}\
	} while (0)

typedef struct {

	int idx;

} inputArgs_t;

typedef struct {

	int v;

} outputArgs_t;

void *threadFunc(void *arg) {

	inputArgs_t *inArg = (inputArgs_t*)arg;

	printf("Hello from thread: %d\n", inArg->idx);

	outputArgs_t *outArg = malloc(sizeof(outputArgs_t));
	check_error(outArg != NULL, "malloc");

	outArg->v = (inArg->idx) * (inArg->idx);

	// ovo prekida izvrsavanje nase niti
	return outArg;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int numThreads = atoi(argv[1]);

	pthread_t *tids = malloc(numThreads * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	// alociramo niz argumenata
	inputArgs_t *threadArgs = malloc(numThreads * sizeof(inputArgs_t));
	check_error(threadArgs != NULL, "malloc");

	for (int i = 0; i < numThreads; i++) {

		threadArgs[i].idx = i;
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &threadArgs[i]), "pthread_create");
	}

	for (int i = 0; i < numThreads; i++) {

		outputArgs_t *retVal = NULL;
		check_pthread(pthread_join(tids[i], (void**)&retVal), "pthread_join");

		printf("Thread %d ended\n", retVal->v);
		free(retVal);
	}

	free(tids);
	free(threadArgs);

	exit(EXIT_SUCCESS);
}