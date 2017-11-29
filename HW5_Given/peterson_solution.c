// Chris Fietkiewicz (chris.fietkiewicz@case.edu)

#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> // For sleep

#define N 1
#define PARENT 0
#define CHILD 1

/* These variables are shared by the thread(s) */
int flag[2], turn = PARENT;

void *child(); /* child thread */

int main(int argc, char *argv[])
{
	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);
	flag[PARENT] = 1;
	turn = CHILD;
	while (flag[CHILD] && turn == CHILD);
	int  number;
	printf("Type an integer: ");
	scanf("%d", &number);
	printf("The number you typed was %d.\n", number);
	flag[PARENT] = 0;
	pthread_join(tid, NULL);
}

void *child() {
	flag[CHILD] = 1;
	turn = PARENT;
	while (flag[PARENT] && turn == PARENT);
	printf("Hello from the child!\n");
	fflush(stdout);
	flag[CHILD] = 0;
	pthread_exit(0);
}
