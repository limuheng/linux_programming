#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void *thread_routine1(void *arg);
void *thread_routine2(void *arg);

pthread_mutex_t mutex;
int count = 0;

int main() {
    pthread_t tid1, tid2;

    srand(time(NULL));

    // init mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    int result1 = pthread_create(&tid1, NULL, thread_routine1, NULL);
    if (result1 != 0) {
        printf("Failed to create thread1: %d\n", result1);
        exit(EXIT_FAILURE);
    }

    int result2 = pthread_create(&tid2, NULL, thread_routine2, NULL);
    if (result2 != 0) {
        printf("Failed to create thread2: %d\n", result2);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for thread1 to finish\n");
    pthread_join(tid1, NULL);
    printf("Thread1 returned\n");

    printf("Waiting for thread2 to finish\n");
    pthread_join(tid2, NULL);
    printf("Thread2 returned\n");

    // destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *thread_routine1(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        count++;
        printf("Thread1 count: %d\n", count);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 10);
    }
    pthread_exit(NULL);
}

void *thread_routine2(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        count++;
        printf("Thread2 count: %d\n", count);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 10);
    }
    pthread_exit(NULL);
}