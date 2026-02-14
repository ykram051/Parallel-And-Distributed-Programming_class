#include <stdio.h>
#include <omp.h>

int main() {
    int num_threads = 0;

    #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        #pragma omp critical
        {
            printf("Hello from the rank %d thread\n", rank);
        }
        #pragma omp master
        {
            num_threads = omp_get_num_threads();
        }
    }
    printf("Parallel execution of hello_world with %d threads\n", num_threads);
    return 0;
}