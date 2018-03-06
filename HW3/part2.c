# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/types.h>

int main(){
	pid_t pid = fork();
	if(pid > 0){
		pid_t pid2 = fork();
		if(pid2 > 0){
			printf("process 1 = %d; parent = %d\n", getpid(), getppid());
			pid = wait(NULL);
			printf("child %d exited\n", pid);
			pid2 = wait(NULL);
			printf("child %d exited\n", pid2);
		}else{
			sleep(2);
			printf("process 3 = %d; parent = %d\n", getpid(), getppid());
		}
	}else{
		sleep(1);
		printf("process 2 = %d; parent = %d\n", getpid(), getppid());
		sleep(2);
	}
}