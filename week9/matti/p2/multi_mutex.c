#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#include "multi_mutex.h"

int multi_mutex_unlock(pthread_mutex_t **mutexv)
{
    if (mutexv == NULL) {
        return -1;
    }
    int i = 0;
    while (mutexv[i] != NULL) {
        pthread_mutex_unlock(mutexv[i]);
        i++;
    }
    return 0;
}

int multi_mutex_trylock(pthread_mutex_t **mutexv)
{
    if (mutexv == NULL) {
        return -1;
    }
    int i = 0;
    while (mutexv[i] != NULL) {

        int ret = pthread_mutex_trylock(mutexv[i]);
        if (ret != 0) {
            i--;
            while (i >= 0) {
                pthread_mutex_unlock(mutexv[i]);
                i--;
            }
            return -1;
        }
        i++;

    }
    return 0;
}

// Just for optional bonus points
// Blocks until all mutexes are available, making sure that no deadlocks can occur,
// i.e. no mutex may be held while blocking
int multi_mutex_lock(pthread_mutex_t **mutexv)
{
    if (mutexv == NULL) {
        return -1;
    }
    int i = 0;
    while (mutexv[i] != NULL) {
        pthread_mutex_lock(mutexv[i]);
        i++;
    }
    return 0;
}

