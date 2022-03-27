#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>

#include "semun.h"

static int sem_id;

static int init_sem(int semId) {
    union semun sem_union;
    // set semaphore value to 0 as default init
    sem_union.val = 0;
    return (semctl(semId, 0, SETVAL, sem_union) == 0); // 0: successful, -1: failed
}

// static int del_sem(int semId) {
//     union semun sem_union;
//     return (semctl(semId, 0, IPC_RMID, sem_union) == 0); // 0: successful, -1: failed
// }

// return: 0 failed, 1: successful
static int wait(int semId) {
    struct sembuf sem_b;
    sem_b.sem_num = 0; // semaphore index in array
    sem_b.sem_op = -1; // P(); decrease sem value by 1
    sem_b.sem_flg = SEM_UNDO;
    return (semop(semId, &sem_b, 1/* number of ops*/) == 0); // 0: successful, -1: failed
}

// return: 0 failed, 1: successful
static int notify(int semId) {
    struct sembuf sem_b;    
    sem_b.sem_num = 0; // semaphore index in array
    sem_b.sem_op = 1; // V(); increase sem value by 1
    sem_b.sem_flg = SEM_UNDO;
    return (semop(sem_id, &sem_b, 1/* number of ops*/) == 0); // 0: successful, -1: failed
}

int main(int argc, char *argv[]) {

    // Get or create a semaphore
    sem_id = semget((key_t)1234, 1/*number of semaphores*/, IPC_CREAT | 0666);

    if (init_sem(sem_id) == 0) {
        perror("Semaphore initialization failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++) {
        printf("%d, Producer producing...\n", getpid());
        if (notify(sem_id) == 0) {
            perror("Failed to wait\n");
            break;
        }
        printf("%d, Producer produce one product\n", getpid());
        sleep(5);
    }

    // Leave the deletion to consumer
    // if (del_sem(sem_id) == 0) {
    //     perror("Semaphore deletion failed\n");
    // }

    printf("%d, Producer exited...\n", getpid());

    return 0;
}