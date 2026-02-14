#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int m = 1000, n = 1000; // Example sizes, adjust as needed
    double *a = (double *)malloc(m * n * sizeof(double));
    double *b = (double *)malloc(n * m * sizeof(double));
    double *c = (double *)malloc(m * m * sizeof(double));

    // Parse schedule type and chunk size from command line
    omp_sched_t sched = omp_sched_static;
    int chunk = 1;
    if (argc > 1) {
        if (strcmp(argv[1], "static") == 0) sched = omp_sched_static;
        else if (strcmp(argv[1], "dynamic") == 0) sched = omp_sched_dynamic;
        else if (strcmp(argv[1], "guided") == 0) sched = omp_sched_guided;
    }
    if (argc > 2) {
        chunk = atoi(argv[2]);
        if (chunk < 1) chunk = 1;
    }
    omp_set_schedule(sched, chunk);

    // Initialize matrices
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = (i + 1) + (j + 1);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            b[i * m + j] = (i + 1) - (j + 1);
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            c[i * m + j] = 0;
        }
    }

    double start = omp_get_wtime();
    // Parallel matrix multiplication with collapse and runtime schedule
    #pragma omp parallel for collapse(2) schedule(runtime)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < n; k++) {
                c[i * m + j] += a[i * n + k] * b[k * m + j];
            }
        }
    }
    double end = omp_get_wtime();
    printf("Schedule: %s, Chunk: %d, Threads: %d, Time taken: %f seconds\n", argc>1?argv[1]:"static", chunk, omp_get_max_threads(), end - start);

    // Optionally print a small part of the result for validation
    printf("c[0]=%f, c[m*m-1]=%f\n", c[0], c[m*m-1]);

    free(a); free(b); free(c);
    return 0;
}
