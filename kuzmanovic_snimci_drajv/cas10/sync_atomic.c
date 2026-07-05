#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>

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
	char *putanja;

} inputArgs_t;

atomic_int globalSum;

void *threadFunc(void *arg) {

	// prvo kupimo input argument
	inputArgs_t *inArg = (inputArgs_t*)arg;

	FILE *f = fopen(inArg->putanja, "r");
	check_error(f != NULL, "fopen");

	// u lokalnu sumu prebrojavamo karaktere
	int localSum = 0;

	// sada petljom citamo svaki karakter u fajlu koji smo otvorili
	while (fgetc(f) != EOF) {
		localSum++;
	}

	// sada na globalni zbir dodajemo izracunat lokalni za taj fajl
	atomic_fetch_add(&globalSum, localSum);

	printf("%s: %d\n", inArg->putanja, localSum);

	fclose(f);

	return NULL;
}

int main(int argc, char **argv) {

	check_error(argc > 1, "argc");

	atomic_init(&globalSum, 0);

	// saznajemo koliko nam niti treba
	int numThreads = argc - 1;

	// inicijalizujemo id - jeve
	pthread_t *tids = malloc(numThreads * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	// zatim pravimo niz za input argumente
	inputArgs_t *threadArgs = malloc(numThreads * sizeof(inputArgs_t));
	check_error(threadArgs != NULL, "malloc");

	// naredna stvar je da pokrenemo sve te niti
	for (int i = 0; i < numThreads; i++) {

		// inicijalizujemo id i putanju za nit
		threadArgs[i].idx = i;
		threadArgs[i+1].putanja = argv[i+1];

		// nakon inicijalizacije pokrecemo nit
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &threadArgs[i]), "pthread_create");
	}

	// stvar koja nam ostaje jeste da sacekamo sve nase niti
	for (int i = 0; i < numThreads; i++) {

		check_pthread(pthread_join(tids[i], NULL), "pthread_join");
	}

	printf("Ukupan broj karaktera: %d\n", atomic_load(&globalSum));

	// na kraju oslobadjamo resurse
	free(tids);
	free(threadArgs);

	exit(EXIT_SUCCESS);
}