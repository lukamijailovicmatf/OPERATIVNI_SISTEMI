#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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

int main(int argc, char **argv){

	(void)argc;
	(void)argv;

	int cldToPar[2];
	check_error(pipe(cldToPar) != -1, "pipe");

	int parToCld[2];
	check_error(pipe(parToCld) != -1, "pipe");

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid > 0) { // parent

		// pisemo u roditelja u neki bafer i saljemo ka detetu preko pajpa
		close(parToCld[READ_END]);
		close(cldToPar[WRITE_END]);

		char buffer[MAX_SIZE];
		sprintf(buffer, "Zdravo");
		check_error(write(parToCld[WRITE_END], buffer, strlen(buffer)) != -1, "write");

		int readBytes = read(cldToPar[READ_END], buffer, MAX_SIZE);
		check_error(readBytes != -1, "read");

		// buffer[readBytes] = 0;
		// printf("%s\n", buffer);

		check_error(write(STDOUT_FILENO, buffer, readBytes) != -1, "write");

		close(parToCld[WRITE_END]);
		close(cldToPar[READ_END]);

	} else { // child

		// salje se sada suprotno od child-a do parent-a
		close(parToCld[WRITE_END]);
		close(cldToPar[READ_END]);

		char poruka[MAX_SIZE];
		int readBytes = read(parToCld[READ_END], poruka, MAX_SIZE - 1);
		check_error(readBytes != -1, "read");
		poruka[readBytes] = 0;

		char buffer[MAX_SIZE];
		sprintf(buffer, "Poruka od parenta: %s\n", poruka);
		check_error(write(cldToPar[WRITE_END], buffer, strlen(buffer)) != -1, "write");

		close(parToCld[READ_END]);
		close(cldToPar[WRITE_END]);

		exit(EXIT_SUCCESS);
	}

	// moramo da sacekao da se child zavrsi da ne bismo sejali zombije
	check_error(wait(NULL) != -1, "wait");

	exit(EXIT_SUCCESS);
}