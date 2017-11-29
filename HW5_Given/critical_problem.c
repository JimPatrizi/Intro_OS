#include <stdio.h>
#include <omp.h> 

int main () { 
	int trial; 
	for (trial = 1; trial <= 100; trial++) {
		int i, sum = 0;
		#pragma omp parallel for
		for (i = 1; i <= 100; i++) {
			sum = sum + i;
		} /* All threads join master thread and terminate */ 
		printf("%d ", sum);
		if (sum != 5050)
			printf(" *** ERROR *** ");
	}
	printf("\n");
	return 0;
} 
