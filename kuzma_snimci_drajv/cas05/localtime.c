#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

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

	time_t now;
	check_error(time(&now) != -1, "time");

	// stampamo broj sekundi od pocetka epohe
	printf("%d\n", (int)now);

	struct tm *brokenTime = localtime(&now);
	check_error(brokenTime != NULL, "localtime");

	// uvecavamo godinu za jedan
	brokenTime->tm_year++;

	time_t newTime = mktime(brokenTime);
	check_error(newTime != -1, "mktime");

	// ispisujemo novo vreme tj. datum ali formatirano
	printf("%s", ctime(&newTime));

	char timeString[1024];
	strftime(timeString, 1024, "Trenutno vreme: %H:%M:%S", brokenTime);

	// ispisujemo trenutno vreme na racunaru
	printf("%s\n", timeString);

	// racunamo precizno vreme
	struct timeval preciseTime;
	check_error(gettimeofday(&preciseTime, NULL) != -1, "gettimeofday");

	if (now == preciseTime.tv_sec) {
		printf("Jednako vreme\n");
	} else {
		printf("Razlicito vreme\n");
	}

	// stampamo mikrosekunde
	printf("Mikrosekunde: %d\n", (int)preciseTime.tv_usec);

	exit(EXIT_SUCCESS);
}