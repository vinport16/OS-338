// Chris Fietkiewicz. Parent and child try to give speeches, 
// but the problem is that they conflict with each other.
// Uses POSIX threads.

#include <stdio.h> // For printf
#include <pthread.h> // For threads

void *child(); /* child thread */

int main(int argc, char *argv[]) {
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

	// Parent tries to give a speech
	printf("Parent: I have the floor!\n");
	fflush(stdout);
	printf("Parent: I hope my child doesn't interrupt me.\n");
	fflush(stdout);
	printf("Parent: They never listen.\n");
	fflush(stdout);
	printf("Parent: Now, I am finished.\n");
	fflush(stdout);
}

void *child() {
	// Child tries to give a speech
	printf("Child: I have the floor!\n");
	fflush(stdout);
	printf("Child: I hope my parent doesn't interrupt me.\n");
	fflush(stdout);
	printf("Child: I have a right to be heard.\n");
	fflush(stdout);
	printf("Child: OK, I'm done.\n");
	fflush(stdout);
}
