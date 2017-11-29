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
#include <math.h>

/* Global variables are shared by the thread(s) */
sem_t sem;
double min;

void *child(); /* child thread */
double shubert(double x1, double x2);


int main()
{
  if(sem_init(&sem, 0, 1) < 0) { // 1 = multithreaded
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}

  // These variables are private to the parent
  double x1, x2, y;
  double loc_y = 0;
  double step = 0.5, x1_start, x1_stop;


	// Prepare the child thread
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

  sem_wait(&sem);
  x1_start = step; x1_stop = 2;
  printf("Parent Thread = x1 range: %.2f - %.2f\n", x1_start, x1_stop);
  for (x1 = x1_start; x1 <= x1_stop; x1 += step)
  {
      for (x2 = -2; x2 <= 2; x2 += step)
      {
          y = shubert(x1, x2);
          min = (y < min) ? y : min;
          loc_y = (y < loc_y) ? y : loc_y;
      }
  }
  printf("Parent Local min y = %.2f\n", loc_y);
	sem_post(&sem);
	pthread_join(tid, NULL);
  printf("Global min = %.2f\n", min);
	sem_destroy(&sem);
}

void *child() {


  // These variables are private to the child
	double x1, x2, y;
  double loc_y = 0;
	double step = 0.5, x1_start, x1_stop;
  x1_start = -2; x1_stop = 0;
  printf("Child Thread = x1 range: %.2f - %.2f\n", x1_start, x1_stop);
  sem_wait(&sem);
  for (x1 = x1_start; x1 <= x1_stop; x1 += step)
  {
    for (x2 = -2; x2 <= 2; x2 += step)
    {
      y = shubert(x1, x2);
      min = (y < min) ? y : min;
      loc_y = (y < loc_y) ? y : loc_y;
    }
  }
  printf("Child Local min y = %.2f\n", loc_y);
	sem_post(&sem);
	pthread_exit(0);
}

double shubert(double x1, double x2) {
	double sum1 = 0;
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}
