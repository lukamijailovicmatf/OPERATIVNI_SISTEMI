#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

int main(int argc, char **argv) {

	(void)argc;
	(void)argv;

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid == 0) { // child
		check_error(execlp("/home/me/Desktop/3. godina/OS/KUZMA/cas06/pipes.c", "pipes", NULL) != -1, "execlp");
		exit(EXIT_FAILURE);
	}

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if (WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)) {
		printf("Uspeh\n");
	} else {
		printf("Neuspeh\n");
	}

	exit(EXIT_SUCCESS);
}