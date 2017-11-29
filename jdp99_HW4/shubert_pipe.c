/**
HW4
Jim Patrizi jdp99

Q2
Shubert function with split work with forks and pipes
*/

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

double write_data[BUFFER_SIZE];
double read_data[BUFFER_SIZE];
pid_t pid;
int fd[2];



double shubert(double x1, double x2);

int main ()
{
  double x1, x2, y, min = 0;

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

  pid = fork();
  //child process does -2 - 0
  if(pid == 0)
  {
  for (x1 = -2; x1 <= 0; x1 += 0.5) {
    for (x2 = -2; x2 <= 0; x2 += 0.5) {
      y = shubert(x1, x2);
      printf("%.2f\n", y);
      if (y < min)
        min = y;
    }
  }
  //child writes its min into pipe and exits
  write_data[0] = min;
  write(fd[WRITE_END], write_data, sizeof(write_data));
  exit(EXIT_SUCCESS);
  }
  //Parent Process does its part of computation
  else if (pid > 0)
  {
    for (x1 = 0; x1 <= 2; x1 += 0.5) {
      for (x2 = 0; x2 <= 2; x2 += 0.5) {
        y = shubert(x1, x2);
        printf("%.2f\n", y);
        if (y < min)
          min = y;
      }
    }
  //parent reads pipes min and compares it to its local min to find global min
  read(fd[READ_END], read_data, sizeof(write_data));
  if(read_data[0] < min){
    min = read_data[0];
  }
  //print global min
  printf("min = %.2f\n", min);
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
