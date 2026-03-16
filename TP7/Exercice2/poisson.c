/*
 * Exercise 2: 2D Poisson Equation with MPI Cartesian Topology
 *
 * Parallel Jacobi solver using 2D domain decomposition.
 * - MPI_Dims_create + MPI_Cart_create for 2D process grid
 * - MPI_Cart_shift for N/S/E/W neighbors
 * - Ghost exchange for rows (contiguous) and columns (MPI_Type_vector)
 * - Convergence check with MPI_Allreduce
 * - Uses compute.c initialization and Jacobi functions
 *
 * Compile: mpicc -o poisson poisson.c -lm
 * Run:     mpirun -np 4 ./poisson [ntx] [nty]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

/* ================================================================
 * Global variables (used by compute functions from compute.c)
 * ================================================================ */
static int ntx, nty;
static int sx, ex, sy, ey;

static double *f;
static double coef[3];

#define IDX(i, j) (((i) - (sx - 1)) * (ey - sy + 3) + (j) - (sy - 1))

/* ================================================================
 * Initialization (from compute.c)
 * ================================================================ */
static void initialization(double **pu, double **pu_new, double **pu_exact) {
    double hx, hy;
    int iterx, itery;
    double x, y;
    int local_size = (ex - sx + 3) * (ey - sy + 3);

    *pu       = calloc(local_size, sizeof(double));
    *pu_new   = calloc(local_size, sizeof(double));
    *pu_exact = calloc(local_size, sizeof(double));
    f         = calloc(local_size, sizeof(double));

    hx = 1.0 / (ntx + 1.0);
    hy = 1.0 / (nty + 1.0);

    coef[0] = (0.5 * hx * hx * hy * hy) / (hx * hx + hy * hy);
    coef[1] = 1.0 / (hx * hx);
    coef[2] = 1.0 / (hy * hy);

    for (iterx = sx; iterx <= ex; iterx++) {
        for (itery = sy; itery <= ey; itery++) {
            x = iterx * hx;
            y = itery * hy;
            f[IDX(iterx, itery)] = 2.0 * (x * x - x + y * y - y);
            (*pu_exact)[IDX(iterx, itery)] = x * y * (x - 1.0) * (y - 1.0);
        }
    }
}

/* ================================================================
 * Jacobi iteration (from compute.c)
 * ================================================================ */
static void compute(double *u, double *u_new) {
    int iterx, itery;

    for (iterx = sx; iterx <= ex; iterx++) {
        for (itery = sy; itery <= ey; itery++) {
            u_new[IDX(iterx, itery)] =
                coef[0] * (
                    coef[1] * (u[IDX(iterx + 1, itery)] + u[IDX(iterx - 1, itery)]) +
                    coef[2] * (u[IDX(iterx, itery + 1)] + u[IDX(iterx, itery - 1)]) -
                    f[IDX(iterx, itery)]
                );
        }
    }
}

/* ================================================================
 * Output results (from compute.c)
 * ================================================================ */
static void output_results(double *u, double *u_exact) {
    int itery;

    printf("Exact solution u_exact - Computed solution u\n");
    for (itery = sy; itery <= ey; itery++)
        printf("%12.5e - %12.5e\n", u_exact[IDX(1, itery)], u[IDX(1, itery)]);
}

/* ================================================================
 * Compute local squared error between successive iterations
 * ================================================================ */
static double local_diff_squared(double *u, double *u_new) {
    double err = 0.0, diff;
    int iterx, itery;

    for (iterx = sx; iterx <= ex; iterx++) {
        for (itery = sy; itery <= ey; itery++) {
            diff = u_new[IDX(iterx, itery)] - u[IDX(iterx, itery)];
            err += diff * diff;
        }
    }
    return err;
}

/* ================================================================
 * Main
 * ================================================================ */
