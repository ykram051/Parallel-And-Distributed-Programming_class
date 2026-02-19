/**
 * Exercise 2: Exclusive Execution - Master vs Single
 * 
 * - Master thread initializes a matrix
 * - Single thread prints the matrix
 * - All threads compute the sum of all elements in parallel
 * - Compare execution time with and without OpenMP
 * 
 * Compile: gcc -fopenmp -o master_single master_single.c
 * Run: ./master_single
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1000

void init_matrix(int n, double *A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = (double)(i + j);
        }
    }
}

void print_matrix(int n, double *A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6.1f ", A[i * n + j]);
        }
        printf("\n");
    }
}

double sum_matrix(int n, double *A) {
    double sum = 0.0;
    for (int i = 0; i < n * n; i++) {
        sum += A[i];
    }
    return sum;
}

int main() {
    double *A;
    double sum;
    double start, end;

    A = (double *)malloc(N * N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // ========================================
    // Sequential Version (No OpenMP)
    // ========================================
    printf("=== Sequential Version ===\n");
    
    start = omp_get_wtime();

    /* Initialization */
    init_matrix(N, A);

    /* Printing (can be commented if N is large) */
    /* print_matrix(N, A); */

    /* Sum computation */
    sum = sum_matrix(N, A);

    end = omp_get_wtime();

    printf("Sum = %lf\n", sum);
    printf("Execution time = %lf seconds\n\n", end - start);

    double seq_time = end - start;

    // ========================================
    // Parallel Version with Master and Single
    // ========================================
    printf("=== Parallel Version (Master/Single) ===\n");
    
    double par_sum = 0.0;

    start = omp_get_wtime();

    #pragma omp parallel shared(A, par_sum)
    {
        // Master thread initializes the matrix
        #pragma omp master
        {
            printf("Master thread %d initializing matrix...\n", omp_get_thread_num());
            init_matrix(N, A);
        }
        
        // Implicit barrier after master? No! master has no implicit barrier
        // We need explicit barrier to ensure initialization is complete
        #pragma omp barrier

        // Single thread prints the matrix (only for small matrices)
        #pragma omp single
        {
            printf("Single thread %d would print matrix (skipped for large N)...\n", omp_get_thread_num());
            /* if (N <= 10) print_matrix(N, A); */
        }
        // Implicit barrier after single

        // All threads compute the sum in parallel using reduction
        #pragma omp for reduction(+:par_sum)
        for (int i = 0; i < N * N; i++) {
            par_sum += A[i];
        }
    }

    end = omp_get_wtime();

    printf("Sum = %lf\n", par_sum);
    printf("Execution time = %lf seconds\n", end - start);
    printf("Number of threads used: %d\n", omp_get_max_threads());

    double par_time = end - start;

    // ========================================
    // Comparison
    // ========================================
    printf("\n=== Comparison ===\n");
    printf("Sequential time: %lf seconds\n", seq_time);
    printf("Parallel time:   %lf seconds\n", par_time);
    printf("Speedup: %.2fx\n", seq_time / par_time);

    // Verify results
    if (sum == par_sum) {
        printf("Results match!\n");
    } else {
        printf("Results do not match! Seq: %lf, Par: %lf\n", sum, par_sum);
    }

    free(A);
    return 0;
}
