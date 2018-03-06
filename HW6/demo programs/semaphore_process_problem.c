// Chris Fietkiewicz. Parent and child try to give speeches, 
// but the problem is that they conflict with each other.
// Uses fork.
#include <unistd.h> // For fork
#include <stdio.h> // For printf

int main() {
	pid_t pid;
	pid = fork();
	if (pid == 0) { // Child tries to give a speech
		printf("Child: I have the floor!\n");
		fflush(stdout);
		printf("Child: I hope my parent doesn't interrupt me.\n");
		fflush(stdout);
		printf("Child: I have a right to be heard.\n");
		fflush(stdout);
		printf("Child: OK, I'm done.\n");
		fflush(stdout);
	}
	else { // Parent tries to give a speech
		printf("Parent: I have the floor!\n");
		fflush(stdout);
		printf("Parent: I hope my child doesn't interrupt me.\n");
		fflush(stdout);
		printf("Parent: They never listen.\n");
		fflush(stdout);
		printf("Parent: Now, I am finished.\n");
		fflush(stdout);
	}
	return 0;
}
