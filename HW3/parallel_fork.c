/**
 * Jim Patrizi
 * jdp99
 * EECS 338 HW3 Q2
 * Parallel Computations
 *
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 1000000000

int main() {
   int num_process;
   long int i;
 	 long long int x = 0;
 	 pid_t pid;
 	 struct timeval start_time, stop_time, elapsed_time;  // timers


   printf("How many processes? ");
   scanf("%d", &num_process);
   printf("You entered: %d\n", num_process);

   if (num_process <= 0){
 		perror("Can't do <= 0 Processes....\n");
 		exit(EXIT_FAILURE);
 	  }

   int forkNum = 1;
   for (; forkNum <= num_process-1; ++forkNum)//-1 because the parent counts as a process, if we wanted parent 1 too many processes.
   {
     pid = fork();
     gettimeofday(&start_time,NULL); // Unix timer
     wait(NULL);//have parent process wait until children are done,
     if  (pid == 0){
       for (i = 0; i < N/num_process; i++) {
         x = x + 1;
       }
      gettimeofday(&stop_time,NULL);
 			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
 			printf("Process %i: Total time was %f seconds. x = %lld.\n", forkNum, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
 			exit(EXIT_SUCCESS);
     }
     else if (pid < 0){
       perror("Parent didn't properly fork! What is wrong...\n");
       exit(EXIT_FAILURE);
     }
   }
   // To be executed by the initial process
   gettimeofday(&start_time,NULL); // Unix timer
   for (i = 0; i < N/num_process; i++) {
     x = x + 1;
   }
   gettimeofday(&stop_time,NULL);
   timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
   printf("Process %i: Total time was %f seconds. x = %lld.\n", num_process, elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
   fflush(stdout);
}
