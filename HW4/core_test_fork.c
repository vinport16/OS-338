	#include <stdio.h>
	#include <sys/time.h>
	#include <sys/wait.h>
	#include <unistd.h>

	#define N1 3000000000
	#define N2 1500000000
	#define N3 1000000000
	#define N4  750000000
int main() {
	long int i;
	long long int x = 0;

	int pros;
	scanf("%d", &pros);

	if(pros == 1){

		pid_t pid;
		struct timeval start_time, stop_time, elapsed_time;  // timers
		gettimeofday(&start_time,NULL); // Unix timer

		for (i = 0; i < N1; i++) {
			x = x + 1;
		}

		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Total time was %f seconds. x = %lld\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
	
	}else if(pros == 2){

		pid_t pid;
		struct timeval start_time, stop_time, elapsed_time;  // timers
		gettimeofday(&start_time,NULL); // Unix timer
		pid = fork();
		if (pid > 0) {
			for (i = 0; i < N2; i++) {
				x = x + 1;
			}
			gettimeofday(&stop_time,NULL);
			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
			printf("Parent total time was %f seconds. x = %lld.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
		}
		else if (pid == 0) { /* child process */
			for (i = 0; i < N2; i++) {
				x = x + 1;
			}
			gettimeofday(&stop_time,NULL);
				timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
				printf("Child total time was %f seconds. x = %lld.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
			}

	}else if(pros == 3){

		pid_t pid;
		struct timeval start_time, stop_time, elapsed_time;  // timers
		gettimeofday(&start_time,NULL); // Unix timer
		pid = fork();
		if (pid > 0) {
			pid = fork();
		}
		for (i = 0; i < N3; i++) {
			x = x + 1;
		}
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);

	}else{

		pid_t pid;
		struct timeval start_time, stop_time, elapsed_time;  // timers
		gettimeofday(&start_time,NULL); // Unix timer
		pid = fork();
		if (pid > 0) {
			pid = fork();
			if (pid > 0) {
				pid = fork();
			}
		}
		for (i = 0; i < N4; i++) {
			x = x + 1;
		}
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
		
	}


}
