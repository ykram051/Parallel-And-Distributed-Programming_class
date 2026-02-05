#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 100000000
void add_noise(double *a) {
  clock_t s = clock();
  a[0] = 1.0;
  for (int i = 1; i < N; i++) {
    a[i] = a[i-1] * 1.0000001;
  }
  clock_t e = clock();
  printf("add_noise time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
void init_b(double *b) {
  clock_t s = clock();
  for (int i = 0; i < N; i++) {
    b[i] = i * 0.5;
  }
  clock_t e = clock();
  printf("init_b time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
void compute_addition(double *a, double *b, double *c) {
  clock_t s = clock();
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }
  clock_t e = clock();
  printf("compute_addition time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
double reduction(double *c) {
  clock_t s = clock();
  double sum = 0.0;
  for (int i = 0; i < N; i++) {
    sum += c[i];
  }
  clock_t e = clock();
  printf("reduction time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
  return sum;
}
int main() {
  clock_t start, end;
  start = clock();
  double *a = malloc(N * sizeof(double));
  double *b = malloc(N * sizeof(double));
  double *c = malloc(N * sizeof(double));
  add_noise(a);
  init_b(b);
  compute_addition(a, b, c);
  double sum = reduction(c);
  end = clock();
  printf("Sum = %f, time = %f s\n", sum, (double)(end - start) / CLOCKS_PER_SEC);
  free(a); free(b); free(c);
  return 0;
}