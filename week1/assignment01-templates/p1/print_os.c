#include "print.h"
#include <inttypes.h>
#include <stdio.h>

void print_line(int64_t number, char *string)
{
    (void) number;
    (void) string;
    // Add code here.
    printf("%" PRIi64 " %s\n", number, string);
}