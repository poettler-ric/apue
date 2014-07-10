#include <sys/resource.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MOCK_SYSCONF
#define _SC_OPEN_MAX 0
long
sysconf(int name) {
    return(LONG_MAX);
}
#else
#include <unistd.h>
#endif // _MOCK_SYSCONF

long getmaxopen(void);
void printmaxopen(void);

#define _MAX_OPEN_GUESS 256

#ifdef MAX_OPEN
long maxopen = MAX_OPEN;
#else
long maxopen = 0;
#endif // MAX_OPEN

int
main(void) {
    printmaxopen();
    printf("got: %ld\n", getmaxopen());
    exit(0);
}

long
getmaxopen(void) {
    struct rlimit limit;

    if (maxopen == 0) {
	errno = 0;
	if ((maxopen = sysconf(_SC_OPEN_MAX)) < 0) {
	    if (errno != 0)
		perror("_SC_OPEN_MAX");
	    maxopen = LONG_MAX;
	}
	// LONG_MAX is too big -> try getrlimit
	if (maxopen == LONG_MAX) {
	    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
		perror("RLIMIT_NOFILE");
		maxopen = _MAX_OPEN_GUESS;
	    } else
		maxopen = limit.rlim_cur;
	}
    }
	
    return(maxopen);
}

void
printmaxopen(void) {
    struct rlimit limit;
    long sysconf_open_max = 0;

#ifdef MAX_OPEN
    printf("MAX_OPEN: %ld\n", MAX_OPEN);
#else
    printf("MAX_OPEN not defined\n");
#endif

    errno = 0;
    if ((sysconf_open_max = sysconf(_SC_OPEN_MAX)) == -1) {
	if (errno)
	    perror("_SC_OPEN_MAX");
	else
	    printf("_SC_OPEN_MAX: functionality not supported\n");
    } else
	printf("_SC_OPEN_MAX: %ld\n", sysconf_open_max);

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1)
	perror("RLIMIT_NOFILE");
    else
	printf("soft: %lu hard: %lu\n", limit.rlim_cur, limit.rlim_max);
}

