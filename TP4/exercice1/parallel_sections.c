/**
 * Exercise 1: Work Distribution with Parallel Sections
 * 
 * - Initialize an array of size N with random values
 * - Use #pragma omp sections to divide the work:
 *   - Section 1: Compute the sum of all elements
 *   - Section 2: Compute the maximum value
 *   - Section 3: Compute the standard deviation (using sum from Section 1)
 * 
 * Compile: gcc -fopenmp -o parallel_sections parallel_sections.c -lm
 * Run: ./parallel_sections
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define N 1000000

int main() {
    double *A = malloc(N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
    double max;
    double start, end;

    // Initialization
    srand(0);
    for (int i = 0; i < N; i++)
        A[i] = (double)rand() / RAND_MAX;

    // ========================================
    // Sequential Version
    // ========================================
    start = omp_get_wtime();
    
    // Compute sum and max
    sum = 0.0;
    max = A[0];
    for (int i = 0; i < N; i++) {
        sum += A[i];
        if (A[i] > max)
            max = A[i];
    }

    // Compute mean
    mean = sum / N;

    // Compute standard deviation
    stddev = 0.0;
    for (int i = 0; i < N; i++)
        stddev += (A[i] - mean) * (A[i] - mean);

    stddev = sqrt(stddev / N);

    end = omp_get_wtime();

    // Print results
    printf("=== Sequential Version ===\n");
    printf("Sum     = %f\n", sum);
    printf("Max     = %f\n", max);
    printf("Std Dev = %f\n", stddev);
    printf("Time    = %f seconds\n\n", end - start);

    // ========================================
    // Parallel Version with Sections
    // ========================================
    double par_sum = 0.0;
    double par_max = A[0];
    double par_stddev = 0.0;
    double par_mean = 0.0;
    int sum_ready = 0;  // Flag to signal sum is computed

    start = omp_get_wtime();

    #pragma omp parallel shared(A, par_sum, par_max, par_stddev, par_mean, sum_ready)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                // Section 1: Compute sum
                double local_sum = 0.0;
                for (int i = 0; i < N; i++) {
                    local_sum += A[i];
                }
                #pragma omp atomic write
                par_sum = local_sum;
                
                par_mean = par_sum / N;
                
                #pragma omp atomic write
                sum_ready = 1;
                
                printf("Section 1 (Sum) executed by thread %d\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                // Section 2: Compute max
                double local_max = A[0];
                for (int i = 1; i < N; i++) {
                    if (A[i] > local_max)
                        local_max = A[i];
                }
                par_max = local_max;
                printf("Section 2 (Max) executed by thread %d\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                // Section 3: Compute standard deviation
                // Wait for sum to be ready
                int ready = 0;
                while (!ready) {
                    #pragma omp atomic read
                    ready = sum_ready;
                }
                
                double local_mean = par_sum / N;
                double local_stddev = 0.0;
                for (int i = 0; i < N; i++) {
                    local_stddev += (A[i] - local_mean) * (A[i] - local_mean);
                }
                par_stddev = sqrt(local_stddev / N);
                printf("Section 3 (StdDev) executed by thread %d\n", omp_get_thread_num());
            }
        }
    }

    end = omp_get_wtime();

    printf("\n=== Parallel Version (Sections) ===\n");
    printf("Sum     = %f\n", par_sum);
    printf("Max     = %f\n", par_max);
    printf("Std Dev = %f\n", par_stddev);
    printf("Time    = %f seconds\n", end - start);

    free(A);
    return 0;
}
