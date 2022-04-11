#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *text[] = {
    "Hi, I am Hank.",
    "What's your name?",
    "Nice to meet you!",
    "Goodbye!",
    "Linux write file test!!"
};

int main() {
    srand(time(NULL));

    /*
     *  R(read)/W(write)/X(execute)
     *  Owner: S_IRWXU, S_IRUSR, S_IWUSR, S_IXUSR
     *  Group: S_IRWXG, S_IRGRP, S_IWGRP, S_IXGRP
     *  Other: S_IRWXO, S_IROTH, S_IWOTH, S_IXOTH
     */
    int fd = open("./writing_test.txt",
        O_WRONLY | O_CREAT | O_TRUNC /*O_APPEND*/,
        S_IRUSR | S_IWUSR);

    /*
     *  create system call is equivalent to
     *  open(filename, O_WRONLY | O_CREAT | O_TRUNC)
     *  e.g. fd = create(filename, mode);
     */

    if (fd == -1) {
        printf("<%d>: Open file failed... ERR: %d\n", getpid(), errno);
        exit(EXIT_FAILURE);
    }

    int idx = rand() % 5;
    size_t length = strlen(text[idx]);
    ssize_t wc;

    /*
     *  Kernel has a buffer for write, so the dirty buffer 
     *  will not be write back to disk immediately.
     */
    wc = write(fd, text[idx], length);
    if (wc == -1) {
         printf("<%d>: Write buffer to file failed... ERR: %d\n", getpid(), errno);
    } else if (wc != length) { // partial write case
        // In this case, 'errno' will not be set.
        printf("<%d>: Write count is not equal to buffer length\n", getpid());
    } else {
        printf("<%d>: Write '%s' to file successfully\n", getpid(), text[idx]);
    }

    /*
     *  fsync not return until data and metadata is written back to disk.
     *  fdatasync only ensure data is written back to disk.
     *  so the later has better performance.
     */
    if (fsync(fd) == -1) { // fdatasync(fd)
        perror("fsync failed");
    }

    if (close(fd) == -1) {
        perror("Close file failed...");
    }

    return 0;
}