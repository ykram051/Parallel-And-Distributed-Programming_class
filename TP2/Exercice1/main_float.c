#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

float sum_u1(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 1) {
        sum += a[i];
    }
    return sum;
}

float sum_u2(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 2) {
        sum += a[i] + a[i + 1];
    }
    return sum;
}

float sum_u3(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 3) {
        sum += a[i] + a[i + 1] + a[i + 2];
    }
    return sum;
}

float sum_u4(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 4) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3];
    }
    return sum;
}

float sum_u5(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 5) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4];
    }
    return sum;
}

float sum_u6(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 6) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5];
    }
    return sum;
}

float sum_u7(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 7) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6];
    }
    return sum;
}

float sum_u8(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 8) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7];
    }
    return sum;
}

float sum_u9(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 9) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8];
    }
    return sum;
}

float sum_u10(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 10) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9];
    }
    return sum;
}

float sum_u11(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 11) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10];
    }
    return sum;
}

float sum_u12(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 12) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11];
    }
    return sum;
}

float sum_u13(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 13) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12];
    }
    return sum;
}

float sum_u14(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 14) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13];
    }
    return sum;
}

float sum_u15(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 15) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14];
    }
    return sum;
}

float sum_u16(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 16) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15];
    }
    return sum;
}

float sum_u17(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 17) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16];
    }
    return sum;
}

float sum_u18(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 18) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17];
    }
    return sum;
}

float sum_u19(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 19) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18];
    }
    return sum;
}

float sum_u20(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 20) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19];
    }
    return sum;
}

float sum_u21(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 21) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20];
    }
    return sum;
}

float sum_u22(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 22) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21];
    }
    return sum;
}

float sum_u23(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 23) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22];
    }
    return sum;
}

float sum_u24(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 24) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23];
    }
    return sum;
}

float sum_u25(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 25) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24];
    }
    return sum;
}

float sum_u26(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 26) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25];
    }
    return sum;
}

float sum_u27(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 27) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26];
    }
    return sum;
}

float sum_u28(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 28) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27];
    }
    return sum;
}

float sum_u29(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 29) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28];
    }
    return sum;
}

float sum_u30(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 30) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29];
    }
    return sum;
}

float sum_u31(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 31) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29] + a[i + 30];
    }
    return sum;
}

float sum_u32(float *a) {
    float sum = 0.0f;
    for (int i = 0; i < N; i += 32) {
        sum += a[i] + a[i + 1] + a[i + 2] + a[i + 3] + a[i + 4] + a[i + 5] + a[i + 6] + a[i + 7] + a[i + 8] + a[i + 9] + a[i + 10] + a[i + 11] + a[i + 12] + a[i + 13] + a[i + 14] + a[i + 15] + a[i + 16] + a[i + 17] + a[i + 18] + a[i + 19] + a[i + 20] + a[i + 21] + a[i + 22] + a[i + 23] + a[i + 24] + a[i + 25] + a[i + 26] + a[i + 27] + a[i + 28] + a[i + 29] + a[i + 30] + a[i + 31];
    }
    return sum;
}

int main() {
    float *a = malloc(N * sizeof(float));
    for (int i = 0; i < N; i++) a[i] = 1.0f;

    typedef float (*sum_func)(float*);

    sum_func funcs[33] = {NULL, sum_u1, sum_u2, sum_u3, sum_u4, sum_u5, sum_u6, sum_u7, sum_u8, sum_u9, sum_u10, sum_u11, sum_u12, sum_u13, sum_u14, sum_u15, sum_u16, sum_u17, sum_u18, sum_u19, sum_u20, sum_u21, sum_u22, sum_u23, sum_u24, sum_u25, sum_u26, sum_u27, sum_u28, sum_u29, sum_u30, sum_u31, sum_u32};

    float best_time = 1e9f;
    int best_u = 1;

    for (int u = 1; u <= 32; u++) {
        double start = (double)clock() / CLOCKS_PER_SEC;
        float sum = funcs[u](a);
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