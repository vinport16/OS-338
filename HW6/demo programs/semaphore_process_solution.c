// Chris Fietkiewicz. Uses a semaphore to control parent child order.
// Uses fork with shared memory.
#include <unistd.h> // For fork and shared memory
#include <stdio.h> // For printf
#include <stdlib.h> // For exit
#include <fcntl.h> // For shared memory
#include <sys/shm.h> // For shared memory
#include <sys/mman.h> // For shared memory
#include <sys/wait.h> // For wait
#include <semaphore.h> // For semaphores

int main() {
	// Set up shared memory for semaphore
	const int SIZE = 1;
	const char *name = "ChrisF";
	int shm_fd;
	sem_t *sem;
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

	pid_t pid;
	pid = fork();
	if (pid == 0) { // Child tries to give a speech
		sem_wait(sem); // Decrement semaphore and lock/wait till available
		printf("Child: I have the floor!\n");
		fflush(stdout);
		printf("Child: I hope my parent doesn't interrupt me.\n");
		fflush(stdout);
		printf("Child: I have a right to be heard.\n");
		fflush(stdout);
		printf("Child: OK, I'm done.\n");
		fflush(stdout);
		sem_post(sem); // Done, so increment semaphore
	}
	else { // Parent tries to give a speech
		sem_wait(sem); // Decrement semaphore and lock/wait till available
		printf("Parent: I have the floor!\n");
		fflush(stdout);
		printf("Parent: I hope my child doesn't interrupt me.\n");
		fflush(stdout);
		printf("Parent: They never listen.\n");
		fflush(stdout);
		printf("Parent: Now, I am finished.\n");
		fflush(stdout);
		sem_post(sem); // Done, so increment semaphore
		wait(NULL); // Need to wait because someone needs to clean up
		sem_destroy(sem); // Clean up semaphore
		if (shm_unlink(name) == -1) { // Clean up shared memory
			printf("Error removing %s\n",name);
			exit(0);
		}
	}
	return 0;
}
