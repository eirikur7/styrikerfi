#include <inttypes.h>

typedef struct _statistics {
	uint64_t min;
	uint64_t avg;
	uint64_t max;
} Statistics;

typedef struct _MeasureParameters {
	int iterations;
	int pages;
} MeasurementParameters;


uint64_t getTimeStamp();
uint64_t measureFunction( void(*function)(void *), void *arg );
void measureStatistics( int statcount, Statistics *stat, void(*function)(void *), void *arg);
void accessMemory(int n, int pages);
void accessMemoryWrapper(void *p);


