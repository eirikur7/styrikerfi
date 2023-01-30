#include "testlib.h"
#include "exploit_program.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    test_start("run_program.c");

    // create test pipe, open it for reading
    unlink("success.txt");
    int res = mkfifo("success.txt", 0644);
    if(res) { perror("Cannot create success.txt pipe"); _test_failed(); goto theend; }

    int fd = open("success.txt", O_RDONLY|O_NONBLOCK);
    if(fd<0) { perror("Cannot open success.txt pipe for reading"); _test_failed(); goto theend; }

    // Call the function to be implemented
    res = exploit_program("./vulnerable", "hello");
    test_equals_int(res, 0);

    // Check if something is available for reading in the test pipe
    char buf[1024];
    int cnt = read(fd, buf, 1024);
    if(cnt<=0) { test_failed_message("Expected data on pipe success.txt, got nothing"); }
    else { 
	buf[cnt]=0;
	printf("Debug: Got message from vulnerable target: %s", buf);
    }
    close(fd);
    unlink("success.txt");

theend:
    return test_end();
}

