/**
Jim Patrizi - jdp99
EECS 338 - Intro to OS
HW2 Question 3
**/

 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <unistd.h>
 #include <stdlib.h>

 int main ( int argc, char *argv[] )
 {
 	pid_t pid, pid_parent_og, child_parent, child;

  pid = fork();
  wait(NULL);
 	pid_parent_og = getppid();
 	child_parent = getpid();
  if (pid != 0){ //parent process only
  printf("Process 1 = %d, ", child_parent);//2nd process runs this, need to move prints to after 2nd process terminates
 	printf("parent = %d.\n", pid_parent_og);
 	fflush(stdout);
  }


 	if (pid == 0) { /* child process, 1 */
    pid = fork();
 		wait(NULL);
 		child = getpid();
 		child_parent = getppid();
    if(pid != 0){ //parent process, child 1 only
 		printf("Process 2 = %d, ", child);//put prints after 3rd process terminates
 		printf("parent = %d.\n", child_parent);
 		fflush(stdout);
    }


		if (pid == 0) { //child of process 2, child 3
		child = getpid();
		child_parent = getppid();
		printf("Process 3 = %d, ", child);
		printf("parent = %d.\n", child_parent);
		fflush(stdout);
		}
 	}
	return 0;
 }
