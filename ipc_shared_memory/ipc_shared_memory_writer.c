#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>

#include "shm_com.h"

char input[5][TEXT_SZ] = {
    "Hi, I am Hank.",
    "What's your name?",
    "Nice to meet you!",
    "Goodbye!",
    "end"
};

int main() {
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shm_id;

    // Get existed or creat new shared memory
    shm_id = shmget((key_t)1234, sizeof(struct shared_use_st), IPC_CREAT | 0666);

    if (shm_id == -1) {
        perror("shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // Make the shared memory accessible to the program.
    shared_memory = shmat(shm_id, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat failed\n");
        exit(EXIT_FAILURE);
    }

    shared_stuff = (struct shared_use_st *)shared_memory;
    shared_stuff->written_by_you = 0;
    for (int i = 0; i < 5; i++) {
        while(shared_stuff->written_by_you == 1) {
            printf("waiting for reader...\n");
            sleep(1);            
        }

        strncpy(shared_stuff->some_text, input[i], TEXT_SZ);
        shared_stuff->written_by_you = 1;
        printf("%d: writing '%s'...\n", getpid(), input[i]);
    }

    // Lastly, the shared memory is detached.
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    printf("%d: writer exits\n", getpid());

    return 0;
}