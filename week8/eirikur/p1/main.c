#include "testlib.h"
#include "pipe.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
    test_start("pipe.c");

    printf("Running ls\n");
    errno = 0;
    char *argv1[] = {
        "/bin/ls",
        "/home",
        NULL
    };
    int result1 = run_program(argv1);
    test_equals_int(result1, 0);
    test_equals_int(errno, 0);

    printf("\n-------\nRunning unknown-program-name\n");
    errno = 0;
    char * argv2[] = {
	"unknown-program-name",
        NULL
    };

    int result2 = run_program(argv2);
    // printf("result2=%d\n", result2);
    // printf("test_equals_int(result2, -1)\n");
    test_equals_int(result2, -1);
    // printf("---\n");
    test_equals_int(errno, ENOENT);
    // printf("Error: %s (errno=%d)\n", strerror(errno), errno);

    return test_end();
}
