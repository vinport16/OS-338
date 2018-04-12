// Chris Fietkiewicz. OpenMP solution with "for" clause
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>

double shubert(double step, double x2);

int main (){
	double min = 0, stepSize = 0.5;
	int nSteps = (int) (4.0 / stepSize);
	int step; // Step index

	#pragma omp parallel for
	for (step = 0; step <= nSteps; step++) {
		double y, x2, x1 = step * stepSize - 2;
		for (x2 = -2; x2 <= 2; x2 += stepSize) {
			y = shubert(x1, x2);
			printf("%.2f ", y);
			#pragma omp critical
			if (y < min)
				min = y;
		}
		printf("\n", y);
	}
	printf("min = %.2f ", min);
}

double shubert(double step, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * step + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}