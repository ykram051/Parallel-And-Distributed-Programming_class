#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000
void generate_noise(double *noise) {
  clock_t s = clock();
  noise[0] = 1.0;
  for (int i = 1; i < N; i++) {
    noise[i] = noise[i-1] * 1.0000001;
  }
  clock_t e = clock();
  printf("generate_noise time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
void init_matrix(double *A) {
  clock_t s = clock();
  for (int i = 0; i < N*N; i++) {
    A[i] = i * 0.1;
  }
  clock_t e = clock();
  printf("init_matrix time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
void matmul(double *A, double *B, double *C, double *noise) {
  clock_t s = clock();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      double sum = noise[i];
      for (int k = 0; k < N; k++) {
        sum += A[i*N + k] * B[k*N + j];
      }
      C[i*N + j] = sum;
    }
  }
  clock_t e = clock();
  printf("matmul time: %f s\n", (double)(e - s) / CLOCKS_PER_SEC);
}
int main() {
  clock_t start, end;
  start = clock();
  double *A = malloc(N*N * sizeof(double));
  double *B = malloc(N*N * sizeof(double));
  double *C = malloc(N*N * sizeof(double));
  double *noise = malloc(N * sizeof(double));
  generate_noise(noise);
  init_matrix(A);
  init_matrix(B);
  matmul(A, B, C, noise);
  end = clock();
  printf("C[0]=%f, time = %f s\n", C[0], (double)(end - start) / CLOCKS_PER_SEC);
  free(A); free(B); free(C); free(noise);
  return 0;
}