int main(int argc, char *argv[]) {
    int rank, nprocs;
    double *u, *u_new, *u_exact, *tmp;
    double local_err, global_err;
    double t_start, t_end;
    int iter;
    double tolerance = 1.0e-6;
    int max_iter = 10000;

    /* 2D Cartesian topology variables */
    int dims[2] = {0, 0};
    int periods[2] = {0, 0};  /* Non-periodic for Poisson */
    int coords[2];
    MPI_Comm cart_comm;
    int north, south, east, west;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* Parse arguments */
    ntx = (argc > 1) ? atoi(argv[1]) : 12;
    nty = (argc > 2) ? atoi(argv[2]) : 10;

    /* ---- Step 1: Create 2D process grid ---- */
    MPI_Dims_create(nprocs, 2, dims);

    /* ---- Step 2: Create 2D Cartesian communicator ---- */
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &rank);

    /* ---- Step 3: Get 2D coordinates ---- */
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    /* ---- Step 4: 2D domain decomposition ---- */
    {
        /* Decompose along x (dim 0) */
        int nx_per_proc = ntx / dims[0];
        int rx = ntx % dims[0];
        if (coords[0] < rx) {
            sx = coords[0] * (nx_per_proc + 1) + 1;
            ex = sx + nx_per_proc;
        } else {
            sx = rx * (nx_per_proc + 1) + (coords[0] - rx) * nx_per_proc + 1;
            ex = sx + nx_per_proc - 1;
        }

        /* Decompose along y (dim 1) */
        int ny_per_proc = nty / dims[1];
        int ry = nty % dims[1];
        if (coords[1] < ry) {
            sy = coords[1] * (ny_per_proc + 1) + 1;
            ey = sy + ny_per_proc;
        } else {
            sy = ry * (ny_per_proc + 1) + (coords[1] - ry) * ny_per_proc + 1;
            ey = sy + ny_per_proc - 1;
        }
    }

    /* ---- Step 5: Identify neighbors ---- */
    MPI_Cart_shift(cart_comm, 0, 1, &north, &south);  /* dim 0 = x */
    MPI_Cart_shift(cart_comm, 1, 1, &west, &east);    /* dim 1 = y */

    /* Print topology info */
    if (rank == 0) {
        printf("Poisson execution with %d MPI processes\n", nprocs);
        printf("Domain size: ntx=%d nty=%d\n", ntx, nty);
        printf("Topology dimensions: %d along x, %d along y\n", dims[0], dims[1]);
        printf("-----------------------------------------\n");
    }
    MPI_Barrier(cart_comm);

    /* Each process prints its info sequentially */
    int p;
    for (p = 0; p < nprocs; p++) {
        if (rank == p) {
            printf("Rank in the topology: %d   Array indices: x from %d to %d, y from %d to %d\n",
                   rank, sx, ex, sy, ey);
            printf("Process %d has neighbors: N %d E %d S %d W %d\n",
                   rank, north, east, south, west);
        }
        fflush(stdout);
        MPI_Barrier(cart_comm);
    }

    /* ---- Step 6: Initialize arrays ---- */
    initialization(&u, &u_new, &u_exact);

    /* Create MPI derived type for column exchange */
    int nrows = ex - sx + 3;   /* local rows including ghost */
    int ncols = ey - sy + 3;   /* local cols including ghost */
    MPI_Datatype col_type;
    MPI_Type_vector(nrows, 1, ncols, MPI_DOUBLE, &col_type);
    MPI_Type_commit(&col_type);

    /* ---- Step 7: Iterative Jacobi solver ---- */
    MPI_Barrier(cart_comm);
    t_start = MPI_Wtime();

    for (iter = 1; iter <= max_iter; iter++) {
        MPI_Status status;

        /* Exchange N/S ghost rows (contiguous, ncols elements) */
        MPI_Sendrecv(&u[IDX(ex, sy - 1)], ncols, MPI_DOUBLE, south, 0,
                     &u[IDX(sx - 1, sy - 1)], ncols, MPI_DOUBLE, north, 0,
                     cart_comm, &status);
        MPI_Sendrecv(&u[IDX(sx, sy - 1)], ncols, MPI_DOUBLE, north, 1,
                     &u[IDX(ex + 1, sy - 1)], ncols, MPI_DOUBLE, south, 1,
                     cart_comm, &status);

        /* Exchange E/W ghost columns (non-contiguous, need col_type) */
        MPI_Sendrecv(&u[IDX(sx - 1, ey)], 1, col_type, east, 2,
                     &u[IDX(sx - 1, sy - 1)], 1, col_type, west, 2,
                     cart_comm, &status);
        MPI_Sendrecv(&u[IDX(sx - 1, sy)], 1, col_type, west, 3,
                     &u[IDX(sx - 1, ey + 1)], 1, col_type, east, 3,
                     cart_comm, &status);

        /* Compute u_new from u */
        compute(u, u_new);

        /* Compute convergence: global error = sqrt(sum(u_new-u)^2) */
        local_err = local_diff_squared(u, u_new);
        MPI_Allreduce(&local_err, &global_err, 1, MPI_DOUBLE, MPI_SUM, cart_comm);
        global_err = sqrt(global_err);

        /* Print progress every 100 iterations */
        if (rank == 0 && iter % 100 == 0)
            printf("Iteration %d global_error = %e\n", iter, global_err);

        /* Swap u and u_new */
        tmp   = u;
        u     = u_new;
        u_new = tmp;

        /* Check convergence */
        if (global_err < tolerance) {
            if (rank == 0)
                printf("Converged after %d iterations (error = %e < %e)\n",
                       iter, global_err, tolerance);
            break;
        }
    }

    MPI_Barrier(cart_comm);
    t_end = MPI_Wtime();

    if (rank == 0)
        printf("Time: %.4f s\n\n", t_end - t_start);

    /* ---- Step 8: Output results ---- */
    /* Process that owns x=1 prints comparison (rank with coords[0]=0, coords[1]=0) */
    if (sx <= 1 && ex >= 1 && coords[1] == 0)
        output_results(u, u_exact);

    /* Cleanup */
    MPI_Type_free(&col_type);
    free(u);
    free(u_new);
    free(u_exact);
    free(f);
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
