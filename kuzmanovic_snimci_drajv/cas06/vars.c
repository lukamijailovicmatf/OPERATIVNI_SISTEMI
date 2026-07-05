#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

	int x = 10;
	int y = 5;

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid > 0) { // parent
		x += 20; // 30
		y -= 20; // -15
		printf("Parent: %d %d\n", x, y);
	} else { // child
		x -= 20; // -10
		y += 20; // 25
		printf("Child: %d %d\n", x, y);
		exit(EXIT_SUCCESS);
	}

	// parent ponovo proverava vrednosti x i y ako se prvo pokrenuo parent pa child da vidimo da
	// child nije imao uticaj na nase promenljive
	printf("Parent again: %d %d\n", x, y);

	// moramo da se osiguramo da nemamo zombije (zombi procese) a to radimo tako sto proverimo status
	int status = 0;
	check_error(wait(&status) != -1, "wait");

	// prvo se proverava da li se proces zavrsio kontrolisano (WEXITED), 
	// a druga stvar je provera exit koda (WEXITSTATUS)
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
		printf("Uspeh\n");
	} else {
		printf("Neuspeh\n");
	}

	exit(EXIT_SUCCESS);
}