#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 10
#define BUFFER_SIZE 5
struct condition {
	sem_t sem;
	int count;
};
/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t mutex, next;
struct condition not_full, not_empty;
int next_count = 0;
int count = 0;

void cwait(struct condition *c);
void cpost(struct condition *c);
void *producer();
void *consumer();

void enter_monitor(){
	sem_wait(&mutex);
}

void exit_monitor(){
	if(next_count > 0){
		sem_post(&next);
	}else{
		sem_post(&mutex);
	}
}

int main(int argc, char *argv[]) {

//initializng the semaphore
	if(sem_init(&mutex, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: 0 could not initialize &semaphore.\n");
		//exit(0);
	}

	if(sem_init(&next, 0, 0) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: 1 could not initialize &semaphore.\n");
		//exit(0);
	}

	if(sem_init(&(not_empty.sem), 0, 0) < 0 || sem_init(&(not_full.sem), 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: 2 could not initialize &semaphore.\n");
		//exit(0);
	}

	exit(0);
	pthread_t tid; /* the thread identifiers */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, producer, NULL);

	pthread_t tid2; /* the thread identifiers */
	pthread_attr_t attr2; /* set of attributes for the thread */
	pthread_attr_init(&attr2);
	pthread_create(&tid2, &attr2, consumer, NULL);

	

	// Join and print result
	pthread_join(tid, NULL);
	sem_destroy(&mutex);
	sem_destroy(&next);
	sem_destroy(&(not_full.sem));
	sem_destroy(&(not_empty.sem));
}

void *producer() {
	
	int data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Must have N values
	int i, input = 0;

	for (i = 0; i < N; i++){
		
		enter_monitor();

		if(count == N){
			cwait(&not_full);
		}
		buffer[input] = data[i];
		count++;
		cpost(&not_empty);

		exit_monitor();

		input = (input + 1) % BUFFER_SIZE;
	}

}

void *consumer(){

	int i, value, input = 0;

	for (i = 0; i < N; i++){
		
		enter_monitor();

		if(count == N){
			cwait(&not_empty);
		}
		value = buffer[input];
		count--;
		cpost(&not_full);

		exit_monitor();

		printf("%d\n", value);

		input = (input + 1) % BUFFER_SIZE;
	}

}

// Semaphore implementation of conditional wait
void cwait(struct condition *c) {
	c->count++;
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	sem_wait(&(c->sem));
	c->count--;
}

// Semaphore implementation of conditional signal
void cpost(struct condition *c) {
	if (c->count > 0) {
		next_count++;
		sem_post(&(c->sem));
		sem_wait(&next);
		next_count--;
	}
}