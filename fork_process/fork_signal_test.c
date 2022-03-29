#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define MAX_FORK_PROCESSES 3

void sig_handler(int sig){
    int retval;
    if (sig == SIGCHLD){
        printf("%d, CATCH SIGNAL: SIGCHLD\n", getpid());
    }
}

int main()
{
    srand(time(NULL));
    if (signal(SIGCHLD, sig_handler) == SIG_ERR) {
        perror("Signal Error");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    for (int i = 0; i < MAX_FORK_PROCESSES; i++) {
        /**
         * rand() must be call in parent process.
         * If called in (pid == 0), it will all be same number.
         **/
        int sleepTime = rand() % 10;
        pid = fork();
        if (pid == 0) { // child process
            printf("%d, [Child] pid: %d\n", getpid(), getpid());
            printf("%d, [Child] sleep %d seconds\n", getpid(), sleepTime);
            sleep(sleepTime);
            printf("%d, [Child] exit\n", getpid());
            exit(EXIT_SUCCESS);
        } else if (pid == -1) { // fork error
            printf("fork error: %s, pid: %d\n", strerror(errno), getpid());
            exit(EXIT_FAILURE);
        } else { // parent process
            //printf("%d, [Parent] pid: %d\n", getpid(), getpid());
            printf("%d, [Parent] Child pid: %d\n", getpid(), pid);
            // If use wait, parent will be blocked until child process done.
            // wait(NULL);
        }
    }
    int i = 0;
    while (i < MAX_FORK_PROCESSES) {
        // pause parent to wait for SIGCHLD
        pause();
        i++;
    }
    printf("%d, [Parent] exit\n", getpid());
	return 0;
}
