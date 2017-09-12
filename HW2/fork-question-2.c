/**
2. Create a program that creates three processes where the 2 nd process
is the child of the 1 st and the parent of
the 3rd. Display the IDs in the order in which the processes
are created. Below is an example of how the
output might look. Use the “flush(stdout)” and “sleep(#)”
instructions to ensure the lines are printed in the
correct order. Alternately, you may use “wait” instead
of “sleep” if you wish.

Process 1 = 45756, parent = 50076.
Process 2 = 71244, parent = 45756.
Process 3 = 18692, parent = 71244.
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
