
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

//#include <memcpy>

#define NC 900000000
#define NP 800000000

int main(void) {

	// let's do some shared memory
	const int SIZE = 4069;
	const char *name = "Vincent";
	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}


	pid_t pid = fork();

	if(pid == 0){

		char *args[2];
		args[0] = "top";
		//args[1] = "-u";
		//args[2] = "vincent";
		args[1] = 0;
		execvp(args[0], args);

	}else{

		pid = fork();

		if(pid == 0){
			int i;
			for(i = 0; i < NC; i++){ i = i + 0; }

			sprintf(ptr," %d ", i);

		}else{
			int i;
			for(i = 0; i < NP; i++){ i = i + 0; }
			wait(NULL);
			wait(NULL);

			printf("TOTAL IS:  %d;\nSHOULD BE: %d", atoi(ptr)+i, NC+NP );

			/* remove the shared memory segment */
			if (shm_unlink(name) == -1) {
				printf("Error removing %s\n",name);
				exit(-1);
			}
		}
	}

	
}