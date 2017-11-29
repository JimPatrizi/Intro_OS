# include <stdlib.h>
# include <stdio.h>
# include <math.h>

double shubert(double x1, double x2);

int main (){
  double x1, x2, y, min = 0;

  for (x1 = -2; x1 <= 2; x1 += 0.5) {
    for (x2 = -2; x2 <= 2; x2 += 0.5) {
      y = shubert(x1, x2);
      printf("%.2f ", y);
      if (y < min)
        min = y;
    }
    printf("\n", y);
  }
  printf("min = %.2f ", min);
}

double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}