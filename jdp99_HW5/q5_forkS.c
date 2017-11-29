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
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

double shubert(double x1, double x2); //shubert prototype

/* Global variables are shared by the thread(s) */
sem_t sem;
double min;

int main()
{
  const int SIZE = 4096;
  int shm_fd;
  double *ptr;
  const char *name = "q5";

  if(sem_init(&sem, 1, 1) < 0) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
		exit(0);
	}

  /* create the shared memory segment */
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd,SIZE);
  ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    printf("Map failed\n");
    exit(0);
  }

  pid_t pid;
  pid = fork();
  if(pid > 0)//parent
  {
    // These variables are private to the parent
    double x1, x2, y;
    double loc_y = 0;
    double step = 0.5, x1_start, x1_stop;

    sem_wait(&sem);
    // Parent does work
	  x1_start = step; x1_stop = 2;
    printf("Parent Process = x1 range: %.2f - %.2f\n", x1_start, x1_stop);
	  for (x1 = x1_start; x1 <= x1_stop; x1 += step)
    {
		    for (x2 = -2; x2 <= 2; x2 += step)
        {
		        y = shubert(x1, x2);
		        ptr[0] = (y < ptr[0]) ? y : ptr[0];
            loc_y = (y < loc_y) ? y : loc_y;
		    }
	  }
    printf("Parent Local min y = %.2f\n", loc_y);
    fflush(stdout);
    sem_post(&sem);
    wait(NULL); //Wait for child to finish its computation
    loc_y = 0;
    /* now read from the shared memory region */
    printf("Global min = %.2f\n",ptr[0]);
    sem_destroy(&sem);
  }

  else if(!pid)
  {
    //do shubert stuff
    // These variables are private to the child
    double x1, x2, y, loc_y;
    double step = 0.5, x1_start, x1_stop;

    x1_start = -2; x1_stop = 0;
    printf("Child Process = x1 range: %.2f - %.2f\n", x1_start, x1_stop);
    sem_wait(&sem);
    for (x1 = x1_start; x1 <= x1_stop; x1 += step)
    {
      for (x2 = -2; x2 <= 2; x2 += step)
      {
        y = shubert(x1, x2);
        ptr[0] = (y < ptr[0]) ? y : ptr[0];
        loc_y = (y < loc_y) ? y : loc_y;
      }
    }
    sem_post(&sem);
    printf("Child Local min y = %.2f\n", loc_y);
  }
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
