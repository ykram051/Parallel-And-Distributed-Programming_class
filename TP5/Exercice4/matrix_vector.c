/*
 * Exercise 4: Matrix Vector Product (MPI Version)
 * 
 * 1. Implements the MPI version of matrix-vector multiplication.
 * 2. Uses MPI_Wtime to measure time for speedup/efficiency calculation.
 * 3. Handles cases when N is not divisible by the number of processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Serial matrix-vector multiplication for verification
void matrixVectorMult(double* A, double* b, double* x, int size) {
    for (int i = 0; i < size; ++i) {
        x[i] = 0.0;
        for (int j = 0; j < size; ++j) {
            x[i] += A[i * size + j] * b[j];
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, num_procs;
    int size;
    double *A = NULL, *b = NULL;
    double *local_A = NULL;
    double *x_parallel = NULL, *x_serial = NULL;
    double *local_x = NULL;
    int *sendcounts = NULL, *displs = NULL;
    int *recvcounts = NULL, *recvdispls = NULL;
    double start_time, end_time, parallel_time, serial_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    // Parse command line arguments
    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <matrix_size>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }
    
    size = atoi(argv[1]);
    if (size <= 0) {
        if (rank == 0) {
            printf("Matrix size must be positive.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    // Calculate row distribution (handle non-divisible case)
    int base_rows = size / num_procs;
    int remainder = size % num_procs;
    
    // Each process gets base_rows, first 'remainder' processes get one extra
    int local_rows = base_rows + (rank < remainder ? 1 : 0);
    
    // Allocate arrays for scatter/gather
    sendcounts = (int*)malloc(num_procs * sizeof(int));
    displs = (int*)malloc(num_procs * sizeof(int));
    recvcounts = (int*)malloc(num_procs * sizeof(int));
    recvdispls = (int*)malloc(num_procs * sizeof(int));
    
    // Calculate sendcounts and displacements for scattering matrix rows
    int offset = 0;
    for (int i = 0; i < num_procs; i++) {
        int rows_for_proc = base_rows + (i < remainder ? 1 : 0);
        sendcounts[i] = rows_for_proc * size;  // Number of elements (rows * cols)
        recvcounts[i] = rows_for_proc;          // Number of result elements
        displs[i] = offset * size;
        recvdispls[i] = offset;
        offset += rows_for_proc;
    }
    
    // Allocate local arrays
    local_A = (double*)malloc(local_rows * size * sizeof(double));
    local_x = (double*)malloc(local_rows * sizeof(double));
    b = (double*)malloc(size * sizeof(double));
    
    if (!local_A || !local_x || !b) {
        printf("Process %d: Memory allocation failed.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Root process initializes data
    if (rank == 0) {
        A = (double*)malloc(size * size * sizeof(double));
        x_parallel = (double*)malloc(size * sizeof(double));
        x_serial = (double*)malloc(size * sizeof(double));
        
        if (!A || !x_parallel || !x_serial) {
            printf("Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        // Initialize with deterministic random values
        srand48(42);
        
        // Initialize matrix A (similar to given code)
        for (int i = 0; i < size * size; i++) {
            A[i] = 0.0;
        }
        
        // Fill A[0][:100] with random values
        int limit = (size < 100) ? size : 100;
        for (int j = 0; j < limit; ++j)
            A[0 * size + j] = drand48();
        
        // Copy A[0][:100] into A[1][100:200] if possible
        if (size > 1 && size > 100) {
            int copy_len = (size - 100 < 100) ? (size - 100) : 100;
            for (int j = 0; j < copy_len; ++j)
                A[1 * size + (100 + j)] = A[0 * size + j];
        }
        
        // Set diagonal
        for (int i = 0; i < size; ++i)
            A[i * size + i] = drand48();
        
        // Fill vector b
        for (int i = 0; i < size; ++i)
            b[i] = drand48();
        
        // Compute serial result for verification
        start_time = MPI_Wtime();
        matrixVectorMult(A, b, x_serial, size);
        end_time = MPI_Wtime();
        serial_time = end_time - start_time;
    }
    
    // Synchronize before parallel timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    
    // Broadcast vector b to all processes
    MPI_Bcast(b, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Scatter matrix rows to all processes
    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, local_rows * size, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);
    
    // Each process computes its local portion of the result
    for (int i = 0; i < local_rows; i++) {
        local_x[i] = 0.0;
        for (int j = 0; j < size; j++) {
            local_x[i] += local_A[i * size + j] * b[j];
        }
    }
    
    // Gather results back to root
    MPI_Gatherv(local_x, local_rows, MPI_DOUBLE,
                x_parallel, recvcounts, recvdispls, MPI_DOUBLE,
                0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    parallel_time = end_time - start_time;
    
    // Root process verifies and reports results
    if (rank == 0) {
        // Compare parallel and serial results
        double max_error = 0.0;
        for (int i = 0; i < size; ++i) {
            double diff = fabs(x_parallel[i] - x_serial[i]);
            if (diff > max_error)
                max_error = diff;
        }
        
        printf("Matrix size: %d x %d\n", size, size);
        printf("Number of processes: %d\n", num_procs);
        printf("Serial time: %f seconds\n", serial_time);
        printf("Parallel time: %f seconds\n", parallel_time);
        printf("Speedup: %f\n", serial_time / parallel_time);
        printf("Efficiency: %f%%\n", (serial_time / parallel_time / num_procs) * 100);
        printf("Maximum difference between parallel and serial result: %e\n", max_error);
        
        if (max_error < 1e-10) {
            printf("VERIFICATION: PASSED\n");
        } else {
            printf("VERIFICATION: FAILED (error too large)\n");
        }
        
        free(A);
        free(x_parallel);
        free(x_serial);
    }
    
    // Clean up
    free(local_A);
    free(local_x);
    free(b);
    free(sendcounts);
    free(displs);
    free(recvcounts);
    free(recvdispls);
    
    MPI_Finalize();
    return 0;
}

/*
 * Compilation: mpicc -o matrix_vector matrix_vector.c -lm
 * Execution:   mpirun -np 4 ./matrix_vector 1000
 * 
 * On Windows with MS-MPI:
 * Compilation: cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" matrix_vector.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
 * Execution:   mpiexec -n 4 matrix_vector.exe 1000
 * 
 * To plot Speedup/Efficiency, run with different number of processes and matrix sizes:
 * mpirun -np 1 ./matrix_vector 1000
 * mpirun -np 2 ./matrix_vector 1000
 * mpirun -np 4 ./matrix_vector 1000
 * mpirun -np 8 ./matrix_vector 1000
 */
