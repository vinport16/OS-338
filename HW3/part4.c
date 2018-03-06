# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/types.h>

int main(){
	pid_t pid = fork();
	if(pid == 0){
		pid_t pid2 = fork();
		if(pid2 == 0){
			printf("process 3 = %d; parent = %d\n", getpid(), getppid());
		}else{
			wait(&pid2);
			printf("process 2 = %d; parent = %d\n", getpid(), getppid());
		}
	}else{
		wait(&pid);
		printf("process 1 = %d; parent = %d\n", getpid(), getppid());
	}
}