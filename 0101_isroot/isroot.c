#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isroot(char *);

int
main(void) {
    int i;
    int isRoot;
    char *dirs[] = { "/home", "/", "/asdf", "/etc/passwd" };

    for (i = 0; i < sizeof(dirs)/sizeof(dirs[0]); i++) {
	isRoot = isroot(dirs[i]);

	if (isRoot == -1)
	    printf("error while testing: %s\n", dirs[i]);
	else if (isRoot)
	    printf("%s is root\n", dirs[i]);
	else
	    printf("%s is not root\n", dirs[i]);
    }
    exit(0);
}

/**
 * returns:
 * -1 = error while execution
 * 0 = is not root
 * 1 = is root
 */
int
isroot(char *path) {
    DIR *dp;
    struct dirent *entry;
    struct dirent *dot = NULL;
    struct dirent *dotdot = NULL;

    if ((dp = opendir(path)) == NULL) {
	perror(path);
	return -1;
    }
    while ((entry = readdir(dp)) != NULL && (dot == NULL || dotdot == NULL))
	if (strcmp(entry->d_name, ".") == 0)
	    dot = entry;
	else if (strcmp(entry->d_name, "..") == 0)
	    dotdot = entry;
    if (closedir(dp) != 0)
	perror(path);

    return (dotdot->d_ino == dot->d_ino) ? 1 : 0;
}

