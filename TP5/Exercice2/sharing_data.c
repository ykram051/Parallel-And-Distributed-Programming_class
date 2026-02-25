/*
 * Exercise 2: Sharing Data
 * 
 * A program that obtains an integer input from the terminal and distributes it
 * to all the MPI processes. Each process displays its rank and the received value.
 * Continues reading values until a negative integer is entered.
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int value;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Keep reading values until negative
    while (1) {
        // Only rank 0 reads from terminal
        if (rank == 0) {
            printf("Enter an integer (negative to quit): ");
            fflush(stdout);
            scanf("%d", &value);
        }
        
        // Broadcast the value from rank 0 to all processes
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        // Check for termination condition
        if (value < 0) {
            if (rank == 0) {
                printf("Negative value entered. Exiting...\n");
            }
            break;
        }
        
        // Each process prints its rank and the received value
        printf("Process %d got %d\n", rank, value);
        fflush(stdout);
        
        // Synchronize before next iteration
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    // Finalize MPI
    MPI_Finalize();
    
    return 0;
}

/*
 * Compilation: mpicc -o sharing_data sharing_data.c
 * Execution:   mpirun -np 4 ./sharing_data
 * 
 * On Windows with MS-MPI:
 * Compilation: cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" sharing_data.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
 * Execution:   mpiexec -n 4 sharing_data.exe
 * 
 * Example Output:
 * Enter an integer (negative to quit): 10
 * Process 0 got 10
 * Process 1 got 10
 * Process 2 got 10
 * Process 3 got 10
 * Enter an integer (negative to quit): -1
 * Negative value entered. Exiting...
 */
