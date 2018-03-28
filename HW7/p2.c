#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 20

char buffer[BUFFER_SIZE];
sem_t rw_mutex, mutex;
int version = 0;
int read_count = 0;

void *child();

int main(int argc, char *argv[]) {
	if(sem_init(&rw_mutex, 0, 1) < 0 || sem_init(&mutex, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, child, NULL);
	pthread_create(&tid2, NULL, child, NULL);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Roses are red");
	version = 1;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Protests are great");
	version = 2;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Before we get gun control");
	version = 3;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Disarm the state");
	version = 4;
	sem_post(&rw_mutex);
	sleep(1);

// Join
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	sem_destroy(&rw_mutex);
	sem_destroy(&mutex);

}

void *child() {
	pthread_t tid = pthread_self();
	int child_version = 0;

	while(child_version != 4){
		sem_wait(&mutex);
		read_count++;
		if (read_count == 1) {
				sem_wait(&rw_mutex);
		}
		sem_post(&mutex);

		if (child_version != version){
			printf("%d: %s\n", (int)tid, buffer);
			child_version = version;
		}

		sem_wait(&mutex);
		read_count--;

		if (read_count == 0){
			sem_post(&rw_mutex);
		}
		sem_post(&mutex);

	}
	pthread_exit(0); 
}