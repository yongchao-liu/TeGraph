#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

template <class ET>
inline bool cas(ET * ptr, ET oldv, ET newv) {
	if (std::is_same<int, ET>::value) {
		return __sync_bool_compare_and_swap((int *)ptr, *((int *)&oldv), *((int *)&newv));
	} else if(std::is_same<long, ET>::value) {
		return __sync_bool_compare_and_swap((long *)ptr, *((long *)&oldv), *((long *)&newv));
	} else if(std::is_same<double, ET>::value) {
		return __sync_bool_compare_and_swap((long *)ptr, *((long *)&oldv), *((long *)&newv));
	} 
	else if(std::is_same<float, ET>::value) {
		return __sync_bool_compare_and_swap((int *)ptr, *((int *)&oldv), *((int *)&newv));
	} else {
		assert(false);
	}
}

template <class ET>
inline bool write_min(ET * a, ET b) {
	ET c; bool r=0;
	do c = *a;
	while (c > b && !(r=cas(a, c, b)));
	return r;
}

template <class ET>
inline void write_add(ET * a, ET b) {
	volatile ET newV, oldV;
	do { oldV = *a; newV = oldV + b;}
	while (!cas(a, oldV, newV));
}

template <class ET>
inline bool write_atomic(ET * a, ET b){
	ET c; bool r = false;
	do c = *a;
	while (!(r = cas(a, c, b)));
	return r;
}
#endif
