#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

double sum_u1(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 1) {
        sum += a[i];
    }
    return sum;
}

double sum_u2(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 2) {
        sum += a[i] + a[i + 1];
    }
    return sum;
}

double sum_u3(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 3) {
        sum += a[i] + a[i + 1] + a[i + 2];
    }
    return sum;
}

double sum_u4(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 4) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3];
    }
    return sum;
}

double sum_u5(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 5) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4];
    }
    return sum;
}

double sum_u6(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 6) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5];
    }
    return sum;
}

double sum_u7(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 7) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6];
    }
    return sum;
}

double sum_u8(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 8) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7];
    }
    return sum;
}

double sum_u9(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 9) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8];
    }
    return sum;
}

double sum_u10(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 10) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9];
    }
    return sum;
}

double sum_u11(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 11) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10];
    }
    return sum;
}

double sum_u12(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 12) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11];
    }
    return sum;
}

double sum_u13(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 13) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12];
    }
    return sum;
}

double sum_u14(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 14) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13];
    }
    return sum;
}

double sum_u15(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 15) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14];
    }
    return sum;
}

double sum_u16(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 16) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15];
    }
    return sum;
}

double sum_u17(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 17) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16];
    }
    return sum;
}

double sum_u18(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 18) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17];
    }
    return sum;
}

double sum_u19(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 19) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18];
    }
    return sum;
}

double sum_u20(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 20) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19];
    }
    return sum;
}

double sum_u21(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 21) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20];
    }
    return sum;
}

double sum_u22(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 22) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21];
    }
    return sum;
}

double sum_u23(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 23) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22];
    }
    return sum;
}

double sum_u24(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 24) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23];
    }
    return sum;
}

double sum_u25(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 25) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24];
    }
    return sum;
}

double sum_u26(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 26) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25];
    }
    return sum;
}

double sum_u27(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 27) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26];
    }
    return sum;
}

double sum_u28(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 28) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27];
    }
    return sum;
}

double sum_u29(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 29) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28];
    }
    return sum;
}

double sum_u30(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 30) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29];
    }
    return sum;
}

double sum_u31(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 31) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29] + a[i + 30];
    }
    return sum;
}

double sum_u32(double *a) {
    double sum = 0.0;
    for (int i = 0; i < N; i += 32) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29] + a[i + 30] + a[i + 31];
    }
    return sum;
}

int main() {
    double *a = malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) a[i] = 1.0;

    typedef double (*sum_func)(double*);

    sum_func funcs[33] = {NULL, sum_u1, sum_u2, sum_u3, sum_u4, sum_u5, sum_u6, sum_u7, sum_u8, sum_u9, sum_u10, sum_u11, sum_u12, sum_u13, sum_u14, sum_u15, sum_u16, sum_u17, sum_u18, sum_u19, sum_u20, sum_u21, sum_u22, sum_u23, sum_u24, sum_u25, sum_u26, sum_u27, sum_u28, sum_u29, sum_u30, sum_u31, sum_u32};

    double best_time = 1e9;
    int best_u = 1;

    for (int u = 1; u <= 32; u++) {
        double start = (double)clock() / CLOCKS_PER_SEC;
        double sum = funcs[u](a);
        double end = (double)clock() / CLOCKS_PER_SEC;
        double time = (end - start) * 1000;
        printf("U=%d, Sum = %f, Time = %f ms\n", u, sum, time);
        if (time < best_time) {
            best_time = time;
            best_u = u;
        }
    }

    printf("Best U = %d, Time = %f ms\n", best_u, best_time);

    free(a);
    return 0;
}