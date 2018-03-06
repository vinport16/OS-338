// Chris Fietkiewicz. Searches for a minimum in Shubert function.
// Note that an interval of 0.5 gives a very inaccurate result but
// works as a fast demonstration. For a realistic search, we would
// use a much small interval and remove the printing from the loop.
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include <pthread.h>

int min1 = 0;
int min2 = 0; /* this data is shared by the thread(s) */

void *child1(); /* the thread */
void *child2(); /* the thread */

// Prototype for computation
double shubert(double x1, double x2);

int main (void){
  pthread_t tid1, tid2; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create the threads */
  pthread_create(&tid1,&attr,child1,NULL);
  pthread_create(&tid2,&attr,child2,NULL);

  /* now wait for the thread to exit */
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);

  printf("min1 = %d\n",min1);
  printf("min2 = %d\n",min2);
  if(min1 > min2){
    printf("smallest min = %d\n", min2);
  }else{
    printf("smallest min = %d\n", min1);
  }


}

void *child1() 
{
  double x1, x2;
  double y = 0;
  for (x1 = 0; x1 <= 2; x1 += 0.5) {
    for (x2 = -2; x2 <= 2; x2 += 0.5) {
      y = shubert(x1, x2);
      printf("%.2f ", y);
      if (y < min1)
        min1 = y;
    }
    printf("\n");
  }
  pthread_exit(0);
}

void *child2() 
{
  double x3, x4;
  double y = 0;
  for (x3 = -2; x3 <= 0; x3 += 0.5) {
    for (x4 = -2; x4 <= 2; x4 += 0.5) {
      y = shubert(x3, x4);
      printf("%.2f ", y);
      if (y < min2)
        min2 = y;
    }
    printf("\n");
  }
  pthread_exit(0);
}

// Compute Shubert value for x1,x2 combination
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