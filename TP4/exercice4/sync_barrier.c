/**
 * Exercise 4: Synchronization and Barrier Cost
 * 
 * Dense matrix-vector multiplication with different scheduling strategies:
 * - Version 1: Implicit barrier
 * - Version 2: schedule(dynamic) with nowait
 * - Version 3: schedule(static) with nowait
 * 
 * Measures: CPU time, Speedup, Efficiency, MFLOP/s
 * 
 * Compile: gcc -fopenmp -o sync_barrier sync_barrier.c
 * Run: ./sync_barrier
 * Run with specific threads: OMP_NUM_THREADS=4 ./sync_barrier
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Matrix dimensions
#define N_COLS 40000   // columns
#define M_ROWS 600     // rows

// Sequential matrix-vector multiplication
void dmvm_sequential(int n, int m, double *lhs, double *rhs, double *mat) {
    for (int c = 0; c < n; ++c) {
        int offset = m * c;
        for (int r = 0; r < m; ++r)
            lhs[r] += mat[r + offset] * rhs[c];
    }
}

// Version 1: Implicit barrier (default behavior)
void dmvm_v1_barrier(int n, int m, double *lhs, double *rhs, double *mat) {
    #pragma omp parallel for
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            lhs[r] += mat[r + c * m] * rhs[c];
        }
    }
    // Implicit barrier here
}

// Version 2: schedule(dynamic) with nowait
void dmvm_v2_dynamic_nowait(int n, int m, double *lhs, double *rhs, double *mat) {
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic) nowait
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                lhs[r] += mat[r + c * m] * rhs[c];
            }
        }
        // No implicit barrier due to nowait
    }
}

// Version 3: schedule(static) with nowait
void dmvm_v3_static_nowait(int n, int m, double *lhs, double *rhs, double *mat) {
    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                lhs[r] += mat[r + c * m] * rhs[c];
            }
        }
        // No implicit barrier due to nowait
    }
}

// Initialize arrays
void initialize(int n, int m, double *mat, double *rhs, double *lhs) {
    for (int c = 0; c < n; ++c) {
        rhs[c] = 1.0;
        for (int r = 0; r < m; ++r)
            mat[r + c * m] = 1.0;
    }
    for (int r = 0; r < m; ++r)
        lhs[r] = 0.0;
}

// Reset lhs array
void reset_lhs(int m, double *lhs) {
    for (int r = 0; r < m; ++r)
        lhs[r] = 0.0;
}

// Calculate FLOPs (2 operations per element: multiply + add)
double calculate_flops(int n, int m) {
    return 2.0 * n * m;
}

// Verify result
int verify_result(int m, double *lhs, double expected) {
    for (int r = 0; r < m; ++r) {
        if (lhs[r] != expected) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const int n = N_COLS;
    const int m = M_ROWS;
    
    double *mat = malloc(n * m * sizeof(double));
    double *rhs = malloc(n * sizeof(double));
    double *lhs = malloc(m * sizeof(double));
    
    if (!mat || !rhs || !lhs) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double flops = calculate_flops(n, m);
    double start, end;
    
    // Thread counts to test
    int thread_counts[] = {1, 2, 4, 8, 16};
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);
    
    // Storage for results
    double v1_times[16], v3_times[16];
    double v1_speedups[16], v3_speedups[16];
    double v1_efficiencies[16], v3_efficiencies[16];
    double v1_mflops[16], v3_mflops[16];

    printf("=====================================================\n");
    printf("Exercise 4: Synchronization and Barrier Cost\n");
    printf("Dense Matrix-Vector Multiplication Benchmark\n");
    printf("=====================================================\n");
    printf("Matrix size: %d rows x %d columns\n", m, n);
    printf("Total FLOPs: %.0f\n\n", flops);

    // ========================================
    // Sequential baseline
    // ========================================
    printf("=== Sequential Baseline ===\n");
    initialize(n, m, mat, rhs, lhs);
    
    start = omp_get_wtime();
    dmvm_sequential(n, m, lhs, rhs, mat);
    end = omp_get_wtime();
    
    double seq_time = end - start;
    printf("Sequential time: %f seconds\n", seq_time);
    printf("MFLOP/s: %.2f\n\n", flops / (seq_time * 1e6));
    
    // Verify result
    double expected = (double)n;  // Each lhs[r] should equal n (sum of 1.0 * 1.0, n times)
    if (verify_result(m, lhs, expected)) {
        printf("Result verification: PASSED\n\n");
    } else {
        printf("Result verification: FAILED\n\n");
    }

    // ========================================
    // Run benchmarks for different thread counts
    // ========================================
    printf("=== Parallel Benchmarks ===\n");
    printf("%-8s %-12s %-12s %-10s %-12s %-12s\n", 
           "Threads", "Version", "Time (s)", "Speedup", "Efficiency", "MFLOP/s");
    printf("------------------------------------------------------------------------\n");

    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);
        
        // Version 1: Implicit barrier
        initialize(n, m, mat, rhs, lhs);
        start = omp_get_wtime();
        dmvm_v1_barrier(n, m, lhs, rhs, mat);
        end = omp_get_wtime();
        
        v1_times[t] = end - start;
        v1_speedups[t] = seq_time / v1_times[t];
        v1_efficiencies[t] = v1_speedups[t] / num_threads;
        v1_mflops[t] = flops / (v1_times[t] * 1e6);
        
        printf("%-8d %-12s %-12.6f %-10.2f %-12.4f %-12.2f\n",
               num_threads, "V1 (barrier)", v1_times[t], 
               v1_speedups[t], v1_efficiencies[t], v1_mflops[t]);
        
        // Version 3: Static with nowait
        initialize(n, m, mat, rhs, lhs);
        start = omp_get_wtime();
        dmvm_v3_static_nowait(n, m, lhs, rhs, mat);
        end = omp_get_wtime();
        
        v3_times[t] = end - start;
        v3_speedups[t] = seq_time / v3_times[t];
        v3_efficiencies[t] = v3_speedups[t] / num_threads;
        v3_mflops[t] = flops / (v3_times[t] * 1e6);
        
        printf("%-8d %-12s %-12.6f %-10.2f %-12.4f %-12.2f\n",
               num_threads, "V3 (static)", v3_times[t], 
               v3_speedups[t], v3_efficiencies[t], v3_mflops[t]);
        printf("\n");
    }

    // ========================================
    // Analysis
    // ========================================
    printf("=====================================================\n");
    printf("ANALYSIS\n");
    printf("=====================================================\n\n");
    
    printf("Why barriers limit scalability:\n");
    printf("-------------------------------\n");
    printf("1. Barriers force all threads to wait for the slowest thread\n");
    printf("2. Synchronization overhead increases with thread count\n");
    printf("3. Load imbalance amplifies the problem\n");
    printf("4. Memory bandwidth contention at barriers\n\n");
    
    printf("When nowait becomes dangerous:\n");
    printf("------------------------------\n");
    printf("1. Data races: When threads read/write shared data without sync\n");
    printf("2. Incorrect results: If computation depends on previous results\n");
    printf("3. Memory consistency: Without barriers, ordering not guaranteed\n");
    printf("4. When subsequent code depends on loop completion\n\n");
    
    printf("In this example, nowait is SAFE because:\n");
    printf("- Each row is independent (no data dependencies)\n");
    printf("- Each thread writes to different lhs[r] elements\n");
    printf("- No subsequent code depends on the parallel region results\n\n");

    // Print data for plotting
    printf("=====================================================\n");
    printf("DATA FOR PLOTTING\n");
    printf("=====================================================\n\n");
    
    printf("# CPU Time (seconds)\n");
    printf("Threads\tV1_barrier\tV3_static\n");
    for (int t = 0; t < num_tests; t++) {
        printf("%d\t%f\t%f\n", thread_counts[t], v1_times[t], v3_times[t]);
    }
    
    printf("\n# Speedup\n");
    printf("Threads\tV1_barrier\tV3_static\tIdeal\n");
    for (int t = 0; t < num_tests; t++) {
        printf("%d\t%.2f\t%.2f\t%d\n", thread_counts[t], 
               v1_speedups[t], v3_speedups[t], thread_counts[t]);
    }
    
    printf("\n# Efficiency\n");
    printf("Threads\tV1_barrier\tV3_static\n");
    for (int t = 0; t < num_tests; t++) {
        printf("%d\t%.4f\t%.4f\n", thread_counts[t], 
               v1_efficiencies[t], v3_efficiencies[t]);
    }
    
    printf("\n# MFLOP/s\n");
    printf("Threads\tV1_barrier\tV3_static\n");
    for (int t = 0; t < num_tests; t++) {
        printf("%d\t%.2f\t%.2f\n", thread_counts[t], v1_mflops[t], v3_mflops[t]);
    }

    free(mat);
    free(rhs);
    free(lhs);
    return 0;
}
