#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_FORK_PROCESSES 1

void sig_handler(int signo, siginfo_t *info, void *p){
    if (signo == SIGCHLD) {
        printf("%d, Child process (%d) was dead...\n", getpid(), info->si_pid); // sender pid
    } else {
        printf("%d, Unhandled signal number: %d, sender: %d\n", getpid(), signo, info->si_pid);
    }
}

int main()
{
    srand(time(NULL));

    struct sigaction act, oact;
    // Clear mask for initialization
    sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_SIGINFO;
    act.sa_sigaction = sig_handler;

    if (sigaction(SIGCHLD, &act, &oact) == -1) {
        printf("Register sigaction failed: pid: %d, reason: %s\n", getpid(), strerror(errno));
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
            exit(-1);
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