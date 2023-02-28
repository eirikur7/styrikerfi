// Name: xxx, yyy (please add your name here)
// Group:  0

#include "measure_tlb.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 4096

uint64_t getTimeStamp() {
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return current_time.tv_sec * 1000000 + current_time.tv_usec;

}

uint64_t measureFunction( void(*function)(void *), void *arg ) {
	// Add code around the invocation of function to measure the execution time

	uint64_t start = getTimeStamp();

	(*function)(arg);
	
	uint64_t end = getTimeStamp();
	return end - start;
}

void measureStatistics( int statcount, Statistics *stat, void(*function)(void *), void*arg) {
	int min = 0;
	int max = 0;
	int avg = 0;

	for (int i = 0; i < statcount; i++) {
		// uint64_t start = getTimeStamp();
		// (*function)(arg);
		// uint64_t end = getTimeStamp();

		// int time = end - start;
		int time = measureFunction(function, arg);
		if (i == 0) {
			min = time;
			max = time;
		}

		if (time < min) {
			min = time;
		}
		if (time > max) {
			max = time;
		}
		avg += time;
	}
	avg = avg / statcount;
	stat->min = min;
	stat->avg = avg;
	stat->max = max;
	return;


	// Implement this function: invoke measureFunction() statcount times and calculate statistics
}

void accessMemory(int n, int pages) {

	// Implement this function


	int *p = malloc(pages * PAGESIZE);

	if (p == NULL) {
		exit(1);
	}

	int page = 0;
	for (int i = 0; i < n; i++) {
			int *address = p + page * PAGESIZE / sizeof(int);
			*address = i;
			page = (page + 1) % pages;
		}
	
	free(p);


}

void accessMemoryWrapper(void *p) {
	(void)p;
	// Implement this function
	MeasurementParameters* parameters = (MeasurementParameters*)p;
	accessMemory(parameters->iterations, parameters->pages);

	


}

