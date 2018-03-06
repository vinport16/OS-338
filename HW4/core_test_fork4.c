#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 500000000

int main() {
	long int i;
	long long int x = 0;
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
	for (i = 0; i < N; i++) {
		x = x + 1;
	}
	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
}
