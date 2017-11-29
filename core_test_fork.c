#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 1000000000

int main() {
	long int i = 0;
	long long int x = 0;
	pid_t pid;
	struct timeval start_time, stop_time, elapsed_time;  // timers
	
	int numProcsToFork;
	
	printf("Enter the number of processes: ");
	
	// Read the number of processes
	scanf("%d", &numProcsToFork);
	
	// Alert the user that they're retarded
	if (numProcsToFork <= 0)
	{
		perror("YOU FUCKING MORON HOW DO YOU EXPECT ME TO HAVE 0 OR FEWER PROCESSES DO THIS?!\n");
		exit(EXIT_FAILURE);
	}
	
	
	int forkNum = 1;
	// Execute one less than the number of requested processes,
	// as the initial process will execute the last sum.
	for (; forkNum <= numProcsToFork-1; ++forkNum)
	{
		pid = fork();
		gettimeofday(&start_time,NULL); // Unix timer
		if (pid == 0) { /* child process */
			for (i = 0; i < N/numProcsToFork; i++) {
				x = x + 1;
			}
			gettimeofday(&stop_time,NULL);
			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
			printf("Process %i: Total time was %f seconds. x = %lld.\n", forkNum, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
			fflush(stdout);
			exit(EXIT_SUCCESS);
		}
		else if (pid < 0)
		{
			perror("Something's fucked!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// To be executed by the initial process
	gettimeofday(&start_time,NULL); // Unix timer
	for (i = 0; i < N/numProcsToFork; i++) {
		x = x + 1;
	}
	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	printf("Process %i: Total time was %f seconds. x = %lld.\n", numProcsToFork, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
}
