#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>


void sig_handler(int signo, siginfo_t *info, void *p){
    if (signo == SIGINT) {
        printf("%d, CATCH SIGNAL: SIGINT (Sender PID:%d)\n", getpid(), info->si_pid); // sender pid
    }
}

int main()
{
    struct sigaction act, oact;
    // Clear mask for initialization
    sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_SIGINFO;
    // Reset to default signal handler
    // act.sa_flags |= SA_RESETHAND;
    act.sa_sigaction = sig_handler;

    if (sigaction(SIGINT, &act, &oact) == -1) {
        printf("Register sigaction failed: pid: %d, reason: %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    while(1) {
        printf("%d, [SIGACTION] Hallo World!\n", getpid());
        sleep(2);
    }
	return 0;
}