// Chris Fietkiewicz (chris.fietkiewicz@case.edu)

#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> // For sleep

#define N 1
#define PARENT 0
#define CHILD 1

void *child(); /* child thread */

int main(int argc, char *argv[])
{
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);
	int  number;
	printf("Type an integer: ");
	scanf("%d", &number);
	printf("The number you typed was %d.\n", number);
	pthread_join(tid, NULL);
}

void *child() {
	printf("Hello from the child!\n");
	fflush(stdout);
	pthread_exit(0);
}
