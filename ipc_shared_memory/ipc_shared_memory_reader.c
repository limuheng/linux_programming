#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>

#include "shm_com.h"

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
    while (1) {
        if (shared_stuff->written_by_you) {
            char buffer[TEXT_SZ] = { 0 };
            strncpy(buffer, shared_stuff->some_text, TEXT_SZ);
            printf("%d: read shared memory = %s\n", getpid(), buffer);
            shared_stuff->written_by_you = 0;
            if (strncmp(buffer, "end", 3) == 0) {
                break;
            }
        }
        printf("waiting for writer...\n");
        sleep(3);
    }

    // Lastly, the shared memory is detached and then deleted.
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        perror("shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    printf("%d: reader exits\n", getpid());

    return 0;
}