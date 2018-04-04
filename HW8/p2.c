#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFFER_SIZE 20
struct condition {
	sem_t sem;
	int count;
};
/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t mutex, next, readers;
struct condition not_reading;
int next_count = 0;
int count = 0;
int version = 0;
int readcount = 0;

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
	not_reading.count = 0;

	if(sem_init(&mutex, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore mutex.\n");
		exit(0);
	}

	if(sem_init(&next, 0, 0) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore next.\n");
		exit(0);
	}

	if(sem_init(&(not_reading.sem), 0, 0) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore not_reading.\n");
		exit(0);
	}

	if(sem_init(&readers, 0, 0) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore readers.\n");
		exit(0);
	}

	pthread_t tid; /* the thread identifiers */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, producer, NULL);

	pthread_t tid2; /* the thread identifiers */
	pthread_attr_t attr2; /* set of attributes for the thread */
	pthread_attr_init(&attr2);
	pthread_create(&tid2, &attr2, consumer, NULL);

	pthread_t tid3; /* the thread identifiers */
	pthread_attr_t attr3; /* set of attributes for the thread */
	pthread_attr_init(&attr3);
	pthread_create(&tid3, &attr3, consumer, NULL);

	

	// Join and print result
	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);
	sem_destroy(&mutex);
	sem_destroy(&next);
	sem_destroy(&(not_reading.sem));
	sem_destroy(&readers);
}

void *producer() {

	enter_monitor();
	printf("writer entered monitor\n");
	cwait(&not_reading);
	printf("not reading happened\n");

	sprintf(buffer, "%s", "cha-cha");
	printf("hahah ok\n");
	version++;

	exit_monitor();

	sleep(1);


	enter_monitor();
	cwait(&not_reading);

	sprintf(buffer, "%s", "super user do");
	version++;

	exit_monitor();

	sleep(1);


	enter_monitor();
	cwait(&not_reading);

	sprintf(buffer, "%s", "ok ok ok ok : )");
	version++;

	exit_monitor();

	sleep(1);


	enter_monitor();
	cwait(&not_reading);

	sprintf(buffer, "%s", "foo bar");
	version++;

	exit_monitor();

	sleep(1);

}

void *consumer(){

	int i, input = 0;
	char data[20];
	i = 1;

	while (i < 5){
		
		enter_monitor();

		sem_wait(&readers);
		readcount++;
		sem_post(&readers);
		
		if(version == i){
			sprintf(data, "%s", buffer);
			printf("%s\n", data);
			i++;
		}

		sem_wait(&readers);
		readcount--;
		sem_post(&readers);

		if(readcount == 0){
			cpost(&not_reading);
		}

		exit_monitor();
		
		sleep(1);
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