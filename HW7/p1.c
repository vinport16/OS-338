#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 10
#define BUFFER_SIZE 5

/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t mutex, full, empty;

void *child(); /* child thread */

int main(int argc, char *argv[]) {

//initializng the semaphore
if(sem_init(&mutex, 0, 1) < 0 || sem_init(&full, 0, 0) < 0 || sem_init(&empty, 0, BUFFER_SIZE) < 0) { // 0 = multithreaded
fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
exit(0);
}

pthread_t tid; /* the thread identifiers */
pthread_attr_t attr; /* set of attributes for the thread */
pthread_attr_init(&attr);
pthread_create(&tid, &attr, child, NULL);

/*
 Parent information (writes to buffer)
 */
int data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Must have N values
int i, input = 0;

for (i = 0; i < N; i++){
	sem_wait(&empty);
	sem_wait(&mutex);

	buffer[input] = data[i];

	sem_post(&mutex);
	sem_post(&full);

	input = (input + 1) % BUFFER_SIZE;
}

// Join and print result
pthread_join(tid, NULL);
sem_destroy(&mutex);
sem_destroy(&full);
sem_destroy(&empty);
}

void *child() {

	/*
	 Child information (retrieves from buffer)
	 */

	int i, output = 0;

	for (i = 0; i < N; i++){
	sem_wait(&full);
	sem_wait(&mutex);

	int data = buffer[output];
	printf("%d\n", data);
	
	sem_post(&mutex);
	sem_post(&empty);

	//increment buffer pointer
	output = (output + 1) % BUFFER_SIZE;
	}
	pthread_exit(0);
}