#include <unistd.h> // For fork and shared memory
#include <stdio.h> // For printf
#include <stdlib.h> // For exit
#include <fcntl.h> // For shared memory
#include <sys/shm.h> // For shared memory
#include <sys/mman.h> // For shared memory
#include <sys/wait.h> // For wait
#include <semaphore.h> // For semaphores

double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}


sem_t sem; // Global semaphore
double min = 9999;

void *child() {

	int x1;
	double x2, y;

	for(x1 = -4; x1 <= 0; x1++){
		for (x2 = -2; x2 <= 2; x2++){
			y = shubert(x1/2.0, x2);
			sem_wait(&sem);
			if(y < min){
				min = y;
				printf("Child found new min: %f\n", min);
				fflush(stdout);
			}
			sem_post(&sem);
		}
	}

	pthread_exit(0);
}

int main (){

	if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, child, NULL);

	int x1;
	double x2, y;

	for(x1 = 1; x1 <= 4; x1++){
		for (x2 = -2; x2 <= 2; x2++){
			y = shubert(x1/2.0, x2);
			sem_wait(&sem);
			if(y < min){
				min = y;
				printf("Parent found new min: %f\n", min);
				fflush(stdout);
			}
			sem_post(&sem);
		}
	}
	
	pthread_join(tid, NULL); // Need to wait because someone needs to clean up
	sem_destroy(&sem); // Clean up semaphore

	printf("\nFinal min: %f\n", min);

}

