/*
 * Exercise 1: Conway's Game of Life with MPI 2D Cartesian Topology
 *
 * - MPI_Cart_create to define a 2D Cartesian communicator
 * - MPI_Cart_shift to identify N/S/E/W neighbors
 * - Ghost layer exchanges with MPI_Sendrecv
 * - Periodic boundary conditions (wrap-around)
 *
 * Compile: mpicc -o game_of_life game_of_life.c
 * Run:     mpirun -np 4 ./game_of_life [global_nx] [global_ny] [generations]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define DEFAULT_NX 20
#define DEFAULT_NY 20
#define DEFAULT_GEN 10

/* Access cell (i,j) in a grid with halo, total columns = local_ny+2 */
#define CELL(grid, i, j) ((grid)[(i) * (local_ny + 2) + (j)])

/* Count live neighbors around cell (i,j) */
static int count_neighbors(int *grid, int i, int j, int local_ny) {
    return CELL(grid, i - 1, j - 1) + CELL(grid, i - 1, j) + CELL(grid, i - 1, j + 1) +
           CELL(grid, i,     j - 1) +                          CELL(grid, i,     j + 1) +
           CELL(grid, i + 1, j - 1) + CELL(grid, i + 1, j) + CELL(grid, i + 1, j + 1);
}

/* Print the full global grid gathered on process 0 */
static void print_grid(int *grid, int *global, int local_nx, int local_ny,
                        int global_nx, int global_ny, int dims[2],
                        MPI_Comm cart_comm, int rank) {
    int coords[2];
    int i, j;

    /* Pack local interior into a contiguous buffer */
    int *local_buf = malloc(local_nx * local_ny * sizeof(int));
    for (i = 0; i < local_nx; i++)
        for (j = 0; j < local_ny; j++)
            local_buf[i * local_ny + j] = CELL(grid, i + 1, j + 1);

    if (rank == 0) {
        /* Process 0 copies its own data first */
        MPI_Cart_coords(cart_comm, 0, 2, coords);
        int sx = coords[0] * local_nx;
        int sy = coords[1] * local_ny;
        for (i = 0; i < local_nx; i++)
            for (j = 0; j < local_ny; j++)
                global[(sx + i) * global_ny + (sy + j)] = local_buf[i * local_ny + j];

        /* Receive from all other processes */
        int *recv_buf = malloc(local_nx * local_ny * sizeof(int));
        int p;
        for (p = 1; p < dims[0] * dims[1]; p++) {
            MPI_Recv(recv_buf, local_nx * local_ny, MPI_INT, p, 99,
                     cart_comm, MPI_STATUS_IGNORE);
            MPI_Cart_coords(cart_comm, p, 2, coords);
            sx = coords[0] * local_nx;
            sy = coords[1] * local_ny;
            for (i = 0; i < local_nx; i++)
                for (j = 0; j < local_ny; j++)
                    global[(sx + i) * global_ny + (sy + j)] = recv_buf[i * local_ny + j];
        }
        free(recv_buf);

        /* Print */
        for (i = 0; i < global_nx; i++) {
            for (j = 0; j < global_ny; j++)
                printf("%c ", global[i * global_ny + j] ? '#' : '.');
            printf("\n");
        }
        printf("\n");
    } else {
        MPI_Send(local_buf, local_nx * local_ny, MPI_INT, 0, 99, cart_comm);
    }
    free(local_buf);
}

