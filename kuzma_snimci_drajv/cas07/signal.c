#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int sigusr1Flag = 0;
int sigusr2Flag = 0;
int stop = 0;

// ovde nikada ne pozivati printf, scanf i slicno...
// jedino sto ovde u handler-u radimo to je postavljanje flag-ova
void processSignal(int signum) {

	switch (signum) {
		case SIGUSR1:
			sigusr1Flag = 1;
			break;
		case SIGUSR2:
			sigusr2Flag = 1;
			break;
		case SIGTERM:
			stop = 1;
			break;
	}
}

int main(int argc, char **argv) {

	(void)argc;
	(void)argv;

	do {

		check_error(signal(SIGUSR1, processSignal) != SIG_ERR, "signal");
		check_error(signal(SIGUSR2, processSignal) != SIG_ERR, "signal");
		check_error(signal(SIGTERM, processSignal) != SIG_ERR, "signal");

		// blokiraj se sve dok se ne desi neki signal, necemo da radimo aktivno cekanje
		pause();

		if (sigusr1Flag) {
			printf("Uhvatio sam SIGUSR1\n");
			sigusr1Flag = 0;
		}

		if (sigusr2Flag) {
			printf("Uhvatio sam SIGUSR2\n");
			sigusr2Flag = 0;
		}

		if (stop) {
			printf("Uhvatio sam SIGTERM\n");
		}

	} while (!stop);


	exit(EXIT_SUCCESS);
}