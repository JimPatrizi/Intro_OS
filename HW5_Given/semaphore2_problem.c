// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>      
#include <pthread.h>    
#include <semaphore.h>  

int main()
{
	pid_t pid;
	pid = fork();
	if (!pid) {
		printf("Hello from the child!\n");
		fflush(stdout);
	}
	else {
		int  number;
		printf("Type an integer: ");
		scanf("%d", &number);
		printf("The number you typed was %d.\n", number);
		wait(NULL);
	}
	return 0;
}
