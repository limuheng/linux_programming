#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

void sig_handler(int sig){
    int retval;
    if (sig == SIGINT){
        printf("%d, [SIGNAL] CATCH SIGNAL: SIGINT\n", getpid());

        // Reset to default signal handler
        // printf("%d, [SIGNAL] Restore to default handler\n", getpid());
        // if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        //     perror("Signal Error");
        //     exit(EXIT_FAILURE);
        // }
    }
}

int main()
{
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("Signal Error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("%d, [SIGNAL] Hallo World!\n", getpid());
        sleep(2);
    }
	return 0;
}