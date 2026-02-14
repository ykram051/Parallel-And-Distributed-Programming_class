#include <stdio.h>
#include <omp.h>

static long num_steps = 100000;
double step;

int main() {
    int i;
    double x, pi, sum = 0.0;
    double start_time, end_time;
    step = 1.0 / (double) num_steps;
    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double local_sum = 0.0;
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        for (i = id; i < num_steps; i += nthreads) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        sum += local_sum;
    }

    pi = step * sum;
    end_time = omp_get_wtime();
    printf("Calculated PI = %.15f\n", pi);
    printf("Time taken: %f seconds\n", end_time - start_time);
    return 0;
}
