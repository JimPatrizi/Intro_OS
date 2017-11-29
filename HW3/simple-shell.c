/**
 * Jim Patrizi
 * jdp99
 * EECS 338 HW3 Q1
 * Simple shell interface program
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LINE		80 /* 80 chars per line, per command */


void parse(char* line, char **argv){
	int i = 0;
	while (argv[i] != NULL)
		argv[++i] = strtok(NULL, " "); //parses line replacing NULL with open chars
}

void executevp(char **argv){
	pid_t pid;
	int complete;//child pid to wait on in wait()

	pid = fork();

	if(pid == 0){
		if(execvp(argv[0], argv) < 0){ //exec functions only return -1 if failure
			printf("Execvp failure, exiting..\n");
			exit(1);
		}
	}
	else{
		while (wait(&complete) != pid) //wait for parent process, waits for child
		;
	}
}

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char line[50];

    while (1){
			printf("Jim's Shell-> ");
			gets(line);
		  printf("\n");
			parse(line, args);
			if(!strcmp(args[0], "exit")) // exits if users says so
				exit(0);
			executevp(args); // Execute the command
		}
}