int main(int argc, char *argv[]) {
    int rank, nprocs;
    int global_nx, global_ny, generations;
    int dims[2] = {0, 0};
    int periods[2] = {1, 1};  /* Periodic boundary conditions */
    int coords[2];
    MPI_Comm cart_comm;
    int north, south, east, west;
    int local_nx, local_ny;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* Parse arguments */
    global_nx   = (argc > 1) ? atoi(argv[1]) : DEFAULT_NX;
    global_ny   = (argc > 2) ? atoi(argv[2]) : DEFAULT_NY;
    generations = (argc > 3) ? atoi(argv[3]) : DEFAULT_GEN;

    /* Step 1-2: Compute suitable 2D process grid */
    MPI_Dims_create(nprocs, 2, dims);

    /* Step 3: Create 2D Cartesian communicator with periodic BCs */
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &rank);

    /* Step 4: Get 2D coordinates */
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    /* Step 5: Compute local subgrid size */
    local_nx = global_nx / dims[0];
    local_ny = global_ny / dims[1];

    if (rank == 0) {
        printf("=== Conway's Game of Life (MPI 2D Cartesian) ===\n");
        printf("Global grid : %d x %d\n", global_nx, global_ny);
        printf("Process grid: %d x %d (%d processes)\n", dims[0], dims[1], nprocs);
        printf("Local grid  : %d x %d (per process)\n", local_nx, local_ny);
        printf("Generations : %d\n", generations);
        printf("Boundary    : Periodic (wrap-around)\n\n");
    }

    /* Step 6: Determine 4 direct neighbors */
    MPI_Cart_shift(cart_comm, 0, 1, &north, &south);  /* dim 0 = rows */
    MPI_Cart_shift(cart_comm, 1, 1, &west, &east);    /* dim 1 = columns */

    printf("Rank %d coords=(%d,%d): N=%d S=%d W=%d E=%d\n",
           rank, coords[0], coords[1], north, south, west, east);

    /* Allocate local grids with halo border */
    int grid_size = (local_nx + 2) * (local_ny + 2);
    int *grid     = calloc(grid_size, sizeof(int));
    int *new_grid = calloc(grid_size, sizeof(int));

    /* Initialize with a deterministic pattern (Glider + random) */
    srand(42 + rank);
    int i, j;
    for (i = 1; i <= local_nx; i++)
        for (j = 1; j <= local_ny; j++)
            CELL(grid, i, j) = (rand() % 5 == 0) ? 1 : 0;  /* ~20% density */

    /* Create MPI derived type for column exchange */
    MPI_Datatype col_type;
    MPI_Type_vector(local_nx + 2, 1, local_ny + 2, MPI_INT, &col_type);
    MPI_Type_commit(&col_type);

    /* Gather and print initial state */
    int *global_grid = NULL;
    if (rank == 0) {
        global_grid = malloc(global_nx * global_ny * sizeof(int));
        printf("Initial state:\n");
    }
    print_grid(grid, global_grid, local_nx, local_ny,
               global_nx, global_ny, dims, cart_comm, rank);

    /* Step 7: Generation loop */
    double t_start = MPI_Wtime();
    int gen;

    for (gen = 0; gen < generations; gen++) {
        MPI_Status status;

        /* Exchange N/S rows (interior columns only) */
        /* Send last row south, receive top ghost from north */
        MPI_Sendrecv(&CELL(grid, local_nx, 1), local_ny, MPI_INT, south, 0,
                     &CELL(grid, 0, 1),        local_ny, MPI_INT, north, 0,
                     cart_comm, &status);
        /* Send first row north, receive bottom ghost from south */
        MPI_Sendrecv(&CELL(grid, 1, 1),            local_ny, MPI_INT, north, 1,
                     &CELL(grid, local_nx + 1, 1), local_ny, MPI_INT, south, 1,
                     cart_comm, &status);

        /* Exchange E/W columns (full height including ghost rows → fills corners) */
        /* Send last col east, receive left ghost from west */
        MPI_Sendrecv(&CELL(grid, 0, local_ny), 1, col_type, east, 2,
                     &CELL(grid, 0, 0),         1, col_type, west, 2,
                     cart_comm, &status);
        /* Send first col west, receive right ghost from east */
        MPI_Sendrecv(&CELL(grid, 0, 1),            1, col_type, west, 3,
                     &CELL(grid, 0, local_ny + 1), 1, col_type, east, 3,
                     cart_comm, &status);

        /* Apply Game of Life rules on internal cells */
        for (i = 1; i <= local_nx; i++) {
            for (j = 1; j <= local_ny; j++) {
                int n = count_neighbors(grid, i, j, local_ny);
                if (CELL(grid, i, j) == 1)
                    CELL(new_grid, i, j) = (n == 2 || n == 3) ? 1 : 0;
                else
                    CELL(new_grid, i, j) = (n == 3) ? 1 : 0;
            }
        }

        /* Swap grids */
        int *tmp = grid;
        grid = new_grid;
        new_grid = tmp;
    }

    double t_end = MPI_Wtime();

    /* Step 8: Gather and print final grid */
    MPI_Barrier(cart_comm);
    if (rank == 0)
        printf("Final state (after %d generations):\n", generations);
    print_grid(grid, global_grid, local_nx, local_ny,
               global_nx, global_ny, dims, cart_comm, rank);

    /* Count total live cells */
    int local_alive = 0;
    for (i = 1; i <= local_nx; i++)
        for (j = 1; j <= local_ny; j++)
            local_alive += CELL(grid, i, j);

    int global_alive;
    MPI_Reduce(&local_alive, &global_alive, 1, MPI_INT, MPI_SUM, 0, cart_comm);

    if (rank == 0) {
        printf("Live cells  : %d / %d\n", global_alive, global_nx * global_ny);
        printf("Time        : %.4f s\n", t_end - t_start);
    }

    /* Cleanup */
    MPI_Type_free(&col_type);
    free(grid);
    free(new_grid);
    if (global_grid) free(global_grid);
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
