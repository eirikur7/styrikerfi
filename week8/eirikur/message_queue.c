#include "message_queue.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <stdio.h>

/*
 * The commands supported by the server
 */
typedef enum _Command {
    CmdAdd = 0x00,     // Adds the two message parameters
    CmdSubtract,       // Subtracts the two message parameters
    CmdExit            // Stops the server
} Command;

/*
 * The message format to be sent to the server.
 */
typedef struct _Message {
    /*
     * One of the command constants.
     */
    Command command;
    /*
     * Used as operand 1 (if required)
     */
    int parameter1;
    /*
     * Used as operand 2 (if required)
     */
    int parameter2;
} Message;

#define QUEUE_NAME "/simple_calculator"

#define FORMAT_STRING_ADD      "Calc: %d + %d = %d\n"
#define FORMAT_STRING_SUBTRACT "Calc: %d - %d = %d\n"
mqd_t startClient(void)
{
    mqd_t client = mq_open(QUEUE_NAME, O_WRONLY);
    if (client == (mqd_t)-1) {
        return -1;
    }
    return client;
}

int sendExitTask(mqd_t client)
{
    Message msg = { .command = CmdExit };
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        return -1;
    }
    return 0;
}

int sendAddTask(mqd_t client, int operand1, int operand2)
{
    Message msg = {
        .command = CmdAdd,
        .parameter1 = operand1,
        .parameter2 = operand2
    };
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        return -1;
    }
    return 0;
}

int sendSubtractTask(mqd_t client, int operand1, int operand2)
{
    Message msg = {
        .command = CmdSubtract,
        .parameter1 = operand1,
        .parameter2 = operand2
    };
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        return -1;
    }
    return 0;
}

int stopClient(mqd_t client)
{
    if (mq_close(client) == -1) {
        return -1;
    }
    return 0;
}

int runServer(void)
{
    int didExit = 0, hadError = 0;
    Message msg;

    struct mq_attr attr;
    attr.mq_flags   = 0;
    attr.mq_maxmsg  = 10;           // Maximum number of messages in the queue
    attr.mq_msgsize = sizeof(msg);  // Maximum message size
    attr.mq_curmsgs = 0;

    // Create and open the message queue. Server only needs to read from it.
    mqd_t server = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
    if(server == -1) {
        perror("mq_open");
        return -1;
    }

    // This is the implementation of the server part, already completed:
    // TODO: You may have to make minor extensions in order to satisfy all requirements
    do {
        // Attempt to receive a message from the queue.
        ssize_t received = mq_receive(server, (char*)&msg, sizeof(msg), NULL);
        if (received != sizeof(msg)) {
            // This implicitly also checks for error (i.e., -1)
            hadError = 1;
            continue;
        }

        switch (msg.command)
        {
            case CmdExit:
                // End this loop.
                didExit = 1;
                break;

            case CmdAdd:
                // Print the required output.
                printf(FORMAT_STRING_ADD,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 + msg.parameter2);
                break;

            case CmdSubtract:
                // Print the required output.
                printf(FORMAT_STRING_SUBTRACT,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 - msg.parameter2);
                break;

            default:
                // Exit on unknown command
                hadError = 1;
                break;
        }
    } while (!didExit);

    // Close the message queue on exit and unlink it
    if (mq_close(server) == -1) {
        perror("mq_close");
        hadError = 1;
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        hadError = 1;
    }

    return hadError ? -1 : 0;
}
