#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* decrease available resources by count resources */
/* block if insufficient resources available */
/* return 0 on success, -1 on error */
int decrease_count(int count) {
    pthread_mutex_lock(&mutex);  // lock the mutex

    while (available_resources < count) {
        pthread_cond_wait(&cond, &mutex);
    }

    available_resources -= count;
    pthread_cond_broadcast(&cond);  // notify waiting threads
    pthread_mutex_unlock(&mutex);  // unlock the mutex

    return 0;
}

/* increase available resources by count */
int increase_count(int count) {
    pthread_mutex_lock(&mutex);  // lock the mutex

    available_resources += count;
    pthread_cond_broadcast(&cond);  // notify waiting threads
    pthread_mutex_unlock(&mutex);  // unlock the mutex

    return 0;
}
