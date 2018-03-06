#include <stdio.h>
#include <sys/time.h>

#define N 1000000000

int main() {
	long int i;
	long long int x = 0;
	struct timeval start_time, stop_time, elapsed_time;  // timers
	gettimeofday(&start_time,NULL); // Unix timer

	for (i = 0; i < N; i++) {
		x = x + 1;
	}
	for (i = 0; i < N; i++) {
		x = x + 1;
	}

	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	printf("Total time was %f seconds. x = %lld\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
}
