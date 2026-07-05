#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define READ_END (0)
#define WRITE_END (1)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	int pipes[2];
	check_error(pipe(pipes) != -1, "pipe");

	// sada treba da napravimo sam proces
	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid == 0) {

		// child grana

		// zatvaramo stranu koja nam ne treba, a to je strana za citanje jer cemo da
		// pisemo po pajpu
		close(pipes[WRITE_END]);

		// sada nam treba preusmeravanje u smislu da komanda misli da pise po standardnom izlazu
		// a u stvari pise po nasem pajpu
		// dup2 - treba nam sta hocemo da preusmerimo npr pipes[WRITE_END] treba da postane STDOUT_FILENO
		check_error(dup2(pipes[WRITE_END], STDOUT_FILENO), "dup2");

		// 1. argument je putanja, 2. argument je poziv koji zelim 
		check_error(execlp("stat", "stat", "--format=%s", argv[1], NULL), "execlp");	
	}

	// zatvaramo stranu koja nam ne treba
	close(pipes[WRITE_END]);

	/* ono sto nam treba u parent-u je da sacekamo izlaz i da proverimo da li je sve uredu */

	char buffer[100];
	int readBytes = read(pipes[READ_END], buffer, 99);
	check_error(readBytes != -1, "read");

	buffer[readBytes] = 0;

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	// prvo proveravamo da li je program kontrolisano zavrsen pa tek onda exit code
	if (WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)) {
		printf("%s", buffer);
	} else {
		printf("Neuspeh\n");
	}

	exit(EXIT_SUCCESS);
}