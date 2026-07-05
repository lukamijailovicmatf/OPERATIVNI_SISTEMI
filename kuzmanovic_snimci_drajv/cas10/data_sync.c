#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

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

#define MAX_SIZE (1024)

typedef struct {

	int idx;

} inputArgs_t;

// broj elemenata u vektoru
int globalCount = 0;

// vektor gde pakujemo vrednosti
int globalArray[MAX_SIZE];

pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

// uslovna globalna promenljiva koja sluzi za uslovno cekanje niti
pthread_cond_t globalCond = PTHREAD_COND_INITIALIZER;

void *threadFunc(void *arg) {

	inputArgs_t *inArg = (inputArgs_t*)arg;

	while (1) {

		// posto je globalCount deljena promenljiva izmedju svih niti prvo treba da zakljucamo
		// mutex pa tek onda smemo da vrsimo provere i pitamo nesto
		check_pthread(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");

		// dok god niko nije uneo niz cekaj na uslovu da se promeni mutex
		while (globalCount == 0) {
			check_pthread(pthread_cond_wait(&globalCond, &globalLock), "pthread_cond_wait");
		}

		printf("Nit %d se aktivira\n", inArg->idx);

		// sta ako je vreme za kraj
		if (globalCount == -1) {
			check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
			break;
		}

		int skalarni = 0;
		for (int i = 0; i < globalCount; i++) {
			skalarni += globalArray[i] * globalArray[i];
		}

		globalCount = 0;

		printf("%d. Skalarni proizvod: %d\n", inArg->idx, skalarni);

		check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
	}

	printf("Zavrsava se %d. nit\n", inArg->idx);

	return NULL;
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int numThreads = atoi(argv[1]);

	pthread_t *tids = malloc(numThreads * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	// inicijalizujemo argumente
	inputArgs_t *args = malloc(numThreads * sizeof(inputArgs_t));
	check_error(args != NULL, "malloc");

	// sada treba da pokrenemo sve niti
	for (int i = 0; i < numThreads; i++) {
		args[i].idx = i;
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &args[i]), "pthread_create");
	}

	// logika koja implementira nasu glavnu nit
	while (globalCount != -1) {

		// zakljucavamo da bi inicijalizovali niz i broj promenljivih
		check_pthread(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");

		if (globalCount != 0) {
			check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
			continue;
		}

		scanf("%d", &globalCount);

		if (globalCount == -1) {

			// ovde pustamo mutex
			check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");

			// obavestavamo sve niti odjednom da je vreme da se otkljucaju i da zavrse posao
			check_pthread(pthread_cond_broadcast(&globalCond), "pthread_cond_broadcast");

			continue;
		}

		// a ako nije -1 onda redom ucitavamo niz
		for (int i = 0; i < globalCount; i++) {
			scanf("%d", &globalArray[i]);
		}

		printf("Zavrsio ucitavanje\n");

		check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");

		// obavestavamo sada samo jednu nit da se oslobodi
		check_pthread(pthread_cond_signal(&globalCond), "pthread_cond_signal");
	}

	// cekamo da se svaka od niti zavrsi
	for (int i = 0; i < numThreads; i++) {
		check_pthread(pthread_join(tids[i], NULL), "pthread_join");
	}

	free(tids);
	free(args);

	check_pthread(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
	check_pthread(pthread_cond_destroy(&globalCond), "pthread_cond_destroy");

	exit(EXIT_SUCCESS);
}