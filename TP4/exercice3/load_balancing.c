/**
 * Exercise 3: Load Balancing with Parallel Sections
 * 
 * - Implement a task scheduling mechanism using parallel sections
 * - Simulate three different workloads:
 *   - Task A (light computation)
 *   - Task B (moderate computation)
 *   - Task C (heavy computation)
 * - Measure the execution time and optimize the workload distribution
 * 
 * Compile: gcc -fopenmp -o load_balancing load_balancing.c -lm
 * Run: ./load_balancing
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

void task_light(int n) {
    double x = 0.0;
    for (int i = 0; i < n; i++) {
        x += sin(i * 0.001);
    }
    // Prevent optimization
    if (x > 1e30) printf("%f", x);
}

void task_moderate(int n) {
    double x = 0.0;
    for (int i = 0; i < 5 * n; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001);
    }
    // Prevent optimization
    if (x > 1e30) printf("%f", x);
}

void task_heavy(int n) {
    double x = 0.0;
    for (int i = 0; i < 20 * n; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
    // Prevent optimization
    if (x > 1e30) printf("%f", x);
}

// Split heavy task for better load balancing
void task_heavy_part(int n, int part, int total_parts) {
    double x = 0.0;
    int total = 20 * n;
    int chunk = total / total_parts;
    int start = part * chunk;
    int end = (part == total_parts - 1) ? total : (part + 1) * chunk;
    
    for (int i = start; i < end; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
    // Prevent optimization
    if (x > 1e30) printf("%f", x);
}

int main() {
    double start, end;
    int n = N / 10;  // Use smaller N for reasonable execution time

    printf("==============================================\n");
    printf("Exercise 3: Load Balancing with Parallel Sections\n");
    printf("==============================================\n");
    printf("Workload sizes:\n");
    printf("  Light task:    %d iterations\n", n);
    printf("  Moderate task: %d iterations\n", 5 * n);
    printf("  Heavy task:    %d iterations\n", 20 * n);
    printf("\n");

    // ========================================
    // Sequential Execution
    // ========================================
    printf("=== Sequential Execution ===\n");
    
    start = omp_get_wtime();
    task_light(n);
    task_moderate(n);
    task_heavy(n);
    end = omp_get_wtime();

    double seq_time = end - start;
    printf("Total time: %f seconds\n\n", seq_time);

    // ========================================
    // Parallel Sections (Basic - Unbalanced)
    // ========================================
    printf("=== Parallel Sections (Basic - Unbalanced) ===\n");
    
    double task_times[3];
    
    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_light(n);
            double t2 = omp_get_wtime();
            task_times[0] = t2 - t1;
            printf("Task A (light) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), task_times[0]);
        }

        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_moderate(n);
            double t2 = omp_get_wtime();
            task_times[1] = t2 - t1;
            printf("Task B (moderate) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), task_times[1]);
        }

        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_heavy(n);
            double t2 = omp_get_wtime();
            task_times[2] = t2 - t1;
            printf("Task C (heavy) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), task_times[2]);
        }
    }

    end = omp_get_wtime();
    double par_basic_time = end - start;
    
    printf("Total time: %f seconds\n", par_basic_time);
    printf("Speedup: %.2fx\n", seq_time / par_basic_time);
    printf("Load imbalance: Heavy task takes %.1fx longer than light task\n\n", 
           task_times[2] / task_times[0]);

    // ========================================
    // Parallel Sections (Optimized - Better Balance)
    // ========================================
    printf("=== Parallel Sections (Optimized - Better Balance) ===\n");
    printf("Strategy: Split heavy task into 4 parts\n");
    
    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_light(n);
            task_moderate(n);  // Combine light + moderate
            double t2 = omp_get_wtime();
            printf("Tasks A+B (light+moderate) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), t2 - t1);
        }

        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_heavy_part(n, 0, 2);  // First half of heavy
            double t2 = omp_get_wtime();
            printf("Task C (heavy part 1/2) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), t2 - t1);
        }

        #pragma omp section
        {
            double t1 = omp_get_wtime();
            task_heavy_part(n, 1, 2);  // Second half of heavy
            double t2 = omp_get_wtime();
            printf("Task C (heavy part 2/2) - Thread %d, Time: %f s\n", 
                   omp_get_thread_num(), t2 - t1);
        }
    }

    end = omp_get_wtime();
    double par_opt_time = end - start;
    
    printf("Total time: %f seconds\n", par_opt_time);
    printf("Speedup: %.2fx\n", seq_time / par_opt_time);
    printf("\n");

    // ========================================
    // Using OpenMP Tasks (Alternative approach)
    // ========================================
    printf("=== Using OpenMP Tasks (Alternative) ===\n");
    
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
                printf("Task A (light) - Thread %d\n", omp_get_thread_num());
                task_light(n);
            }

            #pragma omp task
            {
                printf("Task B (moderate) - Thread %d\n", omp_get_thread_num());
                task_moderate(n);
            }

            // Split heavy task into multiple tasks
            for (int i = 0; i < 4; i++) {
                #pragma omp task
                {
                    printf("Task C (heavy part %d/4) - Thread %d\n", 
                           i + 1, omp_get_thread_num());
                    task_heavy_part(n, i, 4);
                }
            }
        }
    }

    end = omp_get_wtime();
    double task_time = end - start;
    
    printf("Total time: %f seconds\n", task_time);
    printf("Speedup: %.2fx\n", seq_time / task_time);
    printf("\n");

    // ========================================
    // Summary
    // ========================================
    printf("==============================================\n");
    printf("Summary\n");
    printf("==============================================\n");
    printf("Sequential time:         %f seconds\n", seq_time);
    printf("Parallel (basic) time:   %f seconds (Speedup: %.2fx)\n", 
           par_basic_time, seq_time / par_basic_time);
    printf("Parallel (optimized):    %f seconds (Speedup: %.2fx)\n", 
           par_opt_time, seq_time / par_opt_time);
    printf("OpenMP tasks:            %f seconds (Speedup: %.2fx)\n", 
           task_time, seq_time / task_time);

    return 0;
}
