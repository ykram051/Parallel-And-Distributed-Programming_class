/*
 * Exercise 2: Distributed Gradient Descent with MPI Derived Types
 *
 * Compile: mpicc -o distrib_grad distrib_grad.c -lm
 * Run:     mpiexec -n 4 ./distrib_grad
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N_FEATURES 2
#define N_SAMPLES  1000
#define LR         0.01      /* Learning rate */
#define THRESHOLD  1e-2      /* Early stopping threshold */
#define MAX_EPOCHS 5000
#define PRINT_EVERY 10

typedef struct {
    double x[N_FEATURES];
    double y;
} Sample;

/* True weights used to generate data: y = 2*x0 - 1*x1 + noise */
static const double TRUE_W[N_FEATURES] = {2.0, -1.0};

/*
 * Generate synthetic dataset on process 0.
 * y = TRUE_W . x + noise
 */
void generate_data(Sample *data, int n) {
    for (int i = 0; i < n; i++) {
        double y = 0.0;
        for (int f = 0; f < N_FEATURES; f++) {
            data[i].x[f] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            y += TRUE_W[f] * data[i].x[f];
        }
        /* Add small Gaussian-like noise using Box-Muller */
        double u1 = ((double)rand() / RAND_MAX);
        double u2 = ((double)rand() / RAND_MAX);
        if (u1 < 1e-10) u1 = 1e-10;
        double noise = 0.1 * sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        data[i].y = y + noise;
    }
}

/*
 * Build an MPI derived type for the Sample struct.
 */
MPI_Datatype create_sample_type(void) {
    MPI_Datatype sample_type;
    int blocklengths[2] = {N_FEATURES, 1};
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint offsets[2];

    /* Compute offsets using offsetof-like approach */
    Sample dummy;
    MPI_Aint base_addr, x_addr, y_addr;
    MPI_Get_address(&dummy, &base_addr);
    MPI_Get_address(&dummy.x, &x_addr);
    MPI_Get_address(&dummy.y, &y_addr);

    offsets[0] = x_addr - base_addr;
    offsets[1] = y_addr - base_addr;

    MPI_Type_create_struct(2, blocklengths, offsets, types, &sample_type);

    /* Resize to match actual struct size (handles padding) */
    MPI_Datatype resized_type;
    MPI_Type_create_resized(sample_type, 0, sizeof(Sample), &resized_type);
    MPI_Type_commit(&resized_type);
    MPI_Type_free(&sample_type);

    return resized_type;
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Create MPI derived type for Sample */
    MPI_Datatype mpi_sample = create_sample_type();

    /* Compute per-process sample counts and displacements for Scatterv */
    int base_count = N_SAMPLES / size;
    int remainder  = N_SAMPLES % size;

    int *sendcounts = malloc(size * sizeof(int));
    int *displs     = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        sendcounts[i] = base_count + (i < remainder ? 1 : 0);
        displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
    }

    int local_n = sendcounts[rank];

    /* Generate full dataset on process 0 */
    Sample *all_data = NULL;
    if (rank == 0) {
        all_data = malloc(N_SAMPLES * sizeof(Sample));
        srand(42);
        generate_data(all_data, N_SAMPLES);
    }

    /* Scatter data to all processes */
    Sample *local_data = malloc(local_n * sizeof(Sample));
    MPI_Scatterv(all_data, sendcounts, displs, mpi_sample,
                 local_data, local_n, mpi_sample,
                 0, MPI_COMM_WORLD);

    if (rank == 0)
        free(all_data);

    /* Initialize weights to zero */
    double w[N_FEATURES] = {0};

    double start_time = MPI_Wtime();
    int epoch;
    int converged = 0;

    for (epoch = 1; epoch <= MAX_EPOCHS; epoch++) {
        /* Compute local gradient and local loss */
        double local_grad[N_FEATURES] = {0};
        double local_loss = 0.0;

        for (int i = 0; i < local_n; i++) {
            /* Prediction: y_hat = w . x */
            double y_hat = 0.0;
            for (int f = 0; f < N_FEATURES; f++)
                y_hat += w[f] * local_data[i].x[f];

            double error = y_hat - local_data[i].y;
            local_loss += error * error;

            for (int f = 0; f < N_FEATURES; f++)
                local_grad[f] += error * local_data[i].x[f];
        }

        /* Aggregate gradient and loss across all processes */
        double global_grad[N_FEATURES];
        double global_loss;

        MPI_Allreduce(local_grad, global_grad, N_FEATURES, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&local_loss, &global_loss, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        /* Average MSE */
        global_loss /= N_SAMPLES;

        /* Update weights: w -= lr * (1/N) * gradient */
        for (int f = 0; f < N_FEATURES; f++) {
            global_grad[f] /= N_SAMPLES;
            w[f] -= LR * global_grad[f];
        }

        /* Print every PRINT_EVERY epochs from process 0 */
        if (rank == 0 && epoch % PRINT_EVERY == 0) {
            printf("Epoch %d | Loss (MSE): %f | ", epoch, global_loss);
            for (int f = 0; f < N_FEATURES; f++) {
                printf("w[%d]: %.4f", f, w[f]);
                if (f < N_FEATURES - 1) printf(", ");
            }
            printf("\n");
        }

        /* Early stopping */
        if (global_loss < THRESHOLD) {
            if (rank == 0)
                printf("Early stopping at epoch %d — loss %f < %.1e\n",
                       epoch, global_loss, THRESHOLD);
            converged = 1;
            break;
        }
    }

    double end_time = MPI_Wtime();

    if (rank == 0) {
        if (!converged)
            printf("Reached max epochs (%d) — final loss: %f\n", MAX_EPOCHS, 0.0);
        printf("Training time: %.3f seconds (MPI)\n", end_time - start_time);
    }

    free(local_data);
    free(sendcounts);
    free(displs);
    MPI_Type_free(&mpi_sample);
    MPI_Finalize();
    return 0;
}
