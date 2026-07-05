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

	check_error(argc > 1, "argc");

	// za svaki argument mi treba da pokrenemo dete proces
	for (int i = 1; i < argc; i++) {

		// za svaki novi fork treba da napravimo novi pajp
		int child2par[2];
		check_error(pipe(child2par) != -1, "pipe");

		pid_t childPid = fork();
		check_error(childPid != -1, "fork");

		// CHILD grana
		if (childPid == 0) {

			// zatvaramo stranu pajpa koju ne koristimo
			close(child2par[READ_END]);

			// vrsimo preusmeravanje: kraj za pisanje deteta postaje stdout
			check_error(dup2(child2par[WRITE_END], STDOUT_FILENO), "dup2");

			// pozivamo konacno komandu, posto je komanda ugradjena koristi execlp
			// na kraju OBAVEZNO stavimo NULL da ne bi dobili segmentation fault
			// execlp radi tako sto adresni prostor child procesa zameni drugim programom i 
			// taj adresni prostor ce da ucita program koji se zove "du"
			check_error(execlp("du", "du", "-sch", argv[i], NULL) != -1, "execlp");

			// samo radi higijene pisemo ovo da bi naznacili da se ova grana nakon
			// ovog poziva zavrsava
			exit(EXIT_FAILURE);
		}

		// PARENT grana

		// sledeca stvar nakon ovoga je da "pokupimo" odgovor komande i da vidimo sta tu pise
		char buffer[MAX_SIZE];

		// zatvaramo granu roditelja koja nam ne treba, a to je grana za pisanje jer roditelj
		// treba da cita sadrzaj od deteta
		close(child2par[WRITE_END]);

		// definisemo promenljivu da vidimo koliko smo bajtova procitali
		// stavljamo MAX_SIZE - 1 da bi mogli da procitani sadrzaj terminisemo 0
		// i posle da ga ispisemo
		int readBytes = read(child2par[READ_END], buffer, MAX_SIZE - 1);
		check_error(readBytes != -1, "read");
		buffer[readBytes] = 0;

		// status zavrsetka nosi informaciju o tome kako se program zavrsio i koji je exit code
		// to su dve stvari koje nisu iste

		// proveravamo da li se komanda zavrsila uspesno
		int status = 0;
		check_error(wait(&status) != -1, "wait");

		// proveravamo prvo da li se program uspesno zavrsio pa tek onda proveravamo
		// kakav mu je exit code
		if (WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)) {

			// ako je sve ok samo ispisujemo sta nam je vratila komanda "du"
			// i ispisujemo velicine fajlova koji su prosledjeni u komandnoj liniji
			char rezultat[MAX_SIZE];
			sscanf(buffer, "%s", rezultat);
			printf("%s ", rezultat);

		} else {

			// u suprotnom je doslo do neke greske i ispisujemo odgovarajucu poruku
			printf("neuspeh ");

		}

		// zatvaramo stranu pajpa koja nam vise ne treba da nam ne bi cureli fajl deskriptori
		close(child2par[READ_END]);
	}

	printf("\n");

	exit(EXIT_SUCCESS);
}