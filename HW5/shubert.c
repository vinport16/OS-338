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

#define BUFFER_SIZE 25
#define READ_END  0
#define WRITE_END 1

// Prototype for computation
double shubert(double x1, double x2);

int main (){
  double x1, x2, y, min = 0;

  double write_data[BUFFER_SIZE] = {3.14, 2.71};
  double read_data[BUFFER_SIZE];
  int fd[2];

  /* create the pipe */
  if (pipe(fd) == -1) {
    fprintf(stderr,"Pipe failed");
    return 1;
  }
  //printf("File descriptors... read:%d, write:%d\n", fd[READ_END], fd[WRITE_END]);


  pid_t pid = fork();

  if(!pid){

    for (x1 = 0; x1 <= 2; x1 += 0.5) {
    for (x2 = -2; x2 <= 2; x2 += 0.5) {
      y = shubert(x1, x2);
      //printf("%.2f ", y);
      if (y < min)
        min = y;
    }
    //printf("\n", y);
  }
  printf("parent minimum = %.2f \n", min);
  read(fd[READ_END], read_data, sizeof(write_data)); ///read data
  if(min > *read_data){
    printf("smallest min = %f\n", *read_data);
  }else{
    printf("smallest min = %f\n", min);
  }

  }else{

    for (x1 = -2; x1 <= 0; x1 += 0.5) {
    for (x2 = -2; x2 <= 2; x2 += 0.5) {
      y = shubert(x1, x2);
      //printf("%.2f ", y);
      if (y < min)
        min = y;
    }
    //printf("\n", y);
  }
  printf("child minimum = %.2f \n", min);
  write_data[0] = min;
  write(fd[WRITE_END], write_data, sizeof(write_data)); /// read data 

  }
  
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