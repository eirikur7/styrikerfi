#include "scheduler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct _QueueItem {
    /*
     * The data of this item.
     */
    int data;
    /*
     * The next item in the queue.
     * NULL if there is no next item.
     */
    struct _QueueItem *next;
} QueueItem;

typedef struct _Queue {
    /*
     * The first item of the queue.
     * NULL if the queue is empty.
     */
    QueueItem *head;
    /*
     * The last item of the queue.
     * undefined if the queue is empty.
     */
    QueueItem *tail;
} Queue;

typedef enum _ThreadState {
    STATE_UNUSED = 0, // This entry in the _threads array is unused.

    STATE_READY,      // The thread is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The thread is running and should not be on a ready queue
    STATE_WAITING     // The thread is blocked and should not be on a ready queue
} ThreadState;

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
struct _Queue main_queue[HIGHEST_PRIORITY+1] = {{NULL,NULL}};
int num_run_list[HIGHEST_PRIORITY+1] = {0};

/*
 * Adds a new, waiting thread.
 */
int startThread(int threadId, int priority)
{
    if (((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_UNUSED)) ||
        (priority < 0) || (priority > HIGHEST_PRIORITY)) {

        return -1;
    }

    _threads[threadId].threadId = threadId;
    _threads[threadId].state    = STATE_WAITING;
    _threads[threadId].priority = priority;
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, int data)
{

    assert(queue != NULL);

    // TODO: Implement
    struct _QueueItem *item = (struct _QueueItem *)malloc(sizeof(struct _QueueItem));
    if (item == NULL) {
        return;
    }

    item->data = data;
    item->next = NULL;

    if (queue->head == NULL) {
        queue->head = item;
        queue->tail = item;
        return;
    } else {
        assert(queue->tail != NULL);
        queue->tail->next = item;
        queue->tail = item;
    }
}

/*
 * Remove and get the head of the queue.
 * Return -1 if the queue is empty.
 */
int _dequeue(Queue *queue)
{
    assert(queue != NULL);

        if (queue->head == NULL) {
        return -1;
    }

    // TODO: Implement
    QueueItem *headtbr = queue->head;
    queue->head = queue->head->next;

    // We get the value from the list head first, as after the free, the value is no longer available
    int val = headtbr->data;
    free(headtbr);
    return val;
}

void initScheduler()
{
    // TODO: Implement if you need to initialize any global variables you added
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(int threadId)
{
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    assert(_threads[threadId].state == STATE_WAITING);

    _threads[threadId].state = STATE_READY;
    _enqueue(&main_queue[_threads[threadId].priority], threadId);
}

/*
 * Called whenever a running thread is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onThreadPreempted(int threadId)
{
    assert((threadId >= 0) && (threadId < MAX_THREADS));

    _threads[threadId].state = STATE_READY;
    _enqueue(&main_queue[_threads[threadId].priority], threadId);
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(int threadId)
{
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    assert(_threads[threadId].state == STATE_RUNNING);

    _threads[threadId].state = STATE_WAITING;
}

/*
 * Gets the id of the next thread to run and sets its state to running.
 */
int scheduleNextThread()
{
    // TODO: Implement
    int no_low_pri = 0; 
    int queue_num = -1;

    for (int i = HIGHEST_PRIORITY; i >= 0; i--) {
        if (main_queue[i].head != NULL) {
            if (num_run_list[i] < 4) {
                num_run_list[i]++;
                int nextThread = _dequeue(&main_queue[i]);
                _threads[nextThread].state = STATE_RUNNING;
                return nextThread; 
            }
            else {
                num_run_list[i] = 0;
                if (no_low_pri == 0) {
                    no_low_pri = 1;
                    queue_num = i;
                }

            }
            
        }

    }

    if (no_low_pri == 1) {
        num_run_list[queue_num] = 4;
        int nextThread = _dequeue(&main_queue[queue_num]);
         _threads[nextThread].state = STATE_RUNNING;
        return nextThread; 
    }

    for (int i = 0; i < HIGHEST_PRIORITY; i++) {
        num_run_list[i] = 0;
    }


    

    return -1;


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
