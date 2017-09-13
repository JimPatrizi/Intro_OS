/**
Jim Patrizi - jdp99
EECS 338 - Intro to OS
HW2 Question 2
**/

 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <unistd.h>
 #include <stdlib.h>

 int main ( int argc, char *argv[] )
 {
 	pid_t pid, pid_parent_og, child_parent, child;

 	pid_parent_og = getppid();
 	child_parent = getpid();
  printf("Process 1 = %d, ", child_parent);
 	printf("parent = %d.\n", pid_parent_og);
 	fflush(stdout);
  pid = fork();

 	if (pid == 0) { /* child process, 1 */
 		wait(NULL);
 		child = getpid();
 		child_parent = getppid();
 		printf("Process 2 = %d, ", child);
 		printf("parent = %d.\n", child_parent);
 		fflush(stdout);
		wait(NULL);
		pid = fork();

		if (pid == 0) { //child of process 2, child 3
		wait(NULL);
		child = getpid();
		child_parent = getppid();
		printf("Process 3 = %d, ", child);
		printf("parent = %d.\n", child_parent);
		fflush(stdout);
		}
 	}
	return 0;
 }
