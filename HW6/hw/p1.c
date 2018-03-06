#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <omp.h> 

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
	int x1;
	double x2, y, min = 9999;

	#pragma omp parallel for 
	for(x1 = -4; x1 <= 4; x1++){
		for (x2 = -2; x2 <= 2; x2++){
			y = shubert(x1/2.0, x2);
			#pragma omp critical
			if(y < min){
				min = y;
				printf("new min: %f; found by: %d", min, omp_get_thread_num());
			}
		}
	}

}