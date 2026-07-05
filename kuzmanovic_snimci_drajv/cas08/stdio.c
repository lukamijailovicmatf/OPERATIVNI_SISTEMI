#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
	int par2cld[2];

	check_error(pipe(cld2par) != -1, "pipe");
	check_error(pipe(par2cld) != -1, "pipe");

	pid_t childPid = fork();
	check_error(childPid != -1, "fork");

	if (childPid > 0) {

		close(par2cld[READ_END]);
		close(cld2par[WRITE_END]);

		// grana za citanje
		FILE *f_in = fdopen(cld2par[READ_END], "r");
		check_error(f_in != NULL, "fdopen");

		// grana za pisanje
		FILE *f_out = fdopen(par2cld[WRITE_END], "w");
		check_error(f_out != NULL, "fdopen");

		fprintf(f_out, "Neka poruka.\n");
		fflush(f_out);

		char buffer[MAX_SIZE];
		check_error(fgets(buffer, MAX_SIZE - 1, f_in) != NULL, "fgets");

		printf("%s", buffer);

		fclose(f_in);
		fclose(f_out);

	} else {

		close(cld2par[READ_END]);
		close(par2cld[WRITE_END]);

		FILE *f_in = fdopen(par2cld[READ_END], "r");
		check_error(f_in != NULL, "fdopen");

		FILE *f_out = fdopen(cld2par[WRITE_END], "w");
		check_error(f_out != NULL, "fdopen");

		char buffer[MAX_SIZE];
		check_error(fgets(buffer, MAX_SIZE - 1, f_in) != NULL, "fgets");

		int n = strlen(buffer);
		buffer[n-2] = 0;
		time_t now = time(NULL);
		strcat(buffer, ctime(&now));

		fprintf(f_out, "%s", buffer);
		fflush(f_out);

		fclose(f_in);
		fclose(f_out);

		exit(EXIT_SUCCESS);
	}

	check_error(wait(NULL) != -1, "wait");

	exit(EXIT_SUCCESS);
}