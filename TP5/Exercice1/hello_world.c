/*
 * Exercise 1: Hello World - MPI Basic Program
 * 
 * 1. Prints "Hello World" from all processes
 * 2. Each process prints its rank and total number of processes
 * 3. Only rank 0 prints a message
 * 4. Note: Omitting MPI_Finalize may cause resource leaks or hanging processes
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Part 1: All processes print "Hello World"
    printf("Hello World from process %d\n", rank);
    
    // Part 2: Each process prints its rank and total number of processes
    printf("I am process %d out of %d processes\n", rank, size);
    
    // Part 3: Only rank 0 prints a message
    if (rank == 0) {
        printf("\n[Only Rank 0]: This message is printed only by the master process.\n");
        printf("[Only Rank 0]: Total number of processes: %d\n", size);
    }
    
    // Part 4: What happens if MPI_Finalize is omitted?
    // Answer: Omitting MPI_Finalize() can cause:
    // - Resource leaks (memory, communication buffers)
    // - Processes may hang waiting for others
    // - The MPI runtime may not clean up properly
    // - Some MPI implementations may report errors
    // - The program might not terminate cleanly
    
    // Finalize MPI
    MPI_Finalize();
    
    return 0;
}

/*
 * Compilation: mpicc -o hello_world hello_world.c
 * Execution:   mpirun -np 4 ./hello_world
 * 
 * On Windows with MS-MPI:
 * Compilation: cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" hello_world.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
 * Execution:   mpiexec -n 4 hello_world.exe
 */
