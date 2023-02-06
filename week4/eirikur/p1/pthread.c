#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

void* greet(void *id)
{
    printf("Hello, I am %ld\n", (intptr_t)id);
    pthread_exit((void*)0);
}

int main()
{
#define NUM 5

    int i;
    // For each pthread, we need to have a pthread_t structure that allows us
    // to reference the pthread later.
    pthread_t threads[NUM];

    for(i = 0; i < NUM; ++i)
    {
        // Create new pthread with the greet() function as entry point.
        // We pass i as argument to the greet() function.
        int status = pthread_create(threads + i, NULL,
                                    greet, (void*)((intptr_t)i));
        if(status != 0) {
            printf("Error creating thread");
            exit(1);
        }
    }

    // Wait for each thread to exit
    for(i = 0; i < NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}