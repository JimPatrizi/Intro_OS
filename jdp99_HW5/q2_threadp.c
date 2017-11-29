#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> // For sleep

#define N 1
#define PARENT 0
#define CHILD 1

/* These variables are shared by the thread(s), used by petersons soln */
int flag[2], turn = PARENT;
double min;


void *child(); /* child thread */
double shubert(double x1, double x2); //shubert prototype

int main(int argc, char *argv[])
{
    // These variables are private to the parent
    double x1, x2, y, loc_y;
    double step = 0.5, x1_start, x1_stop;
    // Prepare the child thread
  	pthread_t tid; /* the thread identifier */
  	pthread_attr_t attr; /* set of attributes for the thread */
  	pthread_attr_init(&attr);
  	pthread_create(&tid, &attr, child, NULL);
    //used by petersons soln
  	flag[PARENT] = 1;
  	turn = CHILD;
    //makes sure child process is done so no race for min
  	while (flag[CHILD] && turn == CHILD);
    // Parent does work
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
    flag[PARENT] = 0;
    pthread_join(tid, NULL);
    printf("Global min = %.2f\n", min);
}

void *child()
{
  //Peterson's soln
  flag[CHILD] = 1;
	turn = PARENT;
  // makes sure parent is done so that there is no race condition for min
	while (flag[PARENT] && turn == PARENT);
  //do shubert stuff
  // These variables are private to the child
	double x1, x2, y, loc_y;
	double step = 0.5, x1_start, x1_stop;

	x1_start = -2; x1_stop = 0;
  printf("Child Thread = x1 range: %.2f - %.2f\n", x1_start, x1_stop);
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
  flag[CHILD] = 0;
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
