#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1024 // Matrix size (N x N)

// Function to allocate memory for a matrix
double** allocate_matrix(int size) {
    double** matrix = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (double*)malloc(size * sizeof(double));
    }
    return matrix;
}

// Function to initialize a matrix with random values
void initialize_matrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Function to perform block matrix multiplication
void block_matrix_multiplication(double** A, double** B, double** C, int size, int block_size) {
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int k = 0; k < size; k += block_size) {
                for (int ii = i; ii < i + block_size && ii < size; ii++) {
                    for (int jj = j; jj < j + block_size && jj < size; jj++) {
                        for (int kk = k; kk < k + block_size && kk < size; kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

// Function to free allocated memory
void free_matrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int block_size;
    printf("Enter block size: ");
    scanf("%d", &block_size);

    // Allocate and initialize matrices
    double** A = allocate_matrix(N);
    double** B = allocate_matrix(N);
    double** C = allocate_matrix(N);

    initialize_matrix(A, N);
    initialize_matrix(B, N);

    // Initialize matrix C to zero
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
        }
    }

    // Measure execution time
    clock_t start = clock();
    block_matrix_multiplication(A, B, C, N, block_size);
    clock_t end = clock();

    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time: %f seconds\n", cpu_time_used);

    // Free allocated memory
    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);

    return 0;
}