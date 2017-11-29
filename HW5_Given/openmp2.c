// Chris Fietkiewicz
#include <stdio.h>
#include <omp.h> 

int main () { 
  int i;
  /* Fork a team of threads with each thread having a private tid variable */ 
  #pragma omp parallel for
  for (i = 1; i <= 10; i++) {
    /* Obtain and print thread id */ 
    int tid = omp_get_thread_num(); 
    printf("Thread #%d, i = %d\n", tid, i); 
    /* Only master thread does this */ 
    if (tid == 0 && i == 1) 
    { 
      int nthreads = omp_get_num_threads(); 
      printf("Number of threads = %d\n", nthreads); 
    } 
  } /* All threads join master thread and terminate */ 
  return 0;
} 
