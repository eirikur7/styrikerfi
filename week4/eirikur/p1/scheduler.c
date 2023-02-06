// Authors: eirikurb19  marteinns19
// Group: 69
#include "scheduler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// Single queue item
typedef struct _QueueItem {
    int data;
    struct _QueueItem *next;
} QueueItem;

// The entire queue
typedef struct _Queue {
    QueueItem *head;
    QueueItem *tail;
} Queue;

// Define allowed thread states
typedef enum _ThreadState {
    STATE_UNUSED = 0, // This entry in the _threads array is unused.

    STATE_READY,      // The thread is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The thread is running and should not be on a ready queue
    STATE_WAITING     // The thread is blocked and should not be on a ready queue
} ThreadState;


// Thread array
typedef struct _Thread {
    int threadId;
    ThreadState state;
    /*
     * Range: 0 ... HIGHEST_PRIORITY (including)
     * HIGHEST_PRIORITY is highest priority
     */
    int priority;
} Thread;

Thread _threads[MAX_THREADS] = {{0}};

/* TODO: Add global variables if needed. */
struct _Queue main_queue = {NULL, NULL};
struct _Queue ready_queue[HIGHEST_PRIORITY + 1] = {{NULL, NULL}};



/*
 * Adds a new, waiting thread.
 */
int startThread(int threadId, int priority)
{
    printf("-- startThread: starting thread with id %d\n", threadId);
    if (
        ((threadId < 0) || 
        (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_UNUSED))
        ||
        (priority < 0) || (priority > HIGHEST_PRIORITY)
        ) 
        {
        printf("-- StartThread: Error starting thread\n");
        return -1;
        }

    _threads[threadId].threadId = threadId;
    _threads[threadId].state    = STATE_WAITING;
    _threads[threadId].priority = priority;
    printf("-- startThread: thread %d in waiting state\n", threadId);
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, int data)
{
    // printf("-- enqueue: inserting thread id %d\n", data);
    (void)queue;
    (void)data;

    // TODO: Implement
    struct _QueueItem *item = (struct _QueueItem *)malloc(sizeof(struct _QueueItem));

    item->data = data;
    item->next = NULL;

    if (queue->head == NULL) {
        queue->head = item;
        queue->tail = item;
        return;
    } else {
        queue->tail->next = item;
        queue->tail = item;
    }
    // printf("-- enqueue: successfull insertion\n");
}

/*
 * Remove and get the head of the queue.
 * Return -1 if the queue is empty.
 */
int _dequeue(Queue *queue)
{
    (void)queue;

    if (queue->head == NULL) {
        return -1;
    }
    // printf("-- dequeue: removing thread id %d\n", queue->head->data);
    // TODO: Implement
    QueueItem *headtbr = queue->head;
    queue->head = queue->head->next;

    // We get the value from the list head first, as after the free, the value is no longer available
    int val = headtbr->data;
    free(headtbr);
    // printf("-- dequeue: successfull removal\n");
    return val;
}

void initScheduler()
{
    // TODO: Implement if you need to initialize any global variables you added
    printf("-- initScheduler: initializing scheduler\n");

    for (int i = 0; i < HIGHEST_PRIORITY; i++) {
        ready_queue[i].head = NULL;
        ready_queue[i].tail = NULL;
    }
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(int threadId)
{
    (void)threadId;

    // TODO: Implement
    printf("-- onThreadReady: thread %d is ready\n", threadId);
    _threads[threadId].state = STATE_READY; // Change the state to ready
    _enqueue(&ready_queue[_threads[threadId].priority], threadId);        // push to the queue
}

/*
 * Called whenever a running thread is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onThreadPreempted(int threadId)
{
    (void)threadId;

    // TODO: Implement
    printf("-- onThreadPreempted: thread %d is preempted\n", threadId);
    _threads[threadId].state = STATE_READY;
    _enqueue(&ready_queue[_threads[threadId].priority], threadId);
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(int threadId)
{
    (void)threadId;

    // TODO: Implement
    printf("-- onThreadWaiting: thread %d is waiting\n", threadId);
    _threads[threadId].state = STATE_WAITING;
}

/*
 * Gets the id of the next thread to run and sets its state to running.
 */
int scheduleNextThread()
{
    // TODO: Implement
    int priority = HIGHEST_PRIORITY;
    for (int i = HIGHEST_PRIORITY; i >= 0; i--) {
        printf("-- scheduleNextThread: priority is %d\n", i);
        if (ready_queue[i].head != NULL) {
            priority = i;
            break;
        }
    }
    printf("-- scheduleNextThread: priority is %d\n", priority);
    

    int nextThread = _dequeue(&ready_queue[priority]);
    _threads[nextThread].state = STATE_RUNNING;
    printf("-- scheduleNextThread: thread %d is scheduled\n", nextThread);
    return nextThread;
}


#if 0
int main() {
	// Initially empty queue (head and tail is NULL)
	Queue q = {NULL,NULL};

	_enqueue( &q, 42 );
	_enqueue( &q, 99 );
	int x = _dequeue( &q );
	printf("Expect: 42, and I got: %d\n", x);

	x = _dequeue( &q );
	printf("Expect: 99, and I got: %d\n", x);
}
#endif
