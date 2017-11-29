// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
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
#include <errno.h>      
#include <pthread.h>    
#include <semaphore.h>  

int main()
{
	const int SIZE = 1;
	const char *name = "ChrisF";
	int shm_fd;
	sem_t *sem;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,SIZE);
	sem = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (sem == MAP_FAILED) {
		printf("Map failed\n");
		exit(0);
	}

	if(sem_init(sem, 1, 1) < 0) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	pid_t pid;
	pid = fork();
	if (!pid) {
		sem_wait(sem);
		printf("Hello from the child!\n");
		fflush(stdout);
		sem_post(sem);
	}
	else {
		sem_wait(sem);
		int  number;
		printf("Type an integer: ");
		scanf("%d", &number);
		printf("The number you typed was %d.\n", number);
		sem_post(sem);
		wait(NULL);
		sem_destroy(sem);
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(0);
		}
	}
	return 0;
}
