#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
#define MAX_SIZE (1024)

int main(int argc, char **argv) {

	int cld2par[2];
	check_error(pipe(cld2par) != -1, "pipe");

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	int lineCount = 0;

	if (childPid > 0) {

		close(cld2par[WRITE_END]);

		FILE *f = fdopen(cld2par[READ_END], "r");
		check_error(f != NULL, "fdopen");

		// stavljamo na NULL da bi getline za nas sve alociralo
		char *line = NULL;
		size_t lineLen = 0;

		while (getline(&line, &lineLen, f) != -1) {
			lineCount++;
			printf("%s", line);
		}

		free(line);
		fclose(f);

	} else {

		close(cld2par[READ_END]);

		// hocemo da child-ova grana za pisanje postane stdout, tako da komanda kada bude pisala po standardnom
		// izlazu ona ce da pise po pajpu
		check_error(dup2(cld2par[WRITE_END], STDOUT_FILENO) != -1, "dup2");

		check_error(execlp("ls", "ls", "-l", NULL) != -1, "execlp");

		exit(EXIT_FAILURE);

	}

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	// proveravamo da li se program zavrsio KONTROLISANO i USPESNO
	if (WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)) {

		printf("Broj linija: %d\n", lineCount);

	} else {

		printf("Greska\n");
		
	}

	exit(EXIT_SUCCESS);
}