#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_N 100

int fib[MAX_N];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fibonacci(void *param) {
    int i, first = 0, second = 1;

    for (i = 0; i < MAX_N; i++) {
        pthread_mutex_lock(&mutex);

        if (i <= 1)
            fib[i] = i;
        else {
            fib[i] = first + second;
            first = second;
            second = fib[i];
        }

        count++;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}

int main() {
    int i;
    pthread_t tid;

    pthread_create(&tid, NULL, fibonacci, NULL);

    for (i = 0; i < MAX_N; i++) {
        pthread_mutex_lock(&mutex);

        while (count <= i)
            pthread_cond_wait(&cond, &mutex);

        printf("%d ", fib[i]);

        pthread_mutex_unlock(&mutex);
    }

    printf("\n");

    return 0;
}
