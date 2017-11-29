/**
HW4
Jim Patrizi jdp99

Q3
Shubert function split work with threads
*/
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
#include <pthread.h>

double shubert(double x1, double x2);
void *child(); /* the thread */
double x1, x2, x3, x4, y, y_, min = 0; //share min between two threads

int main (){

  pthread_t tid; /* the thread identifiers */
  /* create the threads */
	pthread_create(&tid, NULL, child, NULL);
  for (x1 = 0; x1 <= 2; x1 += 0.5) {
    for (x2 = 0; x2 <= 2; x2 += 0.5) {
      y = shubert(x1, x2);
      printf("%.2f\n ", y);
      if (y < min)
        min = y;
    }
  }
  /* now wait for the thread to exit */
  pthread_join(tid,NULL);
  printf("\nmin = %.2f\n", min);
}

void *child()
{
  for (x3 = -2; x3 <= 0; x3 += 0.5) {
    for (x4 = -2; x4 <= 0; x4 += 0.5) {
      y_ = shubert(x3, x4);
      printf("%.2f\n", y_);
      if (y_ < min)
        min = y_;
    }
  }
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
