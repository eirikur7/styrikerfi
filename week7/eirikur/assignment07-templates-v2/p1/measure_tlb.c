// Name: xxx, yyy (please add your name here)
// Group:  0

#include "measure_tlb.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 4096

// A: Write a function that uses the CPU time stamp register gettimeofday() (see manpage)
// to create a timestamp in microseconds
uint64_t getTimeStamp() {
	struct timeval current_time;
	gettimeofday(&current_time, NULL); 	
	return current_time.tv_sec * 1000000 + current_time.tv_usec;
}

// B: Write a function measureFunction that takes a function and an argument as input
// parameters. Your function shall invoke (*function)(arg) and measure and return the
// exeuction time of that function (in microseconds).
uint64_t measureFunction( void(*function)(void *), void *arg ) {
	// Add code around the invocation of function to measure the execution time

	uint64_t start = getTimeStamp();

	(*function)(arg);
	
	uint64_t end = getTimeStamp();
	return end - start;
}

// C: Measurements of timing depend on many nondeterminstic factors, such as other users
// or processes on the system. We thus want to repeat a measurement multiple times and
// generate statistics (calculate the minimum, average, and maximum value). Implement a
// function that invokes measureFunction statcount times and returns (call-by-reference!)
// the statistics it collected.
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

// D: Implement a function that takes two parameters n and pages. The function shall make n
// memory accesses on pages different memory pages (each memory access shall access a
// different memory page, until pages different pages have been accessed, and then start
// again at the first page.)
// Your function first has to use malloc() to allocate space on the heap that is large
// enough to contain the required number of pages. In case of insufficient available memory, 
// your application shall print an error message and exit with status code 1.
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

// E: The accessMemory function takes two paramters, while the generic measuremenmt
// function passes a single void * to the function it invokes. Implement a simple wrapper
// function that receives a pointer to a MeasureParameters struct (as the generic void *)
// and then invokes your accessMemory function 
void accessMemoryWrapper(void *p) {
	(void)p;
	// Implement this function
	MeasurementParameters* parameters = (MeasurementParameters*)p;
	accessMemory(parameters->iterations, parameters->pages);
}

