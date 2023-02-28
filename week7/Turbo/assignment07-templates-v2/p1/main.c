#include "testlib.h"
#include "measure_tlb.h"
#include <stdio.h>
#include <stdlib.h>

#define REPETITIONS 100
#define ITERATIONS 100000
#define MSTEPS 100

int main()
{
    test_start("measure_tlb.c");

    printf(" # ;  Pages ;      min/ms ;     avg/ms ;     max/ms\n");
    for(int i=0; i<MSTEPS; i++) {
        Statistics stat;
        // approx. logscale in sqrt(2) steps
        uint64_t numpages = (1LL<<(i/2+1)) | ( (i&1)?(1LL<<(i/2)):0 );
        MeasurementParameters params = { ITERATIONS, numpages };
        measureStatistics( REPETITIONS, &stat, accessMemoryWrapper, &params );
        printf("%2d ; %6" PRIu64 " ;  %10.6f ; %10.6f ; %10.6f \n", i, numpages, 0.001*stat.min, 0.001*stat.avg, 0.001*stat.max);
    }        
    return test_end();
}




