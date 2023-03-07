// This changes the way some includes behave.
// This should stay before any include.
#define _GNU_SOURCE

#include "pipe.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork, pipe */
#include <stdlib.h> /* For exit */
#include <fcntl.h>
#include <errno.h>

int run_program(char *argv[])
{

    if (argv == NULL) {
        return -1;
    }

    int pipefd[2];
    if (pipe2(pipefd, O_CLOEXEC) == -1) {
        return -1;
    }

    int child_pid = fork();
    if (child_pid == -1) {
        return -1;
    } else if (child_pid == 0) {

        // Replace program
        execvp(argv[0], argv);

        // Write errno to the pipe if execvp failed
        int exec_errno = errno;
        write(pipefd[1], &exec_errno, sizeof(exec_errno));
        close(pipefd[1]);

        exit(0);
    } else {
        int status, hadError = 0;

        close(pipefd[1]); // Close the write end of the pipe in the parent

        int waitError = waitpid(child_pid, &status, 0);
        if (waitError == -1) {
            // Error while waiting for child.
            hadError = 1;
        } else if (!WIFEXITED(status)) {
            // Our child exited with another problem (e.g., a segmentation fault)
            // We use the error code ECANCELED to signal this.
            hadError = 1;
            errno = ECANCELED;
        } else {
            // Read errno from the pipe if execvp failed in the child
            if (read(pipefd[0], &errno, sizeof(errno)) == sizeof(errno)) {
                hadError = 1;
            }
        }

        close(pipefd[0]); // Close the read end of the pipe in the parent
        return hadError ? -1 : WEXITSTATUS(status);
    }
}