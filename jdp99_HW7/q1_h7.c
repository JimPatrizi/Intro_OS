/**
James Patrizi
jdp99

* Implement a semaphorebased solution for the bounded-buffer problem presented in Section 5.7.1 using monitors
* Use multithreading with one thread that is a producer and
* another thread that is a consumer
*
**/

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> // For sleep

// Struct for condition variable
struct condition {
	sem_t sem; // Semaphore to be initialized to 0
	int count; // Count of threads waiting
};

#define BUFFER_SIZE   5
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int buffer[BUFFER_SIZE] = {};
int values[] = {10,20,30,40,50,60,70,80,90,100}; //to be inserted by producer
int n = NELEMS(values);

//Global variables are shared by the thread(s)
sem_t mutex, next;
struct condition not_full, not_empty;
int next_count = 0;
int counter = 0;

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *producer(); //producer thread
void *consumer(); //consumer thread
//void *add(void *start); // Adds subrange of values
//void *total(); // Computes total for all subranges

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2; // thread identifiers
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);

	not_full.count = 0;// Initialize condition variable count (nobody waiting)
	not_empty.count = 0;
	sem_init(&mutex, 0, 1);
	sem_init(&(not_full.sem), 0, 0);
	sem_init(&(not_empty.sem), 0, 0);
	sem_init(&next, 0, 0);

	pthread_create(&tid1, &attr, producer, NULL);
  pthread_create(&tid2, &attr, consumer, NULL);

	// Join and print result
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	sem_destroy(&mutex);
	sem_destroy(&(not_full.sem));
	sem_destroy(&(not_empty.sem));

	return 0;
}

void *producer() {

//printf("here producer\n");
  int i;
  for (i = 0; i < n; i++)
	{
		sem_wait(&mutex);
		if(counter == BUFFER_SIZE)
		{
			cwait(&not_full);
		}
    buffer[i % BUFFER_SIZE] = values[i];
		cpost(&not_empty);
		sem_post(&mutex);
		//sleep(0.5);
		counter++;
  }
	cpost(&not_empty);
	if(next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
  pthread_exit(0);
}

void *consumer() {
  int i;
  for (i = 0; i < n; i++)
	{
		sem_wait(&mutex);
		if(counter == 0)
		{
			printf("consumer wait\n");
			fflush(stdout);
			cwait(&not_empty);
			printf("consumer done waiting\n");
			fflush(stdout);
		}
    //printf("%d\n", buffer[i % BUFFER_SIZE]);
		printf("buffer = %d\n", buffer[i % BUFFER_SIZE]);
		fflush(stdout);
		//buffer[i % n] = 0;
		cpost(&not_full);
		counter--;
    sem_post(&mutex);
  }
	if(next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	pthread_exit(0);
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
