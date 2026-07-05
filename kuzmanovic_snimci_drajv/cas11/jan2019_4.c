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

#define MAX_SIZE (255)
#define READ_END (0)
#define WRITE_END (1)

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	// otvaramo fajl
	FILE *f = fopen(argv[1], "r");
	check_error(f != NULL, "fopen");

	// citamo dva argumenta
	char komanda[MAX_SIZE];
	char argument[MAX_SIZE];

	int maxBytes = -1;
	char maxCommand[2 * MAX_SIZE];

	while (fscanf(f, "%s%s", komanda, argument) == 2) {

		int pipes[2];
		check_error(pipe(pipes) != -1, "pipe");

		pid_t childPid = fork();
		check_error(childPid != -1, "fork");

		if (childPid > 0) {

			// parent grana

			// zatvaramo granu za pisanje koja nam nije potrebna
			close(pipes[WRITE_END]);

			int localBytes = 0;
			int readBytes = 0;
			char buffer[MAX_SIZE];

			while ((readBytes = read(pipes[READ_END], buffer, MAX_SIZE)) > 0) {
				localBytes += readBytes;
			}

			check_error(readBytes != -1, "read");

			if (localBytes > maxBytes) {
				maxBytes = localBytes;
				sprintf(maxCommand, "%s %s", komanda, argument);
			}

			close(pipes[READ_END]);

		} else {

			// child grana

			// zatvaramo granu za citanje koja nam nije potrebna
			close(pipes[READ_END]);

			// preusmeravamo sadrzaj: ono sto je standardni izlaz treba da bude nas pajp
			check_error(dup2(pipes[WRITE_END], STDOUT_FILENO) != -1, "dup2");

			// kada smo preusmerili ostaje nam samo da pokrenemo komandu sto se postize sa execlp
			// koristimo execlp jer nam je u zadatku receno unapred koliko imamo komandi
			check_error(execlp(komanda, komanda, argument, NULL) != -1, "execlp");

		}

		// parent u svakom slucaju treba da pozove wait i da time ceka na dete proces
		check_error(wait(NULL) != -1, "wait");

	}

	check_error(feof(f), "fscanf");

	printf("%s\n", maxCommand);

	fclose(f);

	exit(EXIT_SUCCESS);
}