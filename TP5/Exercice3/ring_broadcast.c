/*
 * Exercise 3: Sending in a Ring (Broadcast by Ring)
 * 
 * Process 0 reads a value from the user.
 * It sends it to process 1, which adds its rank and sends it to process 2, and so on.
 * Each process receives the data, adds its rank, prints the result, and forwards it
 * to the next process.
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int value;
    int next_rank, prev_rank;
    MPI_Status status;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Calculate neighbors in the ring
    next_rank = (rank + 1) % size;
    prev_rank = (rank - 1 + size) % size;
    
    if (rank == 0) {
        // Process 0 reads the initial value
        printf("Enter a value: ");
        fflush(stdout);
        scanf("%d", &value);
        
        // Add rank 0's contribution
        value += rank;
        printf("Process %d: value after adding rank = %d\n", rank, value);
        fflush(stdout);
        
        // Send to next process (process 1)
        MPI_Send(&value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        
        // Receive the final value from the last process (completing the ring)
        MPI_Recv(&value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        printf("Process %d: final value received from ring = %d\n", rank, value);
        
    } else {
        // Receive from previous process
        MPI_Recv(&value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &status);
        
        // Add this process's rank
        value += rank;
        printf("Process %d: value after adding rank = %d\n", rank, value);
        fflush(stdout);
        
        // Send to next process
        MPI_Send(&value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }
    
    // Finalize MPI
    MPI_Finalize();
    
    return 0;
}

/*
 * Compilation: mpicc -o ring_broadcast ring_broadcast.c
 * Execution:   mpirun -np 4 ./ring_broadcast
 * 
 * On Windows with MS-MPI:
 * Compilation: cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" ring_broadcast.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
 * Execution:   mpiexec -n 4 ring_broadcast.exe
 * 
 * Example with 4 processes and initial value 10:
 * Enter a value: 10
 * Process 0: value after adding rank = 10 (10 + 0)
 * Process 1: value after adding rank = 11 (10 + 1)
 * Process 2: value after adding rank = 13 (11 + 2)
 * Process 3: value after adding rank = 16 (13 + 3)
 * Process 0: final value received from ring = 16
 * 
 * The final value is: initial_value + sum(0 to n-1) = initial_value + n*(n-1)/2
 * For n=4 and initial=10: 10 + 0 + 1 + 2 + 3 = 16
 */
