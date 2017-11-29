#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define TOTAL_STEP 8

double shubert(double x1, double x2);

int main(int argc, char const *argv[]) {

  double x1, x2, y, loc_y = 0;
  int threadCount;
  int tid;
  double step = 0.5, x1_start, x1_stop;
  double min = 99;
  int low_bound = -2;
  int i;
//see openmp3.c for more info
  #pragma omp parallel for private(tid)
  for (i = 0; i <= TOTAL_STEP; i++)
  {//8 is needed because we are doing steps of .5, to allow us to go from -2 to 2 range
    if (tid == 0 && i == 1)
    {
      threadCount = omp_get_num_threads();
      printf("Number of threads = %d\n", threadCount);
    }
    tid = omp_get_thread_num();
    //swings from -2 - 2 as i goes from 0 to 8
    x1 = step * i + low_bound;
    printf("Thread #%d, i = %d, x1 = %.2f\n",tid, i, x1);
    for(x2 = -2; x2<= 2; x2 += step)
    {

      #pragma omp critical
      {
        y = shubert(x1,x2);
        loc_y = (y < loc_y) ? y : loc_y;
        min = (y < min) ? y : min;
      }
    }
    printf("Local min for thread# %d = %.2f\n", tid, loc_y);
  }
  printf("Global min = %.2f\n",min);
  return 0;
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
