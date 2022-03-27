#include <iostream>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void *thread_routine(void *arg);

char thread_arg[] = "My first pthread";

int main() {
    pthread_t tid;
    void *thread_result;
    char errMsg[256];

    // Return 0 is success, otherwise will return an error number.
    int result = pthread_create(&tid, nullptr, thread_routine, thread_arg);

    if (result != 0) {
        cout << "Failed to create thread: " << result << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Thread-" << pthread_self() << ": Wait for thread to finish..." << endl;

    result = pthread_join(tid, &thread_result);
    if (result != 0) {
        cout << "Failed to join thread: " << result << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Thread-" << pthread_self() << ": Join returned from: " << (char *)thread_result << endl;

    return 0;
}

void *thread_routine(void *arg) {
    string msg = "Thread-" + std::to_string(pthread_self());
    cout << msg << ": " << (char *)arg << endl;
    sleep(3);
    pthread_exit((void *)((new string(msg))->c_str()));
}