#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

#include "msg_com.h"

int main() {
    int msg_id;
    struct my_message msg;
    long int msg_to_receive = -3; // only receive type less than 3

    msg_id = msgget((key_t)1234, IPC_CREAT | 0666);

    if (msg_id == -1) {
        perror("receiver msgget failed\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("%d: waiting for message...\n", getpid());
        if (msgrcv(msg_id, (void *)&msg, TEXT_SZ, msg_to_receive, 0) == -1) {
            printf("%d: msgrcv failed\n", getpid());
            break;
        }
        printf("%d: receive message: %s\n", getpid(), msg.text);
        if (strncmp(msg.text, "end", 3) == 0) {
            break;
        }
    }

    // Remove message queue
    if (msgctl(msg_id, IPC_RMID, 0) == -1) {
        perror("msgctl(IPC_RMID) failed\n");
    }

    printf("%d: receiver exit\n", getpid());

    return 0;
}
