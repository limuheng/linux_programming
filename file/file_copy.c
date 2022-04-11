#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <memory.h>

int main() {
    int r_fd, w_fd;
    
    r_fd = open("./news.txt", O_RDONLY);
    w_fd = open("./news(copy).txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (r_fd == -1 || w_fd == -1) {
        printf("<%d>: Open file failed... ERR: %d\n", getpid(), errno);
        printf("<%d>: read fd: %d, write fd: %d\n", getpid(), r_fd, w_fd);
        exit(EXIT_FAILURE);
    }

    ssize_t ret;
    size_t buf_size = 128;
    void *buf = (void *)malloc(buf_size);

    while((ret = read(r_fd, buf, buf_size)) != 0) {
        if (ret == -1) {
            // read was interrupted by signal, just reissue the read system call
            if (errno == EINTR) {
                continue;
            }
            printf("<%d>: read failed. errno: %d", getpid(), errno);
            break;
        } else {
            ssize_t wc;
            wc = write(w_fd, buf, ret);
            if (wc == -1) {
                printf("<%d>: Write buffer to file failed... ERR: %d\n", getpid(), errno);
                break;
            }
            printf("<%d> copy %ld bytes\n", getpid(), wc);
            // clear buffer
            memset(buf, 0, sizeof(buf));
        }
    }

    if (close(r_fd) == -1) {
        printf("<%d>: Close read file failed...", getpid());
    }
    if (close(w_fd) == -1) {
        printf("<%d>: Close write file failed...", getpid());
    }

    printf("<%d>: Copy file completed", getpid());

    return 0;
}