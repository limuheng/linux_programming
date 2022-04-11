#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <memory.h>


int main() {
    int fd = open("./news.txt", O_RDONLY);

    if (fd == -1) {
        printf("<%d>: Open file failed... ERR: %d\n", getpid(), errno);
        exit(EXIT_FAILURE);
    }

    ssize_t ret;
    char buf[128];

    // read size - 1 bytes
    while((ret = read(fd, buf, sizeof(buf) - 1)) != 0) {
        if (ret == -1) {
            // read was interrupted by signal, just reissue the read system call
            if (errno == EINTR) {
                continue;
            }
            printf("<%d>: read failed. errno: %d", getpid(), errno);
            break;
        } else {
            printf("<%d>: read text: %s\n", getpid(), buf);
            // clear buffer
            memset(buf, 0, sizeof(buf));
        }
    }

    if (close(fd) == -1) {
        printf("<%d>: Close file failed...", getpid());
    }

    printf("<%d>: read file completed", getpid());

    return 0;
}