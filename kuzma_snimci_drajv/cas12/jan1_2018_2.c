#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

// ono sto treba da zna svaka nit je putanja do fajla i sekvenca koju treba da obradjuje
// NAJOPASNIJA GRESKA OVDE: otvaramo jedan fajl strim koji cemo da delimo sa svim nitima

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

// sada treba da vidimo sta svaka nasa nit treba da zna da bi mogla dalje da radi
typedef struct {

	char *fPath;
	char *sekv;
	int start;
	int len;

} inArg_t;

// globalna suma koju treba da dele sve niti
int globalSum = 0;

// ono sto treba da uradimo jeste da sinhronizujemo to, pa pravimo katanac
pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

void *threadFunc(void *argument) {

	// prvo kupimo informacije iz argumenta koji smo dobili
	inArg_t *arg = (inArg_t*)argument;

	// treba prvo da otvorimo fajl da bi sa njim bilo sta mogli da radimo
	FILE *f = fopen(arg->fPath, "r");
	check_error(f != NULL, "fopen");

	// sada treba da se postavimo na startnu poziciju
	check_error(fseek(f, arg->start, SEEK_SET) != -1, "fseek");

	// sada gledamo koliko smo karaktera "pojeli" do sada
	int n = arg->len;
	char *sadrzaj = malloc(n * sizeof(char));
	check_error(sadrzaj != NULL, "malloc");

	for (int i = 0; i < n; i++) {
		sadrzaj[i] = fgetc(f);
		if (sadrzaj[i] == EOF) {
			n = i - 1;
			break;
		}
	}

	int ds = strlen(arg->sekv);
	for (int i = 0; i < n; i++) {
		int j;
		for (j = 0; j < ds; j++) {
			if (i + j >= n || arg->sekv[j] != sadrzaj[i+j]) {
				break;
			}
		}

		// prvi slucaj je da sam iskocio iz petlje pre nego sto sam stigao do kraja sekvence, a drugi slucaj je
		// da sam stigao do kraja sekvence

		if (j >= ds) {
			// mutex zakljucavamo i otkljucavamo samo u onim delovima gde koristimo deljenu promenljivu
			check_pthread(pthread_mutex_lock(&globalLock), "pthread_mutex_lock");
			globalSum += 1;
			check_pthread(pthread_mutex_unlock(&globalLock), "pthread_mutex_unlock");
		}
	}

	free(sadrzaj);

	fclose(f);
	return NULL;
}

int main(int argc, char **argv) {

	check_error(argc == 4, "argc");

	int K = atoi(argv[3]);

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	int N = (int)fInfo.st_size;

	pthread_t *tids = malloc(K * sizeof(pthread_t));
	check_error(tids != NULL, "malloc");

	inArg_t *args = malloc(K * sizeof(inArg_t));
	check_error(args != NULL, "malloc");

	int len = N / K;

	for (int i = 0; i < K; i++) {
		args[i].fPath = argv[1];
		args[i].sekv = argv[2];
		args[i].start = i * len;
		args[i].len = len;
		check_pthread(pthread_create(&tids[i], NULL, threadFunc, &args[i]), "pthread_create");
	}

	// cekamo da se nase niti zavrse
	for (int i = 0; i < K; i++) {
		// nasa nit ne vraca nista ne treba nam onda drugi argument
		check_pthread(pthread_join(tids[i], NULL), "pthread_join");
	}

	printf("%d\n", globalSum);

	check_pthread(pthread_mutex_destroy(&globalLock), "pthread_mutex_destroy");
	free(tids);
	free(args);

	exit(EXIT_SUCCESS);
}