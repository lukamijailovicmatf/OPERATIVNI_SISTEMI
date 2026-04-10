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

int globalSum = 0;

// staticka alokacija mutex-a
pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {

	int idx;
	char *putanja;

} inputArgs_t;

void *threadFunc(void *arg) {

	inputArgs_t *inArg = (inputArgs_t*)arg;

	// svaka nit mora da otvori svoj fajl strim
	FILE *f = fopen(inArg->putanja, "r");
	check_error(f != NULL, "fopen");

	int localSum = 0;

	while (fgetc(f) != EOF) {
		localSum++;
	}

	printf("%s: %d\n", inArg->putanja, localSum);

	// kada smo procitali ceo fajl tek tada treba da dodam taj broj karaktera koji je procitan u fajlu
	// na globalnu sumu
	check_pthread(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
	globalSum += localSum;
	check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");

	fclose(f);

	return NULL;
}

int main(int argc, char **argv) {

	check_error(argc > 1, "argc");

	// broj niti
	int numThreads = argc - 1;

	// id-evi za niti (svaka nit ima svoj id)
	pthread_t *tids = malloc(numThreads * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	// alociramo broj argumenata koliko niti ima (svaka nit cita drugi fajl i svaka nit ima drugaciji indeks)
	inputArgs_t *threadArgs = malloc(numThreads * sizeof(inputArgs_t));
	check_error(threadArgs != NULL, "malloc");

	globalSum = 0;

	for (int i = 0; i < numThreads; i++) {

		// prvo inicijalizacija argumenata
		threadArgs[i].idx = i;

		// pa onda inicijalizujemo putanju do fajla koju dobija nasa nit i obradjuje
		threadArgs[i].putanja = argv[i+1];

		// sada treba da pokrenemo nit
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &threadArgs[i]), "pthread_create");
	}

	// sledeci korak je da sacekamo da se sve pokrenute niti zavrse
	for (int i = 0; i < numThreads; i++) {

		check_pthread(pthread_join(tids[i], NULL), "pthread_join");
	}

	printf("Ukupno karaktera: %d\n", globalSum);

	// na kraju oslobadjamo sve resurse
	free(tids);
	free(threadArgs);
	check_pthread(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");

	exit(EXIT_SUCCESS);
}