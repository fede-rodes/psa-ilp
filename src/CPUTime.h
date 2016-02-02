/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */

#ifndef CPUTIME_H_
#define CPUTIME_H_

#if defined(_WIN32)
	#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
	#include <unistd.h>
	#include <sys/resource.h>
	#include <sys/times.h>
	#include <time.h>

#else
	#error " Unable to define getCPUTime( ) for an unknown OS."
#endif

//FUNCTIONS:
/**
 * Returns the amount of CPU time used by the current process, in seconds, or -1.0 if an error occurred.
 */
double get_CPU_time();

#endif /* CPUTIME_H_ */
