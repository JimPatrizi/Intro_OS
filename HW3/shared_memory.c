/**
 * Jim Patrizi
 * jdp99
 * EECS 338 HW3 Q3
 * Shared Memory
 *
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
  pid_t pid;
  const int SIZE = 4096;
	const char *name = "OS";
	const char *message0= "Hello ";
	const char *message1= "From your ";
	const char *message2= "CHILD!\n";

	int shm_fd;
	char *ptr;

  /* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  /* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

  /* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

  pid = fork();
  wait(NULL);
  if(pid == 0)
  {
    sprintf(ptr,"%s",message0);
  	ptr += strlen(message0);
  	sprintf(ptr,"%s",message1);
  	ptr += strlen(message1);
  	sprintf(ptr,"%s",message2);
  	ptr += strlen(message2);
  }
  else //if(pid < 0) //parent prints childs statement
  {
    shm_fd = shm_open(name, O_RDONLY, 0666);
  	if (shm_fd == -1) {
  		printf("shared memory failed\n");
  		exit(-1);
  	}
    /* now map the shared memory segment in the address space of the process */
  	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  	if (ptr == MAP_FAILED) {
  		printf("Map failed\n");
  		exit(-1);
  	}
    /* now read from the shared memory region */
    printf("%s",ptr);
    /* remove the shared memory segment */
  	if (shm_unlink(name) == -1) {
  		printf("Error removing %s\n",name);
  		exit(-1);
  	}
  }
  return 0;
}
