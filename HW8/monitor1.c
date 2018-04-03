/* Chris Fietkiewicz. Demonstrates monitor solution using semaphores.
Based on algorithm from Operating System Concepts by Silberschatz et al.
Does the following steps:
1. Creates 2 threads for "add" function.
   Each thread waits on cond. var. and computes a sum (1-50 and 51-100).
2. Creates 3rd thread for "total" function which signals "add" threads.
3. 3rd thread sleeps for 1 sec to give threads some time to finish and prints sum.
*/
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> // For sleep

// Struct for condition variable
struct condition {
	sem_t sem; // Semaphore to be initialized to 0
	int count; // Count of threads waiting
};

/* Global variables are shared by the thread(s) */
sem_t mutex, next;
struct condition add_condition;
int next_count = 0;
int sum; // Global sum

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *add(void *start); // Adds subrange of values
void *total(); // Computes total for all subranges

int main(int argc, char *argv[]) {
	pthread_t tid1, tid2, tid_total; // thread identifiers
	add_condition.count = 0;
	sem_init(&mutex, 0, 1);
	sem_init(&(add_condition.sem), 0, 0);
	sem_init(&next, 0, 0);
	sum = 0;
	int start1 = 1;
	pthread_create(&tid1, NULL, add, &start1); // Launch add
	int start2 = 51;
	pthread_create(&tid2, NULL, add, &start2); // Launch add
	pthread_create(&tid_total, NULL, total, NULL); // Launch add
	// Join and print result
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid_total, NULL);
	sem_destroy(&mutex);
	sem_destroy(&(add_condition.sem));
}

// Adds subrange of values
void *add(void *start_void) {
	sem_wait(&mutex);
	cwait(&add_condition); // Wait for condition signal
	int *start = start_void; // Cast from void to int
	int i;
	for (i = *start; i <= *start + 49; i++)
		sum += i;
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	pthread_exit(0);
}

// Computes total for all subranges
void *total() {
	sem_wait(&mutex);
	cpost(&add_condition); // Signal for one thread to compute
	cpost(&add_condition); // Signal for another thread to compute
	sleep(1); // Give threads some time to finish
	printf("%d ", sum);
	fflush(stdout);
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	pthread_exit(0);
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
