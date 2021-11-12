#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

inline double get_time() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (t.tv_usec / 1e6);
}

#endif
