#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t bin_sem;

void *thread_routine1(void *arg);
void *thread_routine2(void *arg);

int main() {
    pthread_t tid1, tid2;

    if (sem_init(&bin_sem, 0, 0) != 0) {
        perror("Semaphore initialization failed\n");
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

    sem_destroy(&bin_sem);

    return 0;
}

void *thread_routine2(void *arg) {
    sleep(3);
    int i = 1;
    while(i <= 10) {
        printf("Thread2 count: %d\n", i);
        i++;
        sleep(1);
        if (i % 2 == 0) {
            // Notify thread1 to printf
            sem_post(&bin_sem);
        }
    }
    printf("Thread2 finished\n");
    pthread_exit(NULL);
}

void *thread_routine1(void *arg) {
    sleep(3);
    int i = 1;
    do {
        printf("Thread1 is waiting\n");
        // Wait for semaphore
        sem_wait(&bin_sem);
        printf("Thread1 count: %d\n", i);
        i++;
    } while(i <= 3);
    printf("Thread1 finished\n");
    pthread_exit(NULL);
}
