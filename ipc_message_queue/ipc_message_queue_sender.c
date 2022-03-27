#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#include "msg_com.h"

char *messages[5] = {
    "Hi, I am Hank.",
    "What's your name?",
    "Nice to meet you!",
    "Goodbye!",
    "end"
};

int main() {
    int msg_id;
    struct my_message msg;

    /* First, we set up the message queue. */
    msg_id = msgget((key_t)1234, IPC_CREAT | 0666);

    if (msg_id == -1) {
        perror("sender msgget failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++) {
        // message_type MUST not be 0, or msgsnd will get invalid arguments error.
        msg.message_type = 5 - i;
        memset(&msg.text, 0, TEXT_SZ); // clear msg.text
        strcpy(msg.text, messages[i]);

        printf("%d: send message: %s\n", getpid(), msg.text);
        if (msgsnd(msg_id, (void *)&msg, TEXT_SZ, 0) == -1) {
            printf("%d: msgsnd failed [ERROR:%d]\n", getpid(), errno);
            // Remove message queue if failed
            if (msgctl(msg_id, IPC_RMID, 0) == -1) {
                perror("msgctl(IPC_RMID) failed\n");
            }
            break;
        }
        sleep(3);
    }

    printf("%d: sender exit\n", getpid());

    return 0;
}


