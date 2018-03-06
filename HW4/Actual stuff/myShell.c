/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */
#include <string.h> // Required for string operations
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE 80



#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{   
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
	
	int i, upper;
		
    while (should_run){   
        printf("vincent👀>");
        fflush(stdout);
        

        char line[50];
        char *args[MAX_LINE/2 + 1]; /* command line (of 80) has max of 40 arguments */
        gets(line);
        args[0] = strtok(line, " ");
        int i = 0;
        while (args[i] != NULL)
            args[++i] = strtok(NULL, " ");
        if( !strcmp(args[0], "exit") ){
            should_run = 0;
            break;
        }else{

            /**
            * After reading user input, the steps are:
            * (1) fork a child process
            * (2) the child process will invoke execvp()
            * (3) if command included &, parent will invoke wait()
            */

            pid_t pid = fork();
            if(pid == 0){
                execvp(args[0], args); // Execute the command
                fflush(stdout);
                kill(getpid(), SIGKILL);
                printf("this should not print\t" );
            }else{
                wait(NULL);
            }

        }

        
        

        
    }
    
	return 0;
}
