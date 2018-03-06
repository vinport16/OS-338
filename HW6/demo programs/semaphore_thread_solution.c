// Chris Fietkiewicz. Uses a semaphore to control parent child order.
// Uses POSIX threads with a global variable.

#include <stdio.h> // For printf
#include <stdlib.h> // For exit
#include <pthread.h> // For threads
#include <semaphore.h> // For semaphores

sem_t sem; // Global semaphore

void *child(); /* child thread */

int main(int argc, char *argv[]) {
	// Set up semaphore
	if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

	sem_wait(&sem); // Decrement semaphore and lock/wait till available
	// Parent tries to give a speech
	printf("Parent: I have the floor!\n");
	fflush(stdout);
	printf("Parent: I hope my child doesn't interrupt me.\n");
	fflush(stdout);
	printf("Parent: They never listen.\n");
	fflush(stdout);
	printf("Parent: Now, I am finished.\n");
	fflush(stdout);
	sem_post(&sem); // Done, so increment semaphore
	pthread_join(tid, NULL); // Need to wait because someone needs to clean up
	sem_destroy(&sem); // Clean up semaphore
}

void *child() {
	sem_wait(&sem); // Decrement semaphore and lock/wait till available
	// Child tries to give a speech
	printf("Child: I have the floor!\n");
	fflush(stdout);
	printf("Child: I hope my parent doesn't interrupt me.\n");
	fflush(stdout);
	printf("Child: I have a right to be heard.\n");
	fflush(stdout);
	printf("Child: OK, I'm done.\n");
	fflush(stdout);
	sem_post(&sem); // Done, so increment semaphore
	pthread_exit(0);
}
