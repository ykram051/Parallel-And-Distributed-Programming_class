/*
 * Exercise 5: Pi Calculation using MPI
 * 
 * An approximation of π is given by:
 * π = (4/N) * Σ(1 / (1 + x_i^2)), where x_i = (i + 0.5) / N for i = 0 to N-1
 * 
 * 1. Implements the parallel version of Pi calculation using MPI.
 * 2. Handles cases when N is not divisible by the number of processes.
 * 3. Uses MPI_Wtime to measure time for speedup/efficiency calculation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Serial Pi calculation
double calculate_pi_serial(long long N) {
    double sum = 0.0;
    double h = 1.0 / (double)N;
    
    for (long long i = 0; i < N; i++) {
        double x = (i + 0.5) * h;
        sum += 1.0 / (1.0 + x * x);
    }
    
    return 4.0 * h * sum;
}

int main(int argc, char* argv[]) {
    int rank, num_procs;
    long long N;
    double local_sum = 0.0;
    double global_sum = 0.0;
    double pi_parallel, pi_serial;
    double start_time, end_time, parallel_time, serial_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    // Parse command line argument
    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <N>\n", argv[0]);
            printf("  N: Number of intervals (larger N = more accurate π)\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    N = atoll(argv[1]);
    if (N <= 0) {
        if (rank == 0) {
            printf("N must be positive.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    double h = 1.0 / (double)N;
    
    // Calculate work distribution (handle non-divisible case)
    long long base_work = N / num_procs;
    long long remainder = N % num_procs;
    
    // Each process gets base_work iterations, first 'remainder' processes get one extra
    long long local_start, local_end;
    
    if (rank < remainder) {
        // Processes 0 to remainder-1 get one extra iteration
        local_start = rank * (base_work + 1);
        local_end = local_start + base_work + 1;
    } else {
        // Remaining processes get base_work iterations
        local_start = remainder * (base_work + 1) + (rank - remainder) * base_work;
        local_end = local_start + base_work;
    }
    
    // Compute serial result on rank 0 for verification
    if (rank == 0) {
        start_time = MPI_Wtime();
        pi_serial = calculate_pi_serial(N);
        end_time = MPI_Wtime();
        serial_time = end_time - start_time;
    }
    
    // Synchronize before parallel timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    
    // Each process computes its local sum
    for (long long i = local_start; i < local_end; i++) {
        double x = (i + 0.5) * h;
        local_sum += 1.0 / (1.0 + x * x);
    }
    
    // Reduce all local sums to global sum on rank 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    parallel_time = end_time - start_time;
    
    // Root process computes final pi and reports results
    if (rank == 0) {
        pi_parallel = 4.0 * h * global_sum;
        
        printf("=========================================\n");
        printf("Pi Calculation Results\n");
        printf("=========================================\n");
        printf("N (intervals): %lld\n", N);
        printf("Number of processes: %d\n", num_procs);
        printf("\n");
        printf("Calculated Pi (parallel): %.15f\n", pi_parallel);
        printf("Calculated Pi (serial):   %.15f\n", pi_serial);
        printf("Reference Pi (M_PI):      %.15f\n", M_PI);
        printf("\n");
        printf("Error (parallel vs M_PI): %e\n", fabs(pi_parallel - M_PI));
        printf("Error (serial vs M_PI):   %e\n", fabs(pi_serial - M_PI));
        printf("Difference (parallel - serial): %e\n", fabs(pi_parallel - pi_serial));
        printf("\n");
        printf("Serial time:   %f seconds\n", serial_time);
        printf("Parallel time: %f seconds\n", parallel_time);
        printf("Speedup:       %.2f\n", serial_time / parallel_time);
        printf("Efficiency:    %.2f%%\n", (serial_time / parallel_time / num_procs) * 100);
        printf("=========================================\n");
    }
    
    MPI_Finalize();
    return 0;
}

/*
 * Compilation: mpicc -o pi_calculation pi_calculation.c -lm
 * Execution:   mpirun -np 4 ./pi_calculation 1000000000
 * 
 * On Windows with MS-MPI:
 * Compilation: cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" pi_calculation.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
 * Execution:   mpiexec -n 4 pi_calculation.exe 1000000000
 * 
 * To plot Speedup/Efficiency, run with different number of processes:
 * mpirun -np 1 ./pi_calculation 10000000000
 * mpirun -np 2 ./pi_calculation 1000000000
 * mpirun -np 4 ./pi_calculation 1000000000
 * mpirun -np 8 ./pi_calculation 1000000000
 * 
 * Expected output (example with N=1000000):
 * =========================================
 * Pi Calculation Results
 * =========================================
 * N (intervals): 1000000
 * Number of processes: 4
 * 
 * Calculated Pi (parallel): 3.141592653589971
 * Calculated Pi (serial):   3.141592653589971
 * Reference Pi (M_PI):      3.141592653589793
 * ...
 */
