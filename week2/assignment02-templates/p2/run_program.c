<<<<<<< HEAD
#include "run_program.h"
=======
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
>>>>>>> a7f00060e1ce239a684a91771fd74104e338e8df

#define ERROR_CODE 127

int run_program(char *file_path, char *argv[])
<<<<<<< HEAD
{
    (void) file_path;
    (void) argv;

    // remember to return ERROR_CODE on error.
    return ERROR_CODE;
}
=======
{	
	printf("\n\nStarting program\n");
	if (file_path == NULL) { return ERROR_CODE; }

	int status_ptr;
	pid_t pid = fork();

	switch(pid)
	{
		// Fork error
		case -1: 
			printf("-- Fork Error\n");
			break;

		// We are the child processesssssss
		// Run specified program
		case 0:
			printf("-- Child: Executing script...\n");

			if (argv == NULL) { execlp(file_path, file_path, NULL); }	// argv is NULL
			else { 				execvp( file_path, argv); }				// argv not NULL

			return ERROR_CODE;

		// Wait for child process to complete
		default:
			printf("-- Waiting for child to complete.\n");
			waitpid(pid, &status_ptr, 0);

			// WIFEEXITED
			// This macro evaluates to a nonzero (true)
			// value if the child process ended normally.
			// WEXITSTATUS
			// returns the exit status of the child. 
			if (WIFEXITED(status_ptr)) { return WEXITSTATUS(status_ptr); } 
			else { return ERROR_CODE; }
	}
	return ERROR_CODE;
}
>>>>>>> a7f00060e1ce239a684a91771fd74104e338e8df
