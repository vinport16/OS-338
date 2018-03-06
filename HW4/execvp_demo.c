// Chris Fietkiewicz (cxf47@case.edu)
// Demonstrates gets(), strtok(), strcmp(), and execvp()
#include <unistd.h> // Required for execvp
#include <stdio.h> // Required for I/O
#include <string.h> // Required for string operations

#define MAX_LINE 80 /* 80 chars per line, per command */

int main() {
	char line[50];
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	printf("Enter command: ");
	gets(line);
	args[0] = strtok(line, " ");
	int i = 0;
	while (args[i] != NULL)
		args[++i] = strtok(NULL, " ");
	printf("The last argument is '%s':\n", args[i-1]);
	if (!strcmp(args[0], "ls")) // Returns difference in lexicographical order (0 means equal)	
		printf("Ah, you wish to see the files. Here they are:\n");
	execvp(args[0], args); // Execute the command
}