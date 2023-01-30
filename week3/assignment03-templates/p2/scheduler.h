#ifndef SCHEDULER_H
#define SCHEDULER_H

/*
 * Maximum number of threads
 */
#define MAX_THREADS 20

void initScheduler();

void onThreadReady(int threadId);

void onThreadPreempted(int threadId);

void onThreadWaiting(int threadId);

int scheduleNextThread();

int startThread(int threadId);
#endif
