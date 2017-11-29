#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

/**
James Patrizi
jdp99

* Implement a semaphorebased solution for the bounded-buffer problem presented in Section 5.7.1
* of the textbook. Use multithreading with one thread that is a producer and
* another thread that is a consumer
*
**/

#define BUFFER_SIZE   5
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int buffer[BUFFER_SIZE];
int values[] = {0,1,2,3,4,5,6,7,8,9}; //to be inserted by producer
int n = NELEMS(values);
sem_t mutex;
sem_t empty;
sem_t full;

void *producer(); //producer thread
void *consumer(); //consumer thread

int main(int argc, char const *argv[]) {

  if(sem_init(&mutex, 0, 1) < 0) { // mutex is mutal exclusion
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  if(sem_init(&empty, 0, BUFFER_SIZE) < 0) { //empty init to buffer size
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  if(sem_init(&full, 0, 0) < 0) { //0 for full
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  pthread_attr_t attr; /* set of attributes for the thread */
  pthread_t tid1, tid2;
	pthread_attr_init(&attr);

  pthread_create(&tid1, &attr, producer, NULL);
  pthread_create(&tid2, &attr, consumer, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  sem_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}

void *producer() {
    int i;
    for (i = 0; i < n; i++) {
      //  sem_wait(&empty);
        sem_wait(&mutex);
        buffer[i % BUFFER_SIZE] = values[i];
        sem_post(&mutex);
        sem_post(&full);
    }
    pthread_exit(0);
}

void *consumer() {
    int i;
    for (i = 0; i < n; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        printf("%d\n", buffer[i % BUFFER_SIZE]);
        //buffer[i % n] = 0;
        sem_post(&mutex);
        //sem_post(&empty);
    }
    pthread_exit(0);
}
