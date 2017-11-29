// Chris Fietkiewicz (chris.fietkiewicz@case.edu)

#include <pthread.h>
#include <stdio.h>

int sum; /* this data is shared by the thread(s) */

void *child(); /* the thread */

int main(int argc, char *argv[])
{
	pthread_t tid; /* the thread identifiers */

	/* create the threads */
	pthread_create(&tid, NULL, child, NULL);

	/* now wait for the thread to exit */
	pthread_join(tid,NULL);

	printf("sum = %d\n",sum);
}

void *child() 
{
	int i;
	sum = 0;
	for (i = 1; i <= 10; i++)
		sum += i;
	pthread_exit(0);
}
