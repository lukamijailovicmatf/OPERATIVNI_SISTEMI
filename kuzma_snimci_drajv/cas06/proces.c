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

int main(int argc, char **argv){

	(void)argc;
	(void)argv;

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid > 0) {
		// parent grana
		printf("Parent: \n\tMy ID: %d\n\tChild ID: %d\n", (int)getpid(), (int)childPid);
	} else {
		// child grana
		printf("Child: \n\tMyID: %d\n\tParent ID: %d\n", (int)getpid(), (int)getppid());
		exit(EXIT_SUCCESS);
	}

	// printf("Ovo stampaju oba procesa\n");
	printf("Ovo stampa samo parent\n");

	// cekanje
	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if (WIFEXITED(status)) { // da li se program zavrsio kontrolisano (sam je pozvao exit)
		// provera exit koda
		if (WEXITSTATUS(status) == EXIT_SUCCESS) {
			printf("Uspeh\n");
		} else {
			printf("Neuspeh\n");
		}
	} else { // provera exit koda nema smisla ako dete proces nije pozvalo exit
		printf("Neuspeh\n");
	}

	printf("Child se zavrsio\n");

	exit(EXIT_SUCCESS);
}