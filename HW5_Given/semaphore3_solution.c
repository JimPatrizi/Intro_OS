// Chris Fietkiewicz (chris.fietkiewicz@case.edu)

#include <stdio.h>
#include <math.h>
#include <unistd.h>     
#include <sys/types.h>  
#include <errno.h>      
#include <stdlib.h>     
#include <pthread.h>    
#include <semaphore.h>  

/* Global variables are shared by the thread(s) */
sem_t sem;

void *child(); /* child thread */

int main(int argc, char *argv[])
{
	if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

	sem_wait(&sem);
	int  number;
	printf("Type an integer: ");
	scanf("%d", &number);
	printf("The number you typed was %d.\n", number);
	sem_post(&sem);
	pthread_join(tid, NULL);
	sem_destroy(&sem);
}

void *child() {
	sem_wait(&sem);
	printf("Hello from the child!\n");
	fflush(stdout);
	sem_post(&sem);
	pthread_exit(0);
}
