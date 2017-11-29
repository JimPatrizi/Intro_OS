#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

int N = 4;
int i;
pid_t pid = 0;
int processID = 1;
for (i = 2; i <= N && !pid; i++) {
  pid = fork();
  if(pid == 0)
    processID = i;
}

wait(NULL);
printf("\nHello from %d\n", processID);

}
