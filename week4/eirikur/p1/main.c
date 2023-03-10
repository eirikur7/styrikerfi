#include "testlib.h"
#include "scheduler.h"
#include <stdio.h>

#define test_scheduleNextThread(should) ({ \
    int scheduled = scheduleNextThread(); \
    printf("%s line %d: Next Thread: %d\n", __FILE__, __LINE__, scheduled); \
    test_equals_int(scheduled, should); \
})

int main()
{
    test_start("scheduler.c");
    printf("Initializing scheduler...\n");
    initScheduler();

    printf("\nStarting threads...\n");
    startThread(0, 5);
    startThread(1, 4);
    startThread(2, 2);
    startThread(3, 4);
    startThread(4, 3);
    startThread(5, 4);
    startThread(6, 2);
    startThread(7, 0);

    printf("\nTest: scheduler...\n");
    test_scheduleNextThread(-1);
    printf("\nTest: nothing should be running\n");
    // Let's assume we idle some time.

    onThreadReady(1);

    test_scheduleNextThread(1);
    // Let's assume Thread 1 is running here.
    printf("\nTest: thread 1 should be running\n");

    onThreadPreempted(1);
    test_scheduleNextThread(1);
    printf("\nTest: thread 1 should be running\n");
    // Let's assume Thread 1 is still running here.
    // Thread 1 decides to do some Disk IO.
    onThreadWaiting(1);
    test_scheduleNextThread(-1);
    printf("\nTest: nothing should be running\n");
    // Assume we idle some time.
    // Suddenly, thread 1 gets the data from the disk.
    onThreadReady(1);
    test_scheduleNextThread(1);
    printf("\nTest: thread 1 should be running\n");
    // Now thread 1 is running.
    // Thread 0 and 7 get ready.
    onThreadReady(0);
    onThreadReady(7);
    // Thread 1 is still running and gets preempted.
    onThreadPreempted(1);
    test_scheduleNextThread(0);
    printf("\nTest: thread 0 should be running\n");

    // Now thread 0 is running.\n
    for (int i = 0; i < 4; i++) {
        onThreadPreempted(0);
        test_scheduleNextThread(0);
        printf("\nTest: thread 0 should be running\n");
    }
    onThreadPreempted(0);
    // Now we have scheduled thread 0 five times.
    // This means that a lower priority thread should be scheduled next.
    test_scheduleNextThread(1);
    printf("\nTest: thread 1 should be running\n");

    // The whole thing should repeat until thread 1 was scheduled 5 times.
    // Then thread 7 should be scheduled.
    // You can write the test for this yourself.

    return test_end();
}
