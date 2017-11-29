/**
James Patrizi
jdp99

* Create a multithreaded program for the
readers/writers solution from Section 5.7.2 using monitors. The writer will periodically change
 the buffer, and the readers will continuously check for changes and print them out.
*
**/
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> // For sleep
#include <sys/types.h>
#include <stdlib.h>

// Struct for condition variable
struct condition {
	sem_t sem; // Semaphore to be initialized to 0
	int count; // Count of threads waiting
};

//Global variables are shared by the thread(s)
sem_t mutex, next;
struct condition rw_condition;
int read_count = 0;
int next_count = 0;
//tells the reader to print just once when set
int version_flag = 0;
int version = 0;

#define BUFFER_SIZE 1000
/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
//prototypes for reader threads
void *reader1();


int main(int argc, char *argv[]) {
	if(sem_init(&mutex, 0, 1) < 0) { //init to 1
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}

  sem_init(&next, 0, 0);

	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_t tid1, tid2;
	pthread_attr_init(&attr);

	rw_condition.count = 0;

	sem_init(&mutex, 0, 1);
	sem_init(&(rw_condition.sem), 0, 0);

	//both threads execute the same code
	pthread_create(&tid1, &attr, reader1, NULL);
	pthread_create(&tid2, &attr, reader1, NULL);

	// Parent utilizes reader writer solution for writers with an explicit sleep as per instructions
	cwait(&rw_condition);
	sprintf(buffer, "%s", "Hello");
	version++;
	//tell reader to print, reader sets it back to 0 after the first read
	version_flag = 1;
	//  printf("version = %d\n",version);
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);

	cpost(&rw_condition);

	sleep(1);
	cwait(&rw_condition);
	sprintf(buffer, "%s", "World");
	version++;
	version_flag = 1;
	//  printf("version = %d\n",version);
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	cpost(&rw_condition);

	sleep(1);
	cwait(&rw_condition);
	sprintf(buffer, "%s", "From");
	version++;
	version_flag = 1;
	//  printf("version = %d\n",version);
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	cpost(&rw_condition);

	sleep(1);
	cwait(&rw_condition);
	sprintf(buffer, "%s", "Reader");
	version++;
	version_flag = 1;
	//  printf("version = %d\n",version);
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);

	cpost(&rw_condition);
	sleep(1);
	version++;
	// Join
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	sem_destroy(&mutex);
	sem_destroy(&next);
	sem_destroy(&(rw_condition.sem));
	return 0;
}

//reader implements reader solution for reader writer problem
void *reader1()
{
  while(1)
  {
    sem_wait(&mutex);
    //printf("Mutex acquired by reader %d\n", mutex);
    read_count++;

		if (next_count > 0)
			sem_post(&next);
		else
			sem_post(&mutex);

    long long tid = (long long) pthread_self();

  //  printf("oldversion = %d\n", oldversion);
  //only print once per buffer change
    if(version_flag)
    {
      printf("My thread ID is %lld & buffer: %s.\n", tid, buffer);
      //set flag back to 0 so it only prints once per buffer element
      version_flag = 0;
    }
    sem_wait(&mutex);
    //printf("Reader is finished reading\n");
    read_count--;

    if(read_count == 0){
      cpost(&rw_condition);
    }
    sem_post(&mutex);
    //after buffer goes through all 4 written strings, have thread exit
    if (version == 5) {
      pthread_exit(0);
      exit(0);
    }
  }
}

// Semaphore implementation of conditional wait
void cwait(struct condition *c) {
	c->count++;
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	sem_wait(&(c->sem));
	c->count--;
}

// Semaphore implementation of conditional signal
void cpost(struct condition *c) {
	if (c->count > 0) {
		next_count++;
		sem_post(&(c->sem));
		sem_wait(&next);
		next_count--;
	}
}
