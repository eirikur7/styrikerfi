#include "parseint.h"

/*
 * Returns the value of c or -1 on error
 */
int parseDecimalChar(char c)
{
    (void)c;
    int c_int = (int)c;
    if (c_int >= 48 && c_int <= 57)
    {
        return c_int - 48;
    }

    return -1;
}

/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string)
{
    (void)string;

    int ret_val = 0;    // return value
    int num_size = 1;    // the size of each char, depending on bit's size
    int curr_char = parseDecimalChar(string[0]);    // Current char in loop
    int num_sys = 10;    // Numbering system used, octal or deciamal.

    // if octal
    if (curr_char == 0)
    {
        num_sys = 8;
    }

    // if error on first char
    else if (curr_char == -1)
    {
        return -1;
    }
    
    // Iterate to the sentinel value of the string array to find the 
    // size of the array
    int i = 0;
    while (string[i] != '\0')
    {
        i++;
    }

    // Iterate through each char from back to front
    // and adding the bit's size multiplied with it's value.
    for (i=i-1; i >= 0; --i)
    {
        curr_char = parseDecimalChar(string[i]);
        // if error on char
        if (curr_char == -1)
        {
            return -1;
        }

        // Add the values together
        else 
        {
            // If the number is higher than allowed, f.x. 9 can't be in octal system.
            if (curr_char > num_sys) 
            {
                return -1;
            }
            ret_val += num_size * curr_char;
        }
        
        num_size *= num_sys; // Multiply by eight, to get the next bit's size
    }

    return ret_val;
}
