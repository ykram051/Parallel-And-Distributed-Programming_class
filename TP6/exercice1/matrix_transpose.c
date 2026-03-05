/*
 * Exercise 1: Matrix Transposition using MPI Derived Types
 *
 * Compile: mpicc -o matrix_transpose matrix_transpose.c
 * Run:     mpiexec -n 2 ./matrix_transpose
 */

#include <stdio.h>
#include <mpi.h>

#define ROWS 4
#define COLS 5

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        /* Process 0: Initialize and send the matrix */
        double a[ROWS][COLS];
        int val = 1;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                a[i][j] = val++;

        printf("Process 0 - Matrix a:\n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++)
                printf("%4.0f ", a[i][j]);
            printf("\n");
        }

        /* Send the raw matrix as a flat block of ROWS*COLS doubles */
        MPI_Send(&a[0][0], ROWS * COLS, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    } else {
        /* Process 1: Receive the transposed matrix using derived types */
        double at[COLS][ROWS];

        /*
         * We need to receive a ROWS x COLS row-major matrix and store it
         * transposed into a COLS x ROWS row-major matrix.
         *
         * The source matrix (row-major, ROWS x COLS):
         *   a[i][j] is at offset (i * COLS + j) * sizeof(double)
         *
         * The destination at[j][i] should equal a[i][j].
         * at[j][i] is at offset (j * ROWS + i) * sizeof(double) in at.
         *
         * Strategy: Build a type that describes where each incoming element
         * goes in the at buffer.
         *
         * One column of the source matrix (elements a[0][j], a[1][j], ..., a[ROWS-1][j])
         * should go into one row of at (elements at[j][0], at[j][1], ..., at[j][ROWS-1]),
         * which are contiguous in memory.
         *
         * Step 1: MPI_Type_vector to pick one column from the source.
         *   - count = ROWS (number of elements in one column)
         *   - blocklength = 1 (one element per block)
         *   - stride = COLS (distance between elements in the same column)
         *   This gives us: elements at positions 0, COLS, 2*COLS, ..., (ROWS-1)*COLS
         *   in units of doubles.
         *
         * But we want these ROWS elements to land contiguously in at (one row of at).
         * So we use a resized type for the column vector.
         *
         * Step 2: Resize the column type so its extent is sizeof(double).
         *   This way, when we replicate it with hvector, each copy starts
         *   one double later in the receive buffer.
         *
         * Wait - let me think about this differently.
         *
         * Actually, we want to describe the RECEIVE layout.
         * The data arrives as ROWS*COLS doubles in row-major order of the source.
         * Element (i,j) of source arrives at position (i*COLS + j).
         *
         * We want element at position (i*COLS + j) to go to at[j][i],
         * i.e., memory offset (j*ROWS + i) in the at buffer.
         *
         * Build a derived type for receiving:
         *
         * Column type: picks one column of the *source* layout
         *   - ROWS blocks of 1, stride COLS (in source)
         *   These elements should land in one row of at (contiguous).
         *
         * Actually, let me use a different approach that matches the exercise instructions:
         *
         * MPI_Type_vector: create a type for one column of the source matrix.
         *   count=ROWS, blocklength=1, stride=ROWS (stride in the RECEIVE buffer)
         *   ... No, the type describes the receive buffer layout.
         *
         * Let me think of it as describing the memory layout in the receive buffer.
         * We receive ROWS*COLS doubles. The receive type must map incoming double #k
         * to the right position in at.
         *
         * Incoming double #k corresponds to source element (k/COLS, k%COLS) = (i,j).
         * It must go to at[j][i] = position (j*ROWS + i) in the at buffer.
         *
         * So the mapping is: incoming position k -> buffer position ((k%COLS)*ROWS + k/COLS).
         *
         * Let's think column by column of the SOURCE:
         * - Source column j has elements at incoming positions j, COLS+j, 2*COLS+j, ..., (ROWS-1)*COLS+j
         * - These should go to at[j][0..ROWS-1] = buffer positions j*ROWS, j*ROWS+1, ..., j*ROWS+ROWS-1
         *
         * In the receive buffer, one "transposed column" (= one row of at) is
         * ROWS contiguous doubles. There are COLS such rows, each separated by
         * ROWS doubles.
         *
         * So the receive type should describe:
         * - COLS groups (one per source column / one per row of at)
         * - Each group has ROWS contiguous doubles
         * - Groups are spaced ROWS doubles apart
         * But that's just a contiguous block of COLS*ROWS doubles -- that doesn't do transposition.
         *
         * The transposition happens because the SEND side sends data in one order
         * and the RECEIVE side interprets it differently.
         *
         * Actually, for MPI derived types, the type describes the memory layout.
         * MPI_Recv with a derived type means: take the incoming bytes and place
         * them into the buffer according to the type map.
         *
         * So if we use a regular MPI_Send of ROWS*COLS contiguous doubles,
         * and we want them rearranged on the receive side, we build a receive type
         * that maps each incoming element to the transposed position.
         *
         * The receive type for transposition on process 1:
         *
         * We want incoming element #(i*COLS+j) to go to at[j][i] = offset (j*ROWS+i)*sizeof(double).
         *
         * Think of it this way: the incoming data is row by row of the source.
         * Row i of source: elements a[i][0], a[i][1], ..., a[i][COLS-1]
         * These should go to: at[0][i], at[1][i], ..., at[COLS-1][i]
         * In memory: offsets i, ROWS+i, 2*ROWS+i, ..., (COLS-1)*ROWS+i (in units of double)
         *
         * So one row of the source maps to one column of at.
         * A column of at: COLS elements, each separated by ROWS doubles.
         *
         * Step 1 (MPI_Type_vector): one column of at (= where one row of source goes)
         *   count = COLS
         *   blocklength = 1
         *   stride = ROWS
         * This creates a type for COLS elements spaced ROWS apart.
         *
         * Step 2 (MPI_Type_create_hvector): replicate for all ROWS rows of source
         *   count = ROWS
         *   blocklength = 1
         *   stride = sizeof(double)  (each successive column of at starts one double later)
         *
         * This builds the full transposition layout!
         */

        MPI_Datatype col_type, col_type_resized, transpose_type;

        /* Step 1: A column of at = COLS elements, each ROWS apart */
        MPI_Type_vector(COLS, 1, ROWS, MPI_DOUBLE, &col_type);

        /* Step 2: Resize so extent = sizeof(double), allowing next column to start
         * one double later */
        MPI_Type_create_resized(col_type, 0, sizeof(double), &col_type_resized);

        /* Step 3: Replicate ROWS times with stride = sizeof(double) using hvector
         * Actually with the resized type, we can just use contiguous or hvector.
         * Since the resized extent is sizeof(double), creating ROWS copies with
         * MPI_Type_contiguous would place each one extent apart = sizeof(double). */
        MPI_Type_contiguous(ROWS, col_type_resized, &transpose_type);

        MPI_Type_commit(&transpose_type);

        MPI_Recv(&at[0][0], 1, transpose_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("\nProcess 1 - Matrix transposee at:\n");
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++)
                printf("%4.0f ", at[i][j]);
            printf("\n");
        }

        MPI_Type_free(&transpose_type);
        MPI_Type_free(&col_type_resized);
        MPI_Type_free(&col_type);
    }

    MPI_Finalize();
    return 0;
}
