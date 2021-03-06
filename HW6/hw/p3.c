#include <stdio.h>
#include <stdlib.h> 
#include <semaphore.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h> // For wait

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

int main (){

	const int SIZE = 1;
	const char *name  = "Vincent's C00L Memory";
	const char *name2 = "Vincent's other memory for the max";
	int shm_fd;
	sem_t *sem;
	double *minmem;

	double max;

	// Create shared memory for semaphore
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,SIZE);
	sem = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (sem == MAP_FAILED) {
		printf("Map failed\n");
		exit(0);
	}
	// Set up semaphore
	if(sem_init(sem, 1, 1) < 0) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	// Create shared memory for max
	shm_fd = shm_open(name2, O_CREAT | O_RDWR, 0656);
	ftruncate(shm_fd,SIZE);
	minmem = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (minmem == MAP_FAILED) {
		printf("Map failed\n");
		exit(0);
	}

	*minmem = 9999;

	pid_t pid;
	pid = fork();
	if (pid == 0) {
		int x1;
		double x2, y;

		for(x1 = -4; x1 <= 0; x1++){
			for (x2 = -2; x2 <= 2; x2++){
				y = shubert(x1/2.0, x2);
				sem_wait(sem);
				if(y < *minmem){
					*minmem = y;
					printf("Child found new min: %f\n", *minmem);
					fflush(stdout);
				}
				sem_post(sem);
			}
		}
	}
	else {

		int x1;
		double x2, y;

		for(x1 = 1; x1 <= 4; x1++){
			for (x2 = -2; x2 <= 2; x2++){
				y = shubert(x1/2.0, x2);
				sem_wait(sem);
				if(y < *minmem){
					*minmem = y;
					printf("Parent found new min: %f\n", *minmem);
					fflush(stdout);
				}
				sem_post(sem);
			}
		}

		wait(NULL); // Need to wait because someone needs to clean up
		

		printf("\nMinimum = %f\n", *minmem);

		sem_destroy(sem); // Clean up semaphore


		if (shm_unlink(name) == -1) { // Clean up shared memory
			printf("Error removing %s\n",name);
			exit(0);
		}

		if (shm_unlink(name2) == -1) { // Clean up shared memory
			printf("Error removing %s\n",name);
			exit(0);
		}

	}

}