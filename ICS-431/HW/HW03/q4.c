#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 40

int A[MATRIX_SIZE][MATRIX_SIZE];
int B[MATRIX_SIZE][MATRIX_SIZE];
int sum[MATRIX_SIZE][MATRIX_SIZE];
int diff[MATRIX_SIZE][MATRIX_SIZE];
int product[MATRIX_SIZE][MATRIX_SIZE];

void* compute_sum(void* arg) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            sum[i][j] = A[i][j] + B[i][j];
        }
    }
    pthread_exit(NULL);
}

void* compute_diff(void* arg) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            diff[i][j] = A[i][j] - B[i][j];
        }
    }
    pthread_exit(NULL);
}

int main() {
    // initialize matrices A and B with random binary values
    srand(time(NULL));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            A[i][j] = rand() % 2;
            B[i][j] = rand() % 2;
        }
    }

    // create threads for computing sum and difference
    pthread_t sum_thread, diff_thread;
    pthread_create(&sum_thread, NULL, compute_sum, NULL);
    pthread_create(&diff_thread, NULL, compute_diff, NULL);

    // wait for threads to complete
    pthread_join(sum_thread, NULL);
    pthread_join(diff_thread, NULL);

    // compute product of sum and difference matrices
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int val = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                val += sum[i][k] * diff[k][j];
            }
            product[i][j] = val;
        }
    }

    // print product matrix
    printf("Product of sum and difference matrices:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", product[i][j]);
        }
        printf("\n");
    }

    return 0;
}
