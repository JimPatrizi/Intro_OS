#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

/**
James Patrizi
jdp99

* Create a multithreaded program for the
readers/writers solution from Section 5.7.2. The writer will periodically change
 the buffer, and the readers will continuously check for changes and print them out.
*
**/

sem_t mutex;
sem_t rw_mutex;
int read_count = 0;

//tells the reader to print just once when set
int version_flag = 0;
int version = 0;

#define BUFFER_SIZE 1000

/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];

//prototypes for reader threads
void *reader1();
void *reader2();

//parent is the writer
int main(int argc, char const *argv[]) {
  if(sem_init(&rw_mutex, 0, 1) < 0) { // mutex is mutal exclusion, init to 1
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  if(sem_init(&mutex, 0, 1) < 0) { //init to 1
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  pthread_attr_t attr; /* set of attributes for the thread */
  pthread_t tid1, tid2;
	pthread_attr_init(&attr);

  //both threads execute the same code
  pthread_create(&tid1, &attr, reader1, NULL);
  pthread_create(&tid2, &attr, reader1, NULL);

  // Parent utilizes reader writer solution for writers with an explicit sleep as per instructions
  sem_wait(&rw_mutex);
  sprintf(buffer, "%s", "Hello");
  version++;
  //tell reader to print, reader sets it back to 0 after the first read
  version_flag = 1;
//  printf("version = %d\n",version);
  sem_post(&rw_mutex);

  sleep(1);
  sem_wait(&rw_mutex);
  sprintf(buffer, "%s", "World");
  version++;
  version_flag = 1;
//  printf("version = %d\n",version);
  sem_post(&rw_mutex);

  sleep(1);
  sem_wait(&rw_mutex);
  sprintf(buffer, "%s", "From");
  version++;
  version_flag = 1;
//  printf("version = %d\n",version);
  sem_post(&rw_mutex);

  sleep(1);
  sem_wait(&rw_mutex);
  sprintf(buffer, "%s", "Reader");
  version++;
  version_flag = 1;
//  printf("version = %d\n",version);
  sem_post(&rw_mutex);
  sleep(1);
  version++;
  // Join
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  sem_destroy(&mutex);
  sem_destroy(&rw_mutex);
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

    if (read_count == 1) {
      sem_wait(&rw_mutex);
    }

    sem_post(&mutex);
    //printf("Mutex returned by reader %d\n", mutex);

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
      sem_post(&rw_mutex);
    }
    sem_post(&mutex);
    //after buffer goes through all 4 written strings, have thread exit
    if (version == 5) {
      pthread_exit(0);
      exit(0);
    }
  }
}
