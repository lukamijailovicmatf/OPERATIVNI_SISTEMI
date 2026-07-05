#define _XOPEN_SOURCE (700)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define check_error(cond, msg)\
	do {\
		if (!(cond)) {\
			perror(msg);\
			fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define MAX_SIZE (1024)

char *getUserName(uid_t u) {

	struct passwd *userInfo = getpwuid(u);
	check_error(userInfo != NULL, "getpwuid");

	char *userName = malloc(strlen(userInfo->pw_name) + 1);
	check_error(userName != NULL, "malloc");

	strcpy(userName, userInfo->pw_name);

	return userName;
}

char *getGroupName(gid_t g) {

	struct group *groupInfo = getgrgid(g);
	check_error(groupInfo != NULL, "getgrgid");

	char *groupName = malloc(strlen(groupInfo->gr_name) + 1);
	check_error(groupName != NULL, "malloc");

	strcpy(groupName, groupInfo->gr_name);

	return groupName;
} 

void fileInfo(char *path, char **s) {

	check_error(*s == NULL, "prazan string");

	char *buffer = malloc(MAX_SIZE*sizeof(char));
	check_error(buffer != NULL, "malloc");
	*s = buffer;

	struct stat fInfo;
	check_error(stat(path, &fInfo) != -1, "stat");

	char pravaPristupa[11];
	strcpy(pravaPristupa, "-rwxrwxrwx");

	switch (fInfo.st_mode & S_IFMT) {
		case S_IFREG:
			pravaPristupa[0] = '-';
			break;
		case S_IFDIR:
			pravaPristupa[0] = 'd';
			break;
		case S_IFCHR:
			pravaPristupa[0] = 'c';
			break;
		case S_IFBLK:
			pravaPristupa[0] = 'l';
			break;
		case S_IFIFO:
			pravaPristupa[0] = 'p';
			break;
		case S_IFSOCK:
			pravaPristupa[0] = 's';
			break;
	}

	if (!(fInfo.st_mode & S_IRUSR))
		pravaPristupa[1] = '-';

	if (!(fInfo.st_mode & S_IWUSR))
		pravaPristupa[2] = '-';

	if (!(fInfo.st_mode & S_IXUSR))
		pravaPristupa[3] = '-';

	if (!(fInfo.st_mode & S_IRGRP))
		pravaPristupa[4] = '-';

	if (!(fInfo.st_mode & S_IWGRP))
		pravaPristupa[5] = '-';

	if (!(fInfo.st_mode & S_IXGRP))
		pravaPristupa[6] = '-';

	if (!(fInfo.st_mode & S_IROTH))
		pravaPristupa[7] = '-';

	if (!(fInfo.st_mode & S_IWOTH))
		pravaPristupa[8] = '-';

	if (!(fInfo.st_mode & S_IXOTH))
		pravaPristupa[9] = '-';

	int bytes = sprintf(buffer, "%s ", pravaPristupa);

	char *string = getUserName(fInfo.st_uid);
	bytes += sprintf(buffer + bytes, "%s ", string);
	free(string);

	string = getGroupName(fInfo.st_gid);
	bytes += sprintf(buffer + bytes, "%s ", string);
	free(string);

	bytes += sprintf(buffer + bytes, "%d ", (int)fInfo.st_size);

	char *vreme = ctime(&(fInfo.st_mtime));
	bytes += sprintf(buffer + bytes, "%s", vreme);
	buffer[--bytes] = 0;

	bytes += sprintf(buffer + bytes, " %s", path);
}

int main(int argc, char **argv) {

	check_error(argc == 2, "argc");

	char *s = NULL;
	fileInfo(argv[1], &s);

	printf("%s\n", s);

	free(s);

	exit(EXIT_SUCCESS);
}