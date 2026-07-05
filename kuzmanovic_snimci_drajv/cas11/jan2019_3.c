#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>

/* PAZNJA: treba da vodimo racuna o tome da li veci broj niti treba da pristupi i ne samo da pristupi
   nego i da izmeni sadrzaj u deljenom resursu. Ako to postoji neophodna nam je sinhronizacija tj.
   neki mutex ili slicno. Na kraju svega je potrebno da oslobodimo sve sto smo alocirali. U funkciji
   za nit povratna vrednost je alocirana na hip-u i onda tu povratnu vrednost treba dealocirati */

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

	// na osnovu indeksa mozemo da znamo koliko parce vektora nit moze da obradjuje
	int idx;
	int numVect;

} inputArg_t;

typedef struct {

	int idx;
	double norma;

} outputArg_t;

int m, n, k;
double **vektori;

void *threadFunc(void *a) {

	// prvi korak pokupiti argument
	inputArg_t *arg = (inputArg_t*)a;

	// treba nam pocetna pozicija sa koje krecemo
	int startIdx = arg->numVect * arg->idx;
	int endIdx = startIdx + arg->numVect;

	double maxNorma = -1;

	for (int i = startIdx; i < endIdx; i++) {
		// ono sto ovde treba da uradimo je da nadjemo samu nit
		double norma = 0;
		for (int j = 0; j < n; j++) {
			norma += vektori[i][j] * vektori[i][j];
		}

		norma = sqrt(norma);

		if (norma > maxNorma) {
			maxNorma = norma;
		}
	}

	outputArg_t *out = malloc(sizeof(outputArg_t));
	check_error(out != NULL, "malloc");

	out->idx = arg->idx;
	out->norma = maxNorma;

	return out;
}

int main(int argc, char **argv) {

	scanf("%d%d%d", &m, &n, &k);

	vektori = malloc(m * sizeof(double*));
	check_error(vektori != NULL, "malloc");

	// sada alociramo svaki pojedinacni vektor
	for (int i = 0; i < m; i++) {
		vektori[i] = malloc(n * sizeof(double));
		check_error(vektori[i] != NULL, "malloc");
	}

	// vrsimo ucitavanje u vektore
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%lf", &vektori[i][j]);
		}
	}

	/* pravimo dalje aplikaciju sa nitima tako da svaka racuna m/k vektora */

	// alociramo prostor za id-jeve
	pthread_t *tids = malloc(k * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	/* sada treba da kreiramo same niti
	   ali treba da vodimo racuna o tome da svaka nit treba da zna odakle treba da krene i to pakujemo kao
	   argument koji saljemo niti */

	// alociramo same argumente
	inputArg_t *args = malloc(k * sizeof(inputArg_t));
	check_error(args != NULL, "malloc");

	for (int i = 0; i < k; i++) {
		args[i].idx = i;
		args[i].numVect = m / k;
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &args[i]), "pthread_create");
	}

	// na kraju deo koji hvata sam zavrsetak
	double maxNorma = -1;
	int maxIdx = -1;

	for (int i = 0; i < k; i++) {

		outputArg_t *retVal = NULL;
		check_pthread(pthread_join(tids[i], (void**)&retVal), "pthread_join");

		if (retVal->norma > maxNorma) {
			maxNorma = retVal->norma;
			maxIdx = retVal->idx;
		}

		free(retVal);
	}

	printf("%d %lf\n", maxIdx, maxNorma);

	free(tids);
	free(args);

	for (int i = 0; i < m; i++) {
		free(vektori[i]);
	}

	free(vektori);

	exit(EXIT_SUCCESS);
}